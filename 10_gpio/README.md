# Exercise 10: Basic Digital I/O Polling

## Overview
This project demonstrates basic digital input and output operations on the Microchip dsPIC33EP512MU810 microcontroller using the Clicker 2 development board. The program polls the real-time state of an external push button and drives an LED directly based on that state.

## Hardware Mapping
| Component | Microcontroller Pin | Configuration | Electrical Behavior |
| :--- | :--- | :--- | :--- |
| **LED1** | `RA0` | Digital Output | Active-High (1 = ON, 0 = OFF) |
| **Button1 (T2)** | `RE8` | Digital Input | Active-Low (1 = Released, 0 = Pressed) |

## Key Features
* **Analog Override:** Explicitly clears all port analog selection registers (`ANSELx = 0x0000`) to guarantee true digital mode.
* **Direction Control:** Configures pin directions safely using the `TRISbits` data structure.
* **Glitch Mitigation:** Implements proper embedded practices by reading input states from the `PORT` register and writing output states via the `LAT` register to prevent Read-Modify-Write (RMW) micro-architectural anomalies.

## Software Logic Execution
The main loop continuously samples the raw hardware logic state of `PORTEbits.RE8`. When the input condition changes, it immediately mirrors an updated execution status directly into the `LATAbits.LATA0` register latch.