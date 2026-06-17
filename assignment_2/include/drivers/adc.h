/**
 * @file adc.h
 * @brief Header file for the Analog-to-Digital Converter driver.
 */

#ifndef ADC_H
#define ADC_H

#include "config.h"

/**
 * @struct adcState
 * @brief Container for the processed telemetry metrics.
 */
typedef struct{
    float battery_voltage; /**< Scaled battery voltage reading. */
    float ir_distance;     /**< Linearized distance from the IR sensor in cm. */
} adcState;

/**
 * @brief Initializes the ADC hardware and configures target analog pins.
 * @param channel The specific channel (11 or 15) to lock the multiplexer onto.
 */
void adc_init(uint8_t channel);

/**
 * @brief Executes a sample request with automatic hardware conversion.
 * @param channel The specific channel (11 or 15) to lock the multiplexer onto.
 * @return float The raw ADC buffer value.
 */
double adc_scan_auto(uint8_t channel);

/**
 * @brief Core processing task to sample sensors and compute physical telemetry metrics.
 * @param adc Pointer to the global state structure to store final converted metrics.
 */
void adc_converter(adcState *adc);

#endif // ADC_H