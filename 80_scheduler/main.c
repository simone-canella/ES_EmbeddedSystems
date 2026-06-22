/**
 * @file main.c
 * @author simon
 * @brief Main execution entry point containing peripheral initializations and the 
 * real-time, time-triggered cooperative task scheduler loop.
 */

#include "../library/pwm.c"
#include "../library/pwm.h"
#include "../library/scheduler.c"
#include "../library/scheduler.h"
#include "../library/timer.c"
#include "../library/timer.h"
#include "../library/uart.c"
#include "../library/uart.h"
#include <stdbool.h>
#include <stdio.h>
#include <xc.h>

#define MAX_TASKS 4 // Number of tasks scheduled

// FUNCTION PROTOTYPES
void scheduler_setup(void);
void task_parse_uart(void *param);
void task_poll_button(void *param);
void task_update_pwm(void *param);
void task_manage_lights(void *param);

// DATA STRUCTURES
typedef struct {
	int speed;		   // Speed signal parsed from UART (-100 to 100%)
	int yawrate;	   // Yaw rate signal parsed from UART (-100 to 100%)
	bool motor_active; // true = PWM generation enabled, false = Disabled
} control_data;

// Global tracking structure instances
volatile control_data buggyState;
heartbeat schedInfo[MAX_TASKS]; // Array tracking timing info for all registered tasks

/**
 * @brief Configure initialization parameters, pin layouts, peripheral clocks,
 * and implement the primary periodic time-triggered real-time task framework.
 */
int main() {
	// SETUP PIN: Configure all pins as digital
	ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

	// Configure pin direction (0 = output, 1 = input)
	TRISBbits.TRISB8 = 0; // Left-lights (RB8)
	TRISFbits.TRISF1 = 0; // Right-lights (RF1)
	TRISFbits.TRISF0 = 0; // Brake-lights (RF0)
	TRISGbits.TRISG1 = 0; // Low-intensity-lights (RG1)
	TRISEbits.TRISE8 = 1; // Button2 (RE8)

	// SETUP TIMER
	tmr_setup_period(TIMER1, 100);

	// UART SETUP
	uart1_mikrobus2_setup();

	// PWM SETUP
	pwm_setup();

	// Scheduler INIT
	scheduler_setup();

	int missed_deadline = 0;

	// EXECUTION-LOOP
	while (1) {
		scheduler(schedInfo, MAX_TASKS); // Cycle through the task checklist

		// Enforce the execution frame duration alignment
		if (tmr_wait_period(TIMER1) == 1) {
			missed_deadline++; // Hardware-blocking delay until heartbeat interval completes
		}
	}

	return 0;
}

/**
 * @brief Initialize parameters, frequencies, offsets, activation flags, 
 * and function pointers for every registered real-time task block.
 */
void scheduler_setup() {
	// Task 0: Parse incoming telemetric UART strings
	schedInfo[0].n = 0;		 // Start immediately
	schedInfo[0].N = 1;		 // High frequency (runs every single heartbeat frame)
	schedInfo[0].enable = 1; // Active
	schedInfo[0].f = task_parse_uart;
	schedInfo[0].params = (void *)(&buggyState);

	// Task 1: Poll Button E8 for software debouncing (Slide 6 Phase Shifting)
	schedInfo[1].n = -1; // Staggered by -1 to offset CPU load spikes!
	schedInfo[1].N = 10; // Runs every 10 heartbeats (e.g., 10ms if HB is 1ms)
	schedInfo[1].enable = 1;
	schedInfo[1].f = task_poll_button;
	schedInfo[1].params = (void *)(&buggyState);

	// Task 2: Calculate kinematics blending and refresh duty cycles
	schedInfo[2].n = 0;
	schedInfo[2].N = 10; // Must be refreshed at 100 Hz (every 10ms)
	schedInfo[2].enable = 1;
	schedInfo[2].f = task_update_pwm;
	schedInfo[2].params = (void *)(&buggyState);

	// Task 3: Manage the lights state machine and flash blinkers
	schedInfo[3].n = 0;
	schedInfo[3].N = 250; // Check lights state (blinks require 250ms or 500ms intervals)
	schedInfo[3].enable = 1;
	schedInfo[3].f = task_manage_lights;
	schedInfo[3].params = (void *)(&buggyState);
}

// --------------------
// TASK FUNCTIONS
// --------------------

