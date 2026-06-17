#include "uart.h"

#define FCY 72000000UL
#define BAUDRATE 9600

#define UART_RX_BUFFER_SIZE 64
#define UART_TX_BUFFER_SIZE 64
#define RX_BUFFER_EMPTY -1
#define TX_BUFFER_FULL -1

// --------------------
// DATA STRUCTURES
// --------------------

/**
 * @struct CircularBuffer
 * @brief Internal circular buffer structure for non-blocking data queuing
 */
typedef struct {
	volatile char *data;	  // pointer to the data array
	volatile int write_index; // index we WRITE to
	volatile int read_index;  // index we READ from
} CircularBuffer;

/**
 * @brief Global variable of the TX and RX circular buffers
 */
volatile struct CircularBuffer U1RX_buffer;
volatile struct CircularBuffer U1TX_buffer;

/**
 * @brief arrays allocated fot the circular buffers
 */
volatile char uart_rx_data_buffer[UART_RX_BUFFER_SIZE];
volatile char uart_tx_data_buffer[UART_TX_BUFFER_SIZE];

// --------------------
// FUNCTIONS
// --------------------

/**
 * @brief Configures UART1 module parameters, I/O cell directionality, and PPS
 * lines.
 */
void uart1_mikrobus2_setup() {
	// Master Data Direction Configuration (1 = Input, 0 = Output)
	TRISDbits.TRISD11 = 1; // Set RD11 (UART1 RX) as digital input
	TRISDbits.TRISD0 = 0;  // Set RD0  (UART1 TX) as digital output

	// PPS Mapping
	RPINR18bits.U1RXR = 75; // Map U1RX function to remappable input RPI75 (RD11)
	RPOR0bits.RP64R = 1;	// Map U1TX function (code 1) to remappable output RP64 (RD0)

	// Configure the baud rate clock divider (9600 at 72MHz)
	U1BRG = (FCY / (16 * BAUDRATE)) - 1;

	// Configure Data Framing Parameters
	U1MODEbits.PDSEL = 0; // 8-bit data, no parity
	U1MODEbits.STSEL = 0; // 1 stop bit

	// Enable RX interrupt
	// IPC2bits.U1RXIP = 1; // set priority
	IFS0bits.U1RXIF = 0; // clear the flag
	IEC0bits.U1RXIE = 1; // enable interrupt

	// Hardware Activation Sequencing
	U1MODEbits.UARTEN = 1; // Enable master UART module clock
	U1STAbits.UTXEN = 1;   // Enable transmission pin line drivers

	// BUFFER initialization
	U1TX_buffer.data = uart_tx_data_buffer;
	U1TX_buffer.read_index = 0;
	U1TX_buffer.write_index = 0;

	U1RX_buffer.data = uart_rx_data_buffer;
	U1RX_buffer.read_index = 0;
	U1RX_buffer.write_index = 0;
}

// --------------------
// READ PROCEDURE
// --------------------

/**
 * @brief NON-blocking function to fetch a single character from the UART1(RX)
 * buffer, handling overruns.
 * @return char The oldest ASCII byte extracted from the FIFO queue, or the
 * RX_BUFFER_EMPTY.
 */
char uart1_read_char() {
	if (U1RX_buffer.read_index == U1RX_buffer.write_index) {
		return RX_BUFFER_EMPTY;
	}

	unsigned char data = U1RX_buffer.data[U1RX_buffer.read_index];
	U1RX_buffer.read_index = (U1RX_buffer.read_index + 1) % UART_RX_BUFFER_SIZE;

	return data;
}

/**
 * @brief Hardware interrupt routine for UART1 reception
 *
 * @details Triggered asynchronously when incoming bytes arrive. It handels
 * OERR, and copies valid incoming bytes into RX circular buffer. If the
 * circular buffer is full, incoming data are dropped
 */
void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void) {
	IFS0bits.U1RXIF = 0; // cleat the flag

	// Trap and resolve buffer overrun errors to prevent peripheral freeze
	// conditions
	if (U1STAbits.OERR == 1) {
		U1STAbits.OERR = 0; // reset receiver if it is blocked
	}

	int next_write_index = 0;

	while (U1STAbits.URXDA) { // data available
		next_write_index = (U1RX_buffer.write_index + 1) % UART_RX_BUFFER_SIZE;

		// control if the buffer has free space
		if (next_write_index != U1RX_buffer.read_index) {
			U1RX_buffer.data[U1RX_buffer.write_index] = U1RXREG; // read data from UART and put in the circular buffer

			U1RX_buffer.write_index = next_write_index;
		} else {
			// buffer is full => dummy read to flush hardware FIFO and prevent infinite loop
			volatile char dummy = U1RXREG;
		}
	}
}

// --------------------
// WRITE PROCEDURE
// --------------------

/**
 * @brief NON-blocking function to queue a string for UART transmission
 * @param msg string to transmit
 * @return int Number of bytes succefully queued, or TX_BUFFER_FULL message if
 * space is full
 *
 * @details copied the input string into the circular buffer without waiting for
 * hardware transmission to complete. Disable the TX interrupt during buffer
 * manipulation to prevent race conditions, then start the transmission
 * interrupt if hardware is idle
 */
int uart1_write_string(const char *msg) {
	int next_write_index = 0;
	int queued_bytes = 0;

	for (int i = 0; msg[i] != '\0'; i++) {
		IEC0bits.U1TXIE = 0; // DISABLE Tx interrupt to protect shared variables

		next_write_index = (U1TX_buffer.write_index + 1) % UART_TX_BUFFER_SIZE;

		// control if the buffer is full
		if (next_write_index == U1TX_buffer.read_index) {
			IEC0bits.U1TXIE = 1; // ENABLE Tx interrupt

			return TX_BUFFER_FULL;
		}

		U1TX_buffer.data[U1TX_buffer.write_index] = msg[i];
		U1TX_buffer.write_index = next_write_index;

		IEC0bits.U1TXIE = 1; // ENABLE Tx interrupt

		queued_bytes++; // update the queued bytes
	}

	// Kick start the background transmission
	IEC0bits.U1TXIE = 0; // DISABLE Tx interrupt to protect shared variables

	// if the buffer is not full, and we aren't currently sending
	if (!U1STAbits.UTXBF) {
		IFS0bits.U1TXIF = 1; // Force interrupt to fetch first byte
	}

	IEC0bits.U1TXIE = 1; // ENABLE Tx interrupt

	return queued_bytes;
}

/**
 * @brief Hardware interrupt routine for UART1 transmission
 *
 * @details Triggered asynchronously when incoming UART Tx buffer has empty
 * space. It pulls the next available byte from the circular buffer and pushes
 * it to the U1TXREG register. If the circular buffer us empty, the interrupt
 * disables itself to avoid other triggers
 */
void __attribute__((__interrupt__, __auto_psv__)) _U1TXInterrupt(void) {
	IFS0bits.U1TXIF = 0; // clear the flag

	U1TXREG = U1TX_buffer.data[U1TX_buffer.read_index]; // send data to the UART1 Tx
	U1TX_buffer.read_index = (U1TX_buffer.read_index + 1) % UART_TX_BUFFER_SIZE;

	if (U1TX_buffer.read_index == U1TX_buffer.write_index) {
		IEC0bits.U1TXIE = 0; // disable interrupt
	}
}