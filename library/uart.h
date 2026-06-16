#ifndef UART_H
#define UART_H

#include <xc.h>
#include <stdbool.h>
#include <math.h>

// Function Prototypes
void uart1_mikrobus2_setup(void);
char uart1_read_char(void);
int uart1_write_string(const char *msg);

#endif