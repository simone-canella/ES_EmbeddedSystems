#ifndef ADC_H
#define ADC_H

#include <math.h>
#include <stdbool.h>
#include <xc.h>

/**
 * @struct AdcState
 * @brief Storage container for processed, human-readable sensor metrics.
 */
typedef struct {
    float battery_voltage;
    float ir_distance;
} AdcState;

// Function Prototypes
void adc_setup(int sample_mode, int conversion_mode);
unsigned int adc_read_manual_manual(uint8_t channel);
unsigned int adc_read_manual_auto(uint8_t channel);
unsigned int adc_read_auto_auto(uint8_t channel);

void adc_converter(AdcState *adc_ptr, int sample_mode, int conversion_mode);
#endif