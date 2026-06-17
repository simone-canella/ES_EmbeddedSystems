/**
 * @file uart.h
 * @brief Header file for the non-blocking UART driver implementation using circular buffers.
 */

#ifndef UART_H
#define UART_H

    #include "config.h"

    // #define FCY 72000000UL
    // #define BAUDRATE 9600 // 115200
    // #define BRGVAL ((FCY/BAUDRATE)/16)-1 // only for slow speed mode

    /** * @brief Size allocation for the UART RX software ring buffer.
     * Length of single correct command: 6 bytes. Only two commands possible: 12 bytes.
     * Rounding up in case of syntax errors: 16 bytes. Chosen length of the buffer: 256.
     */
    #define UART_RX_BUF_SIZE 256
    
    /** * @brief Size allocation for the UART TX software ring buffer.
     * Length of the message in worst case scenario: 59 bytes.
     * Rounded up for little of flexibility with the protocol: 64 bytes. Chosen length of the buffer: 256.
     */
    #define UART_TX_BUF_SIZE 256

    /** @brief Error code returned when the TX circular buffer is completely full. */
    #define TX_BUFFER_FULL -1
    
    /** @brief Error code returned when attempting to read an empty RX circular buffer. */
    #define RX_BUFFER_EMPTY -1

    /**
     * @brief Configures UART1 hardware pins, registers, and communication buffers.
     * @return int Always returns 0 upon successful setup.
     */
    int uart1_mikrobus2_setup();

    /**
     * @brief Non-blocking function to queue a string for UART transmission.
     * @param msg Null-terminated string to transmit.
     * @return int Number of bytes successfully queued, or TX_BUFFER_FULL if space is exhausted.
     */
    int uart1_write(const char *msg);

    /**
     * @brief Non-blocking function to fetch a single character from the RX buffer.
     * @return int The received character, or RX_BUFFER_EMPTY if no data is currently available.
     */
    int uart1_read_char(void);

    /**
     * @brief Calculates the number of bytes currently waiting in the TX circular buffer.
     * @return int Number of pending bytes queued for transmission.
     */
    int uart1_get_tx_count(void);

    /**
     * @brief Calculates the number of unread bytes currently residing in the RX circular buffer.
     * @return int Number of available bytes waiting to be read.
     */
    int uart1_get_rx_count(void);

    // int uart1_read_string(char *dest_buffer, int max_len);

    /* * Doxygen workaround trick: Hide the __attribute__ macro from the generator 
     * to prevent naming collisions between the two interrupt functions. 
     */
    #ifdef __DOXYGEN__
        #define INTERRUPT_ATTR
    #else
        #define INTERRUPT_ATTR __attribute__((__interrupt__))
    #endif

    /**
     * @brief Hardware interrupt service routine for UART1 transmission.
     */
    void INTERRUPT_ATTR _U1TXInterrupt(void);

    /**
     * @brief Hardware interrupt service routine for UART1 reception.
     */
    void INTERRUPT_ATTR _U1RXInterrupt(void);

#endif // UART_H