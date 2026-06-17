#include "adc.h"

/** 
 * @brief Initialize the ADC hardware and configures target analog pins
 * @param sample_mode Selects sampling start (0 = manual, 1 = auto)
 * @param conversion_mode Selects conversion trigger (0 = manual, 1 = auto)
 */
void adc_setup(int sample_mode, int conversion_mode) {
	AD1CON1bits.ADON = 0; // turn-off the ADC module during the configuration

	// PIN SETUP: configure pins as analog
	ANSELBbits.ANSB11 = 1; // Battery (RB11)
	ANSELBbits.ANSB15 = 1; // IR-sensor (RB15)

	// Configure pins direction (0 = output, 1 = input)
	TRISBbits.TRISB11 = 1; // Battery (RB11)
	TRISBbits.TRISB15 = 1; // IR-sensor (RB15)

	// Configure output format (0 = 10-bit, 1 = 12-bit)
	AD1CON1bits.AD12B = 0; // 10-bit

	// Configure the tracking mode matrix based on parameter inputs
	if (sample_mode == 1 && conversion_mode == 1) {
		// AUTOMATED SCAN CONFIGURATION (Exercise 3)
		AD1CON1bits.SSRC = 7;  // Internal counter handles conversion timing
		AD1CON1bits.ASAM = 1;  // Free-running automatic loop sampling start
		AD1CON2bits.CSCNA = 1; // Enable automated channel scan mode
		AD1CON2bits.SMPI = 1;  // Toggle DONE flag after 2 channel conversions (2 - 1 = 1)
		AD1CON3bits.SAMC = 16; // Keep sampling window open for 16 TAD clock ticks

		// Define active target pins inside the low scan selection register
		AD1CSSL = 0x0000;	   // Reset the register mask
		AD1CSSLbits.CSS11 = 1; // Add Battery Monitor (AN11) to scan list
		AD1CSSLbits.CSS15 = 1; // Add Front IR Sensor (AN15) to scan list
	} else if (sample_mode == 0 && conversion_mode == 0) {
		AD1CON1bits.SSRC = 0;  // Manual conversion trigger
		AD1CON1bits.ASAM = 0;  // Manual sampling start
		AD1CON2bits.CSCNA = 0; // Disable channel scan routing
	} else if (sample_mode == 0 && conversion_mode == 1) {
		AD1CON1bits.SSRC = 7;  // Internal counter handles conversion timing
		AD1CON3bits.SAMC = 16; // Keep sampling window open for 16 TAD clock ticks
		AD1CON1bits.ASAM = 0;  // Manual sampling start
		AD1CON2bits.CSCNA = 0; // Disable channel scan routing
	} else {
		AD1CON1bits.SSRC = 0;  // Manual conversion trigger
		AD1CON1bits.ASAM = 1;  // Free-running automatic loop sampling start
		AD1CON2bits.CSCNA = 0; // Disable channel scan routing
	}

	// Configure ADC clock-period
	AD1CON3bits.ADCS = 8; // TAD = Tcy * (ADCS + 1) / 2

	// Start the ADC module
	AD1CON1bits.ADON = 1;
}

/**
 * @brief Reading the sensor value with manual sampling and manual conversion
 * @return unsigned int converted sensor-value
*/
unsigned int adc_read_manual_manual(uint8_t channel) {
	// tell to ADC which pin to look at
	AD1CHS0bits.CH0SA = channel;

	AD1CON1bits.DONE = 0; // Clear the flag to resolve the software race hazard
	AD1CON1bits.SAMP = 1; // start sampling

	// Wait for the sample-and-hold capacitor to charge completely
	for (volatile int i = 0; i < 1000; i++) {
		;
	}

	AD1CON1bits.SAMP = 0; // stop sampling

	while (!AD1CON1bits.DONE) {
		; // wait until fully conversion
	}

	return ADC1BUF0; // return raw value in the internal buffer
}

/**
 * @brief Reading the sensor value with manual sampling and auto conversion
 * @return unsigned int converted sensor-value
*/
unsigned int adc_read_manual_auto(uint8_t channel) {
	// tell to ADC which pin to look at
	AD1CHS0bits.CH0SA = channel;

	AD1CON1bits.DONE = 0; // Clear the flag to resolve the software race hazard
	AD1CON1bits.SAMP = 1; // Start the sampling window

	while (!AD1CON1bits.DONE) {
		; // wait until fully conversion
	}

	return ADC1BUF0; // return raw value in the internal buffer
}

/**
 * @brief Reading the sensor value with auto sampling and auto conversion
 * @param channel physical pin to look at (11 = battery, 15 = ir-sensor)
 * @return unsigned int converted sensor-value
*/
unsigned int adc_read_auto_auto(uint8_t channel) {
	if (AD1CON2bits.CSCNA == 1) {
		// If Scan Mode is enabled, extract data from fixed hardware buffer slots
		if (channel == 11) {
			return ADC1BUF0; // AN11 is stored in BUF0
		} else {
			return ADC1BUF1; // AN15 is stored in BUF1
		}
	} else {
		// tell to ADC which pin to look at
		AD1CHS0bits.CH0SA = channel;

		while (!AD1CON1bits.DONE) {
			; // wait until fully conversion
		}

		return ADC1BUF0; // return converted value in the internal buffer
	}
}

/**
 * @brief Compute the conversion from raw data to human-readable one
 * @param adc_ptr pointer to the AdcState structure which contains the human-readable converted data
 */
void adc_converter(AdcState *adc_ptr, int sample_mode, int conversion_mode) {
	unsigned int raw_battery = 0;
	unsigned int raw_ir = 0;

	// Route data collection requests based on the active hardware configuration
	if (sample_mode == 1 && conversion_mode == 1) {
		// Wait until the background hardware scan finishes processing both channels
		while (AD1CON1bits.DONE == 0)
			;
		raw_battery = ADC1BUF0; // Extract AN11 from buffer 0
		raw_ir = ADC1BUF1;		// Extract AN15 from buffer 1
		AD1CON1bits.DONE = 0;	// Clear flag to acknowledge read and clear the slot
	} else if (sample_mode == 0 && conversion_mode == 0) {
		raw_battery = adc_read_manual_manual(11);
		raw_ir = adc_read_manual_manual(15);
	} else if (sample_mode == 0 && conversion_mode == 1) {
		raw_battery = adc_read_manual_auto(11);
		raw_ir = adc_read_manual_auto(15);
	} else {
		raw_battery = adc_read_auto_auto(11);
		raw_ir = adc_read_auto_auto(15);
	}

	// Process raw battery metrics using the correct 10-bit full-scale denominator
	float v_battery = ((float)raw_battery / 1023.0f) * 3.3f;
	adc_ptr->battery_voltage = v_battery * 3.0f;

	// Normalize raw IR distance counts using the correct 10-bit full-scale denominator
	float ir_voltage = ((float)raw_ir / 1023.0f) * 3.3f;

	// Apply the 4th-order polynomial equation to linearize the distance measurement
	adc_ptr->ir_distance = 100* (2.34f - (4.74f * ir_voltage) + (4.06f * ir_voltage * ir_voltage) -
						   (1.60f * ir_voltage * ir_voltage * ir_voltage) +
						   (0.24f * ir_voltage * ir_voltage * ir_voltage * ir_voltage));
}