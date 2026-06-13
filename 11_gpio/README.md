# Exercise 11: Non-Blocking Edge Detection & Toggle

## Overview
This project implements an advanced **Non-Blocking State Machine** to toggle the status of an LED exactly once per button click. By tracking state changes over time instead of using blocking delay routines, the processor remains free to run other system tasks continuously at full speed.

## Hardware Mapping
| Component | Microcontroller Pin | Configuration | Electrical Behavior |
| :--- | :--- | :--- | :--- |
| **LED1** | `RA0` | Digital Output | Active-High (1 = ON, 0 = OFF) |
| **Button1 (T2)** | `RE8` | Digital Input | Active-Low (1 = Released, 0 = Pressed) |

## Key Features
* **Rising Edge Detection:** Toggles the LED state specifically upon the *release* of the active-low push button (transition from 0V back to 5V).
* **Non-Blocking Architecture:** Uses zero software delay loops (`delay_ms`) or hardware-trapping `while()` loops, ensuring maximum CPU availability.
* **Software De-glitching Strategy:** Utilizes a state-tracking variable flag (`buttonPressed`) to store state history across separate execution cycles of the main loop, completely eliminating the "machine-gun" double-triggering error caused by rapid micro-execution loops.

## State Machine Execution Flow
1. **Falling Edge Detection:** When the button is pressed (`RE8 == 0`), the software sets the tracking flag (`buttonPressed = 1`).
2. **Rising Edge & Toggle Action:** When the button is subsequently released (`RE8 == 1` while the flag is locked), the LED state is inverted (`LATA0 = !LATA0`), and the tracking flag resets to `0`, arming the state machine for the next click event.