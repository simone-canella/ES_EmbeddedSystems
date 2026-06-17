/**
 * @file uart.c
 * @brief Non-blocking UART driver implementation using circular buffers and interrupts.
 *
 * This module provides a fully non-blocking serial communication interface. 
 * By utilizing interrupt service routines (ISRs) and software circular buffers 
 * for both transmission and reception, the main execution thread is never 
 * halted waiting for hardware shift registers to clear.
 */

#include "uart.h"
#include "config.h"
#include "timer.h"

/**
 * @struct circular_buffer
 * @brief Internal circular buffer structure for non-blocking data queuing.
 */
struct circular_buffer
{ // circular buffer
    volatile char *data; /**< Pointer to the data array */
    volatile int idxH; /**< The index we WRITE to */
    volatile int idxT; /**< The index we READ from */
    volatile int size; /**< Total capacity of the buffer */
};

/**
 * @brief Global instances of the TX and RX circular buffers.
 */
volatile struct circular_buffer U1RX_buffer;
volatile struct circular_buffer U1TX_buffer;

/**
 * @brief Underlying memory arrays allocated for the circular buffers.
 */
volatile char uart_rx_data_buff[UART_RX_BUF_SIZE];
volatile char uart_tx_data_buff[UART_TX_BUF_SIZE];

/**
 * @brief Configures UART1 hardware pins, registers, and communication buffers.
 *
 * Sets up the MikroBus2 RX/TX pins, configures the baud rate with High Precision (BRGH=1), 
 * and initializes the circular buffer pointers. Rx interrupts are enabled 
 * by default, while TX interrupts are kept disabled until data is actively queued.
 * @return int Always returns 0 upon successful setup.
 */
int uart1_mikrobus2_setup()
{

    RPINR18bits.U1RXR = 0b1001011; // MikroBus2 UART input map
    RPOR0bits.RP64R = 0b000001;    // MikroBus2 UART output map

    U1STA = 0x00;  // reset control and status register
    U1MODE = 0x00; // reset mode register

    // --- HC05 Bluetooth ---
    // 98:D3:34:90:A8:40
    U1MODEbits.PDSEL = 0b00;
    //DUPLICATED VERSION U1MODEbits.STSEL = 0b1;

    U1MODEbits.STSEL = 0; // 0 = 1 stop bit (Compatible with Arduino IDE / PC)

    // MODIFICATO: 1 = Alta precisione (compatibile con la nuova formula in uart.h)
    U1MODEbits.BRGH = 1; // 1 = High Precision (Uses /4 formula in config.h)

    // ASSICURATI DI ASSEGNARE U1BRG DOPO AVER IMPOSTATO BRGH
    U1BRG = BRGVAL; // Baud rate assignment after BRGH setup

    U1MODEbits.UARTEN = 1; // First UARTx Enable bit...
    U1STAbits.UTXEN = 1;   // ...then UARTx Transmit Enable bit

    // Enable RX interrupt
    IFS0bits.U1RXIF = 0; // clear the flag
    IEC0bits.U1RXIE = 1; // enable the interrupt

    // uart1_write() will enable the interrupt when needed
    IEC0bits.U1TXIE = 0;
    IFS0bits.U1TXIF = 0;

    tmr_wait_ms(TIMER1, 0.1);

    // --- buffer initialization --- //
    U1TX_buffer.data = uart_tx_data_buff;
    U1TX_buffer.size = UART_TX_BUF_SIZE;
    U1TX_buffer.idxH = 0;
    U1TX_buffer.idxT = 0;

    U1RX_buffer.data = uart_rx_data_buff;
    U1RX_buffer.size = UART_RX_BUF_SIZE;
    U1RX_buffer.idxH = 0;
    U1RX_buffer.idxT = 0;

    return 0;
}

/**
 * @brief Non-blocking function to queue a string for UART transmission.
 *
 * Copies the input string into the TX circular buffer without waiting for hardware transmission to complete.
 * Briefly disables the TX interrupt during buffer manipulation to prevent race conditions, 
 * then kickstarts the transmission interrupt if the hardware is idle.
 * @param msg Null-terminated string to transmit.
 * @return int Number of bytes successfully queued, or TX_BUFFER_FULL if space is exhausted.
 */
