#ifndef UART_H
#define UART_H

#include <math.h>
#include <stdbool.h>
#include <xc.h>

// Function Prototypes
void uart1_mikrobus2_setup(void);
char uart1_read_char(void);
int uart1_write_string(const char *msg);

#endif