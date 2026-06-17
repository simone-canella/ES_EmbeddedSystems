# Exercise 7: Output Compare PWM Motor Control Infrastructure

## Overview
This subsystem implements a high-frequency, hardware-synchronized differential drive controller for the robotic buggy chassis using the dsPIC33 Output Compare (OC) peripheral. The module operates entirely without consuming primary hardware timer resources by utilizing an autonomous self-synchronization clock engine. It generates edge-aligned PWM signals to drive left and right dual H-bridge motor axes synchronously at a carrier frequency of 10 kHz, managing directional switches and real-time kinematic mixing through an interrupt-driven software-debounced button interface.

## Hardware Configuration & Signal Mapping
* **Clock Source & Base Frequency:** The Output Compare modules are locked strictly to the system instruction clock ($F_{CY} = 72\text{ MHz}$) by configuring `OCTSEL = 7`. 
* **Self-Synchronization:** Enforced by setting `SYNCSEL = 0x1F`. The secondary register `OCxRS` functions as the period counter window, locked to a calculated value of `7199` counts to guarantee a precise 10 kHz carrier frequency.
* **Actuation Resolution:** The primary match register `OCxR` establishes an edge-aligned pulse width duration ($0\%$ to $100\%$ duty cycle mapping to $0$ to $7199$ counts).
* **Remappable Pin Mapping (PPS):**
  * **`OC1` $\rightarrow$ `RD1` (`RP65`):** Left-side Backward Drive Control (`PWM-B`)
  * **`OC2` $\rightarrow$ `RD2` (`RP66`):** Left-side Forward Drive Control (`PWM-A`)
  * **`OC3` $\rightarrow$ `RD3` (`RP67`):** Right-side Backward Drive Control (`PWM-D`)
  * **`OC4` $\rightarrow$ `RD4` (`RP68`):** Right-side Forward Drive Control (`PWM-C`)

## H-Bridge Actuation Logic Matrix
The motor control outputs route through onboard DRV8833 dual drivers matching the specified direction patterns:

| Operational State | Left Wheels (`OC1` / `OC2`) | Right Wheels (`OC3` / `OC4`) |
| :--- | :--- | :--- |
| **Forward Motion** | `OC1 = 0`, `OC2 = PWM` | `OC3 = 0`, `OC4 = PWM` |
| **Backward Motion** | `OC1 = PWM`, `OC2 = 0` | `OC3 = PWM`, `OC4 = 0` |
| **Full Brake / Stop** | `OC1 = 0`, `OC2 = 0` | `OC3 = 0`, `OC4 = 0` |

## Control Loop & Interrupt Debouncing Architecture
1. **Master Control Loop:** Runs deterministically at 10 Hz (100 ms intervals managed by `TIMER1`) to dispatch kinematic demands and prevent H-bridge saturation.
2. **Button Interruption Matrix:** Physical button `RE8` connects via remappable input `RPI88` to External Interrupt 1 (`INT1`). It is configured as an active-low input with falling-edge detection (`INT1EP = 1`).
3. **Trailing-Edge Software Debouncing:** To completely isolate mechanical contact bouncing without blocking loops:
   * A button press triggers `_INT1Interrupt`, which clears the flag and spins up a high-priority 10 ms hardware timeout via `TIMER3`.
   * Subsequent contact bounces continuously re-arm the 10 ms timing floor, locking out transient noise.
   * Once contact transitions settle completely, `_T3Interrupt` safely toggles the `motor_active` control flag, smoothly enabling or stopping vehicle motion.