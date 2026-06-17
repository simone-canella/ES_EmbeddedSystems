/**
 * @file adc.c
 * @brief Analog-to-Digital Converter (ADC) configuration and sampling routines.
 */

#include "adc.h"

/**
 * @brief Initializes the ADC hardware and configures target analog pins.
 * @param channel The analog channel to initialize.
 */
void adc_init(uint8_t channel) {

    AD1CON1bits.ADON = 0; 

    AD1CON1bits.AD12B = 0b00;   // Output format: 10-bit

    ANSELBbits.ANSB11 = 1;      // Set RB11 as analog input (Battery)
    TRISBbits.TRISB11 = 1;      // Set RB11 as input

    ANSELBbits.ANSB15 = 1;      // Set RB15 as analog input (IR Sensor)
    TRISBbits.TRISB15 = 1;      // Set RB15 as input

    AD1CON1bits.SSRC = 0b111;   // Auto-Convert Mode
    AD1CON1bits.ASAM = 0;       // Manual Sampling Start

    // Maximize the physical charge time using the hardware clock
    AD1CON3bits.ADCS = 8;
    AD1CON3bits.SAMC = 16;

    AD1CON1bits.ADON = 1; 
}

/**
 * @brief Executes a sample request with automatic hardware conversion.
 * @param channel The analog channel to scan.
 * @return double The raw ADC conversion result.
 */
double adc_scan_auto(uint8_t channel){

    // 1. Point the internal multiplexer to the new pin
    AD1CHS0bits.CH0SA = channel;

    // 2. Start the hardware sampling sequence
    AD1CON1bits.DONE = 0;
    
    AD1CON1bits.SAMP = 1; 
    
    // 3. Wait for the hardware conversion to finish
    while(!AD1CON1bits.DONE);
    
    return ADC1BUF0;
}

/**
 * @brief Core processing task to sample sensors and compute physical telemetry metrics.
 * @param adc Pointer to the adcState structure to populate with metrics.
 */
void adc_converter(adcState* adc){
    double raw_battery = adc_scan_auto(11); 
    double raw_ir = adc_scan_auto(15);
    
    float ir_pin_voltage = ((float) raw_ir / 1024.0f) * 3.3f;

    adc->ir_distance = 100 * ( 2.340f - (4.74f * ir_pin_voltage) 
                        + (4.06f * powf(ir_pin_voltage, 2)) 
                        - (1.60f * powf(ir_pin_voltage, 3)) 
                        + (0.24f * powf(ir_pin_voltage, 4)));
    adc->battery_voltage = ( (float) raw_battery / 1024.0f) * 3.3f * 3.0f;
}