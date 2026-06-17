/**
 * @file main.c
 * @author simon
 * @date 2026-06-14
 * @brief Real-time deadline monitoring and long-duration delay test.
 * 
 * This program demonstrates the use of a heartbeat timer (Timer 1) to monitor
 * execution blocks. It uses Timer 2 to simulate a workload. If the workload
 * exceeds the 200ms heartbeat period, LED2 illuminates as an error signal.
 */

#include "../library/timer.c"
#include "../library/timer.h"
#include <xc.h>

/**
 * @brief Main execution entry point.
 * 
 * Performs peripheral initialization and enters a cyclic executive loop.
 * The loop monitors if the "artificial workload" satisfies the real-time 
 * constraints of the system.
 */
int main() {
	// PIN SETUP: Configure all analog/digital pins to digital mode
	ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

	// Configure LED1 (RA0) and LED2 (RG9) as outputs (0 = output)
	TRISAbits.TRISA0 = 0; // Heartbeat LED1 (RA0)
	TRISGbits.TRISG9 = 0; // Error LED2 (RG9)

	// Initialize LED states (OFF)
	LATAbits.LATA0 = 0;
	LATGbits.LATG9 = 0;

	/**
     * SETUP: Configure TIMER1 for a 200ms periodic heartbeat.
     * This establishes our real-time execution "budget."
     */
	tmr_setup_period(TIMER1, 200);

	/**
     * TEST PARAMETER: Artificial workload duration in milliseconds.
     * - If set to 50: LED2 stays OFF (Deadline met).
     * - If set to 250: LED2 turns ON (Deadline missed!).
     * - If set to 1000: Tests the chunking logic for values > 233ms.
     */
	int workload_ms = 250;

	while (1) {
		/**
         * TASK 1: Simulate application workload using TIMER2.
         * Our updated driver handles values > 233ms using chunking.
         */
		tmr_wait_ms(TIMER2, workload_ms);

		// Toggle Heartbeat LED to show the loop is still running
		LATAbits.LATA0 = !LATAbits.LATA0;

		/**
         * TASK 2: Check for deadline violations.
         * tmr_wait_period returns 1 if Timer 1 expired before we arrived here.
         */
		if (tmr_wait_period(TIMER1) == 1) {
			LATGbits.LATG9 = 1; // Turn ON Error LED (Deadline Missed)
		} else {
			LATGbits.LATG9 = 0; // Keep Error LED OFF (Deadline Met)
		}
	}

	return 0;
}