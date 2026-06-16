# Exercise 4(task-1): Basic UART Communication and Hardware Character Echo

## Overview
This project establishes a foundational asynchronous serial data communication link using the Universal Asynchronous Receiver-Transmitter (UART1) peripheral on a dsPIC33 microcontroller. The application configures the transceiver hardware to communicate at a standard baud rate of 9600 bps over an expansion socket link. It runs a continuous polling loop that intercepts incoming characters from a host computer terminal and instantly echoes them back to verify physical and logical loopback signal integrity.

## Hardware Configuration
* **Expansion Interface:** MikroBUS Slot 2.
* **Receive Line (RX):** Physical pin `RD11`, routed via Peripheral Pin Select (PPS) to internal functional input lane `RPI75`.
* **Transmit Line (TX):** Physical pin `RD0`, routed via Peripheral Pin Select (PPS) to internal functional output lane `RP64`.
* **Data Frame Parameters:** 8 Data Bits, No Parity checking, 1 Stop Bit (8N1 formatting).
* **Baud Rate Speed:** 9600 bps (calculated using a system clock frequency of $F_{CY} = 72\text{ MHz}$).

## Peripheral Pin Select & Control Register Map
1. **`TRISDbits.TRISD11 = 1` & `TRISDbits.TRISD0 = 0`**: Establishes correct data direction tracking, forcing the RX pin to operate as an input buffer and the TX pin to drive output voltages.
2. **`RPINR18bits.U1RXR = 75`**: Binds the UART1 internal receiver data bus to look exclusively at remappable pin index 75 (`RD11`).
3. **`RPOR0bits.RP64R = 1`**: Binds remappable output pin index 64 (`RD0`) to project the hardcoded function code `1`, which represents the active UART1 Transmit data stream.
4. **`U1BRG = 468`**: Programs the 16-bit internal Baud Rate Generator clock divider circuit to establish precise serialization timing slots.

## Operational Control Flow
The firmware utilizes strict hardware power-on sequencing by enabling the master clock module (`UARTEN = 1`) prior to activating the line drivers (`UTXEN = 1`). 

During runtime execution, characters are managed via a two-stage polling mechanism:
* **Receiver Supervision:** The code continually monitors the Receive Data Available flag (`URXDA`). If a buffer overrun error occurs (`OERR = 1`), it automatically clears the error state to prevent a hardware freeze condition.
* **Transmitter Protection:** Before writing an ASCII byte to `U1TXREG`, the application checks the Transmit Buffer Full status flag (`UTXBF`) to guarantee queued data is never overwritten or corrupted.