/**
 * @brief Process incoming streaming characters from the UART ring buffer inside
 * a non-blocking state machine frame parser to extract system control inputs.
 */
void task_parse_uart(void *param) {
	control_data *cd = (control_data *)param;

	static int current_state = 0; // 0 = WAIT_START, 1 = GATHER_DATA
	static char rx_buffer[64];
	static int rx_index = 0;

	char data;

	// Pull every available byte currently sitting in the background FIFO queue
	while ((data = uart1_read_char()) != RX_BUFFER_EMPTY) {
		if (current_state == 0) {
			if (data == '$') {
				rx_index = 0;
				rx_buffer[0] = '\0';
				current_state = 1; // Transition to GATHER_DATA
			}
		} else if (current_state == 1) {
			if (data == '*') {
				rx_buffer[rx_index] = '\0'; // Seal the payload string cleanly

				// Extract and validate ONLY when the frame is fully complete
				int temp_speed = 0;
				int temp_yawrate = 0;

				// Match prefix structure and pull arguments
				if (sscanf(rx_buffer, "REF,%d,%d", &temp_speed, &temp_yawrate) == 2) {
					// Enforce structural limits (-100% to +100%)
					if (temp_speed >= -100 && temp_speed <= 100 && temp_yawrate >= -100 && temp_yawrate <= 100) {
						cd->speed = temp_speed;
						cd->yawrate = temp_yawrate;
						uart1_write_string("OK\r\n"); // Success acknowledgment handshake
					} else {
						uart1_write_string("ERR\r\n"); // Out-of-bounds error
					}
				} else {
					uart1_write_string("ERR\r\n"); // Malformed text string error
				}

				current_state = 0; // Reset state machine to wait for next '$'
			} else {
				// Safely append byte while preventing array memory overflow strings
				if (rx_index < 63) {
					rx_buffer[rx_index++] = data;
				}
			}
		}
	}
}

/**
 * @brief Poll digital input pin RE8 to determine debounced edge transitions
 * and handle toggling the system's operational active flag.
 */
void task_poll_button(void *param) {
	control_data *cd = (control_data *)param;

	static int last_button_state = 1;
	int current_button_state = PORTEbits.RE8; // Sample button pin

	// debouncing logic tracking falling edge transitions
	if (last_button_state == 1 && current_button_state == 0) {
		cd->motor_active = !cd->motor_active; // activate the motor
	}

	last_button_state = current_button_state;
}

/**
 * @brief Calculate differential motor kinematics mapping and refresh duty
 * cycle variables at a fixed real-time interval framework loop rate.
 */
void task_update_pwm(void *param) {
	control_data *cd = (control_data *)param;

	if (cd->motor_active == 0) {
		// Force a full hardware bridge stop state if disabled
		execute_motion(0, 0);
	} else {
		// Drive the wheels using your differential kinematic engine
		execute_motion(cd->speed, cd->yawrate);
	}
}

/**
 * @brief Manage chassis signaling, lighting parameters, hazard configurations,
 * and track turning indicator flash states based on system kinematics.
 */
void task_manage_lights(void *param) {
	control_data *cd = (control_data *)param;

	static bool toggle = 0;
	toggle = !toggle; // Flip-flop toggle cadence generation

	if (cd->motor_active == 0) {
		LATFbits.LATF0 = 1; // Brakes ON
		LATGbits.LATG1 = 0; // Low-intensity OFF

		// 1 Hz Alternate hazards configuration
		if (toggle) {
			LATBbits.LATB8 = 1;
			LATFbits.LATF1 = 1;
		} else {
			LATBbits.LATB8 = 0;
			LATFbits.LATF1 = 0;
		}
	} else {
		LATFbits.LATF0 = 0; // Brakes OFF
		LATGbits.LATG1 = 1; // Low-intensity ON

		if (cd->yawrate > 50) { // LEFT-MOTION indicator at 2 Hz
			LATFbits.LATF1 = 0;

			if (toggle) {
				LATBbits.LATB8 = 1;
			} else {
				LATBbits.LATB8 = 0;
			}
		} else if (cd->yawrate < -50) { // RIGHT MOTION indicator at 2 Hz
			LATBbits.LATB8 = 0;

			if (toggle) {
				LATFbits.LATF1 = 1;
			} else {
				LATFbits.LATF1 = 0;
			}
		} else { // STRAIGHT MOTION
			LATFbits.LATF1 = 0;
			LATBbits.LATB8 = 0;
		}
	}
}