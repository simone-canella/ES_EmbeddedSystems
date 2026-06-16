/**
 * @file main.c
 * @author simon
 * @brief Main function: Advanced cyclic executive real-time command parser.
 */

#include "../library/timer.c"
#include "../library/timer.h"
#include "../library/uart.c"
#include "../library/uart.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <xc.h>

// FUNCTION PROTOTYPES
void algorithm(void);
void interrupt_setup(void);

// GLOBAL VARIABLES
volatile int received_chars = 0;  // Tracked globally across the main loop and background RX ISR
volatile int missed_deadline = 0; // Tracked globally across the main loop and background T4 ISR

int main() {
	// PIN SETUP: configure all pins as digital
	ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

	// Configuration pin direction (0 = output, 1 = input)
	TRISAbits.TRISA0 = 0; // LED1 (RA0)
	TRISGbits.TRISG9 = 0; // LED2 (RG9)
	TRISEbits.TRISE8 = 1; // Button2 (RE8)
	TRISEbits.TRISE9 = 1; // Button3 (RE9)

	// TIMER SETUP
	tmr_setup_period(TIMER1, 10); // timer for execution-loop at 100HZ (10ms)
	int led_counter = 0;

	// INTERRUPT SETUP
	interrupt_setup();

	// UART SETUP
	uart1_mikrobus2_setup();

	// INTERNAL VARIABLES
	int data; // character received from the UART1
	char string_command[4] = { '\0', '\0', '\0', '\0'}; // command received from the UART (3 chars + '\0')

	bool led2_blink_status = true; // boolean for understand if toggle or not the LED2

	// EXECUTION-LOOP at 100Hz
	while (1) {
		algorithm(); // perform algorithm computation

		data = uart1_read_char(); // read data from UART1

		while (data != RX_BUFFER_EMPTY) { // Buffer is not empty => i need to collect characters
			received_chars++;			  // increments the number of received chars

			// Shifting the values of the string
			string_command[3] = string_command[2];
			string_command[2] = string_command[1];
			string_command[1] = string_command[0];
			string_command[0] =
				(char)data; // new data is positioned in the first

			// Command interpretation
			if (string_command[2] == 'L' && string_command[1] == 'D') {
				if (string_command[0] == '1') {
					LATAbits.LATA0 = !LATAbits.LATA0; // toggle LED1
				} else if (string_command[0] == '2') {
					led2_blink_status = !led2_blink_status;
				}
			}

			// Refresh variable tracking
			data = uart1_read_char();
		}

		if (led_counter == 20) { // 200ms are passed => toggle LED2 at 2.5Hz
			if (led2_blink_status) {
				LATGbits.LATG9 = !LATGbits.LATG9; // toggle LED2
			}

			led_counter = 0; // reset the timer counter for LED2
		}

		led_counter++;

		if (tmr_wait_period(TIMER1) == 1) { // Execution-loop at 100Hz
			missed_deadline++;
		}
	}
	return 0;
}

/**
 * @brief Dummy function that need 7ms to be executed
 */
void algorithm() { 
    tmr_wait_ms(TIMER2, 7); 
}

// --------------------
// INTERRUPT FUNCTIONS
// --------------------
/**
 * @brief Configure the interrupt's: priority (IPC), flag(IFS), enable(IEC),
 * pheriphera-pin-select
 */
void interrupt_setup() {
	// PPS: Map External Interrupt 1 (INT1) to pin RPI88 (Button2 RE8)
	RPINR0bits.INT1R = 0b1011000;
    // PPS: Map External Interrupt 2 (INT2) to pin RPI89 (Button3 RE9)
    RPINR1bits.INT2R = 89;

	// INT1 Setup (Button2)
	IPC5bits.INT1IP = 1; // set priority
	IFS1bits.INT1IF = 0; // clear the flag
	IEC1bits.INT1IE = 1; // enable interrupt

	// Fixed: INT2 Setup (Button3)
    IPC7bits.INT2IP = 1; // set priority
    IFS1bits.INT2IF = 0; // clear the flag
    IEC1bits.INT2IE = 1; // enable interrupt

	// TIMER3
	IPC2bits.T3IP = 2; // set priority
	IFS0bits.T3IF = 0; // clear the flag
	IEC0bits.T3IE = 0; // keep disabled until button press

	// TIMER4 
    IPC6bits.T4IP = 2; // set priority
    IFS1bits.T4IF = 0; // clear the flag
    IEC1bits.T4IE = 0; // keep disabled until button press

	// Global Interrupt Switch
	INTCON2bits.GIE = 1; // Unlock system-wide interrupt vector execution
}

/**
 * @brief Button2 interrupt function
 *
 * @details When the button is pressed it starts a timer of 10ms for avoiding
 * debouncing, then calls the timer interrupt function
 */
void __attribute__((__interrupt__, __auto_psv__)) _INT1Interrupt(void) {
	IFS1bits.INT1IF = 0; // clear the flag

	// when the button is pressed starts a 10ms TIMER3 to avoid debouncing errors
	IEC0bits.T3IE = 1; // enable
	tmr_setup_period(TIMER3, 10);
}

/**
 * @brief Timer3 interrupt function is activated one time for sending the
 * message
 *
 * @details After the debouncing procedure, sending the current number of chars
 * received from the UART1
 */
void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void) {
	IFS0bits.T3IF = 0; // clear the TIMER3 flag
	IEC0bits.T3IE = 0; // disable TIMER3 interrupt
	T3CONbits.TON = 0; // turn off TIMER3

	char message[16];							// Safe memory allocation size
	sprintf(message, "C=%d\n", received_chars); // Formats text string automatically with '\0'
	uart1_write_string(message);
}

/**
 * @brief Button3 interrupt function
 *
 * @details When the button is pressed it starts a timer of 10ms for avoiding
 * debouncing, then calls the timer interrupt function
 */
void __attribute__((__interrupt__, __auto_psv__)) _INT2Interrupt(void) {
	IFS1bits.INT2IF = 0; // Fixed: Clears its own INT2 flag rather than targeting INT1

	// when the button is pressed starts a 10ms TIMER4 to avoid debouncing errors
	IEC1bits.T4IE = 1; // enable
	tmr_setup_period(TIMER4, 10);
}

/**
 * @brief Timer4 interrupt function is activated one time for sending the
 * message
 *
 * @details After the debouncing procedure, sending the current number of missed
 * deadline
 */
void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(void) {
	IFS1bits.T4IF = 0; // clear the TIMER4 flag
	IEC1bits.T4IE = 0; // disable TIMER4 interrupt
	T4CONbits.TON = 0; // turn off TIMER4

	char message[16];							 // Safe memory allocation size
	sprintf(message, "D=%d\n", missed_deadline); // Formats text string automatically with '\0'
	uart1_write_string(message);
}