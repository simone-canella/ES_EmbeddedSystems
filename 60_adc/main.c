/**
 * @file main.c
 * @author simon
 * @date 2026-06-17
 * @brief Main execution loop: 1 kHz Automated ADC Scan with 10 Hz UART streaming.
*/

#include "../library/adc.c"
#include "../library/adc.h"
#include "../library/spi.c"
#include "../library/spi.h"
#include "../library/timer.c"
#include "../library/timer.h"
#include "../library/uart.c"
#include "../library/uart.h"
#include <stdio.h>
#include <xc.h>

// Lock system configuration parameters to free-running automated Scan Mode
#define SAMPLE_MODE 1	  //0 = manual, 1 = auto
#define CONVERSION_MODE 1 //0 = manual, 1 = auto

#define BATTERY 11
#define IR_SENSOR 15

int main() {

	// SETUP PIN: Configure all pins as digital
	ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

	// TIMER SETUP
	tmr_setup_period(TIMER1, 1);

	// SETUP UART
	uart1_mikrobus2_setup();

	// SETUP ADC
	adc_setup(SAMPLE_MODE, CONVERSION_MODE);

	int missed_deadline = 0;
	int counter = 0;

	AdcState adc;

	// EXECUTION-LOOP at 1KHz (1ms)
	while (1) {
		// Read background hardware scan conversions and update metrics at 1 kHz
		adc_converter(&adc, SAMPLE_MODE, CONVERSION_MODE);

		// Down-sample telemetry transmission to exactly 10 Hz (once every 100 loops)
		if (counter >= 100) {
			// Format the value into an ASCII string and transmit via non-blocking UART
			char message[32];

			sprintf(message, "$SENS,%f,%f*\n", adc.ir_distance, adc.battery_voltage);

			uart1_write_string(message);

			counter = 0;
		}

		if (tmr_wait_period(TIMER1) == 1) {
			missed_deadline++;
		}

		counter++;
	}

	return 0;
}
