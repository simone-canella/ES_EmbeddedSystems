#include "uart.h"

struct circular_buffer { // circular buffer
    volatile char *data;
    volatile int idxH;  // The index we WRITE to
    volatile int idxT;  // The index we READ from
    volatile int size;
};

volatile struct circular_buffer U1RX_buffer;
volatile struct circular_buffer U1TX_buffer;

volatile char uart_rx_data_buff[UART_RX_BUF_SIZE];
volatile char uart_tx_data_buff[UART_TX_BUF_SIZE];

int uart1_mikrobus2_setup(){

    RPINR18bits.U1RXR = 0b1001011;  // MikroBus2 UART input map
    RPOR0bits.RP64R = 0b000001;     // MikroBus2 UART output map

    U1STA = 0x00;   // reset control and status register
    U1MODE = 0x00;  // reset mode register
    U1BRG = BRGVAL; // Baud

    U1MODEbits.UARTEN = 1;  // First UARTx Enable bit...
    U1STAbits.UTXEN   = 1;  // ...then UARTx Transmit Enable bit

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

int uart1_write(const char *msg) {
    int i = 0;
    
    while (msg[i] != '\0') {
        IEC0bits.U1TXIE = 0; // DISABLE TX interrupt to protect shared variables
        
        int next_idxH = (U1TX_buffer.idxH + 1) % U1TX_buffer.size;
        
        if (next_idxH == U1TX_buffer.idxT) {
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
    if (!U1STAbits.UTXBF) {
        IFS0bits.U1TXIF = 1; // Force interrupt to fetch the first byte
    }

    IEC0bits.U1TXIE = 1; 

    return i;
}

void __attribute__((__interrupt__)) _U1TXInterrupt(void){
    // AFTER A BYTE IS COPIED IN THE TX BUFFER, at least one location is empty in the transmit buffer.
    IFS0bits.U1TXIF = 0;

    U1TXREG = U1TX_buffer.data[U1TX_buffer.idxT];
    U1TX_buffer.idxT = (U1TX_buffer.idxT+1) % U1TX_buffer.size;

    if(U1TX_buffer.idxT == U1TX_buffer.idxH) IEC0bits.U1TXIE = 0;
}

void __attribute__((__interrupt__)) _U1RXInterrupt(void){
    IFS0bits.U1RXIF = 0; // clear interrupt flag

    if(U1STAbits.OERR == 1) U1STAbits.OERR = 0; // in case of error, discard everything (data loss)

    while(U1STAbits.URXDA){ // as long as there are data to read
        unsigned char received_byte = U1RXREG; 
        int next_idxH = (U1RX_buffer.idxH + 1) % U1RX_buffer.size;
        
        if(next_idxH != U1RX_buffer.idxT){ // if there is space into the buffer to store the incoming data
            U1RX_buffer.data[U1RX_buffer.idxH] = received_byte;
            U1RX_buffer.idxH = next_idxH;
        } else {
            // buffer is full, data discarded
            // different strategies can be implemented here
        }
    }
}

int uart1_read_char(void) {
    if (U1RX_buffer.idxT == U1RX_buffer.idxH) {
        return RX_BUFFER_EMPTY; 
    }

    unsigned char data = U1RX_buffer.data[U1RX_buffer.idxT];
    U1RX_buffer.idxT = (U1RX_buffer.idxT + 1) % U1RX_buffer.size;
    return data;
}