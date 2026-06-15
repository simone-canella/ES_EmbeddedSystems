# Exercise 3 (task-1): Dual-Frequency Blinking via Polling and Timer Interrupts

## Overview
This project demonstrates the execution of independent, multi-frequency tasks on a single-core microcontroller by separating foreground polling routines from background hardware interrupts. 

The application maintains a stable **1 Hz visual heartbeat** on LED1 via synchronous main-loop polling while concurrently executing a precise **2 Hz toggle pattern** on LED2 driven asynchronously by a hardware timer interrupt service routine (ISR).

## Hardware Configuration
| Component | Pin Mapping | Execution Track | Timing Interval | Target Frequency |
| :--- | :--- | :--- | :--- | :--- |
| **LED1 (LD1)** | `RA0` | Foreground Polling | 500 ms Toggle | 1 Hz Baseline |
| **LED2 (LD2)** | `RG9` | Background Interrupt | 250 ms Toggle | 2 Hz Baseline |

## Architectural Resource Separation
To avoid hardware resource conflicts such as flag-fighting or timer-ceiling corruption, the tasks are isolated onto entirely separate hardware tracks:
* **Timer 1 (Foreground Focus):** Configured for a 500 ms match period. The main `while(1)` execution block continuously polls this timer to toggle LED1 synchronously.
* **Timer 2 (Background Focus):** Configured for a 250 ms match period. It runs completely independently in the background to handle asynchronous execution for LED2.

## Interrupt Controller Configuration
The Timer 2 background interrupt behavior is managed using three specific registers:
1. **`IPC1bits.T2IP = 1`**: Sets the interrupt priority level for Timer 2. This ensures the CPU acknowledges the request.
2. **`IFS0bits.T2IF = 0`**: Clears any residual hardware flag events before enabling the module to prevent an immediate accidental trigger during boot up.
3. **`IEC0bits.T2IE = 1`**: Unlocks the local interrupt enable gate, allowing the hardware match event to force a CPU context switch.

## Interrupt Service Routine (ISR) Implementation
The Timer 2 vector lane is handled using the explicit `_T2Interrupt` macro signature. Upon matching its 250 ms target, the hardware halts main line execution, saves the Program Counter (PC) and Status Register (SR) onto the stack, and enters the ISR. 

The ISR executes two vital instructions:
* **Manual Flag Reset:** `IFS0bits.T2IF = 0` is cleared immediately in software to allow the chip to monitor the next timing cycle.
* **Non-Blocking Action:** Toggles `LATGbits.LATG9` instantly before exiting the context switch to return control to the main loop.