# Exercise 20: Hardware Timers and Precise Clock Polling

## Overview
This project implements a precise, hardware-timed execution delay using the onboard **Timer 1** (Type A) peripheral of the dsPIC33EP512MU810 microcontroller on the Clicker 2 development board. The driver dynamically calculates internal execution parameters on the fly to establish a highly accurate **2.5 Hz blinking heartbeat frequency** (200 ms ON, 200 ms OFF) on LED1.

## Hardware & Clock Configuration
* **LED1 Mapping:** `RA0` configured as a Digital Output.
* **Primary Oscillator ($F_{in}$):** 8 MHz external crystal.
* **System Clock ($F_{osc}$):** 144 MHz, pre-scaled and multiplied through the onboard bootloader PLL network.
* **Instruction Cycle Clock ($F_{cy}$):** Locked at 72 MHz ($F_{cy} = \frac{F_{osc}}{2}$), which means the hardware timer increments its internal counter exactly 72,000,000 times per second.

## Driver Math & Prescaler Selection Strategy
Because the dsPIC33 hardware timers are **16-bit registers**, the maximum integer value they can hold before resetting is 65,535 ticks. At a raw instruction frequency of 72 MHz, the timer would overflow its limits in approximately 0.91 milliseconds.

To support a 200 ms period safely without a memory overflow or a fatal divide-by-zero error, the driver script executes this automatic setup logic:
1. **Tick Range Evaluation:** It computes total raw ticks using long long integers to prevent truncation errors.
2. **Dynamic Scaling Mux:** It evaluates the math against the four available hardware prescaler options (1:1, 1:8, 1:64, 1:256).
3. **Period Matching Calculation:** For a 200 ms target, it selects the 1:256 clock divider network and solves for the target match criteria, driving the period ceiling comfortably beneath the 65,535 16-bit physical threshold.

## Special Function Registers (SFRs) Utilized
* **`T1CON`**: Controls global operational profiles, including `TON` (Timer Enable/Disable State), `TCS` (Clock Source Selector), and `TCKPS` (Prescaler Bit Configuration Mux).
* **`TMR1`**: Reset explicitly to `0` during configuration loops to clear out any leftover historical counts.
* **`PR1`**: Loaded with the finalized target division count to serve as the hardware match ceiling.
* **`IFS0bits.T1IF`**: The dedicated interrupt event flag bit. The software performs precise polling on this bit and forces a manual software overwrite to `0` upon match expiration to prepare for the next clock sequence.

## Compilation & Workspace Library Architecture
To encourage robust code reuse, the timer drivers are kept inside a centralized, independent `library/` folder located outside the specific project directory. To bypass conflicting configuration path parameters imposed by automated IDE extension tools, compilation is achieved using direct translation-unit inclusion (`#include "../library/timer.c"`). This links the source drivers seamlessly into a single workspace module, ensuring flawless assembly execution every single time.