# Project: One-Shot Blocking Delays and Asymmetric Blink

## Overview
This project implements an on-demand, one-shot blocking delay driver utilizing the hardware timers of the dsPIC33EP512MU810 microcontroller. Unlike a continuous heartbeat timer, the `tmr_wait_ms` function configures a timer dynamically for a requested duration, executes a busy-wait loop until the time elapses, and then completely deactivates the peripheral to prevent it from consuming clock cycles in the background.

## Hardware Configuration
* **LED1 (LD1):** Mapped to pin `RA0` as a digital output.
* **Blink Profile:** Programmed for an asymmetric cycle consisting of a 20 ms active-high state followed by a 200 ms inactive-low state.

## Implementation Details
* **Dynamic Prescaler Selection:** The configuration logic evaluates the target millisecond request against the 72 MHz instruction clock baseline (`F_CY`). It automatically chooses the most accurate hardware prescaler configuration bits (`0b00` for 1:1 up to `0b11` for 1:256) to ensure the final target value safely fits within the 16-bit hardware register capacity limit of 65,535 ticks.
* **Mandatory State Safeguards:** Before modifying any control or period registers, the timer module is explicitly frozen (`TON = 0`) to avoid race conditions. The accumulator counter (`TMRx`) is cleared to zero at the start of every function call to wipe out historical counts and guarantee exact accuracy.
* **Resource Preservation:** The moment the hardware match flag is asserted, the polling loop breaks, the interrupt flag is manually pulled back to zero, and the timer is immediately disabled (`TON = 0`).

## File Structure
To keep the main repository clean and promote modular architecture, the core configuration and wait routines are organized inside an independent `library` directory outside of the main workspace project folder. The code links these drivers by including the source file directly into the main execution unit.