int uart1_write(const char *msg)
{
    int i = 0;

    while (msg[i] != '\0')
    {
        IEC0bits.U1TXIE = 0; // DISABLE TX interrupt to protect shared variables

        int next_idxH = (U1TX_buffer.idxH + 1) % U1TX_buffer.size;

        if (next_idxH == U1TX_buffer.idxT)
        {
            IEC0bits.U1TXIE = 1; // RE-ENABLE before returning on error
            return TX_BUFFER_FULL;
        }

        U1TX_buffer.data[U1TX_buffer.idxH] = msg[i];
        U1TX_buffer.idxH = next_idxH;

        IEC0bits.U1TXIE = 1; // RE-ENABLE
        i++;
    }

    // Safely kickstart the transmission if it's currently idle
    IEC0bits.U1TXIE = 0;

    // If the hardware buffer is not full, and we aren't currently sending
    if (!U1STAbits.UTXBF)
    {
        IFS0bits.U1TXIF = 1; // Force interrupt to fetch the first byte
    }

    IEC0bits.U1TXIE = 1;

    return i;
}

/**
 * @brief Hardware interrupt service routine for UART1 transmission.
 *
 * Automatically triggered when the UART hardware TX buffer has empty space.
 * It pulls the next available byte from the software circular buffer and pushes it 
 * to the hardware register (U1TXREG). If the software buffer empties completely, 
 * the interrupt safely disables itself to halt further triggers.
 */
void __attribute__((__interrupt__)) _U1TXInterrupt(void)
{
    // AFTER A BYTE IS COPIED IN THE TX BUFFER, at least one location is empty in the transmit buffer.
    IFS0bits.U1TXIF = 0;

    U1TXREG = U1TX_buffer.data[U1TX_buffer.idxT];
    U1TX_buffer.idxT = (U1TX_buffer.idxT + 1) % U1TX_buffer.size;

    if (U1TX_buffer.idxT == U1TX_buffer.idxH)
        IEC0bits.U1TXIE = 0;
}

/**
 * @brief Hardware interrupt service routine for UART1 reception.
 *
 * Triggered asynchronously when incoming bytes arrive. It handles hardware overrun 
 * errors (OERR) by clearing the bit (which discards corrupted data), and safely copies valid incoming 
 * bytes into the RX software circular buffer. If the circular buffer is full, 
 * incoming data is silently dropped to prevent memory corruption.
 */
void __attribute__((__interrupt__)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0; // clear interrupt flag

    if (U1STAbits.OERR == 1)
        U1STAbits.OERR = 0; // in case of error, discard everything (data loss)

    while (U1STAbits.URXDA)
    { // as long as there are data to read
        unsigned char received_byte = U1RXREG;
        int next_idxH = (U1RX_buffer.idxH + 1) % U1RX_buffer.size;

        if (next_idxH != U1RX_buffer.idxT)
        { // if there is space into the buffer to store the incoming data
            U1RX_buffer.data[U1RX_buffer.idxH] = received_byte;
            U1RX_buffer.idxH = next_idxH;
        }
        else
        {
            // buffer is full, data discarded
            // different strategies can be implemented here
        }
    }
}

/**
 * @brief Non-blocking function to fetch a single character from the RX buffer.
 *
 * Checks the software circular buffer for unread data. If data is present, it returns 
 * the byte and advances the read pointer. Because it does not wait for incoming bytes, 
 * it is safe to call inside high-frequency processing loops.
 * @return int The received character, or RX_BUFFER_EMPTY if no data is currently available.
 */
int uart1_read_char(void)
{
    if (U1RX_buffer.idxT == U1RX_buffer.idxH)
    {
        return RX_BUFFER_EMPTY;
    }

    unsigned char data = U1RX_buffer.data[U1RX_buffer.idxT];
    U1RX_buffer.idxT = (U1RX_buffer.idxT + 1) % U1RX_buffer.size;
    return data;
}

/**
 * @brief Calculates the number of bytes currently waiting in the TX circular buffer.
 * @return int Number of pending bytes queued for transmission.
 */
int uart1_get_tx_count(void)
{
    return (U1TX_buffer.idxH - U1TX_buffer.idxT + U1TX_buffer.size) % U1TX_buffer.size;
}

/**
 * @brief Calculates the number of unread bytes currently residing in the RX circular buffer.
 * @return int Number of available bytes waiting to be read.
 */
int uart1_get_rx_count(void)
{
    return (U1RX_buffer.idxH - U1RX_buffer.idxT + U1RX_buffer.size) % U1RX_buffer.size;
}