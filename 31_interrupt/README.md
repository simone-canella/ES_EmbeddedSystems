# Exercise 3(task-2): Asynchronous External Edge Interrupts with Software Debouncing

## Overview
This project implements an advanced asynchronous input routing system using External Interrupt 1 (INT1) linked to a physical push-button. The application showcases a non-blocking software debouncing design pattern. While LED1 runs a continuous 1 Hz background blinking sequence via standard main loop polling, a press on Button T2 triggers an immediate, asynchronous state change on LED2 after a small filter window.

## Hardware Mapping
* **LED1 (Heartbeat):** Connected to pin `RA0` (Output). Toggles every 500 ms via Timer 1 polling.
* **LED2 (Status Trigger):** Connected to pin `RG9` (Output). Governed by the debouncing interrupt pipeline.
* **Button T2 (Input Trigger):** Connected to pin `RE8` (Input). Leverages internal input lines mapped to peripheral logic.

## Peripheral Pin Select (PPS) Configuration
Because external interrupts are not hardwired to physical ports on the dsPIC33 architecture, input routing is managed through the Peripheral Pin Select matrix:
* Physical pin `RE8` maps internally to remappable input index `88` (RPI88).
* The function mapping register `RPINR0bits.INT1R` is loaded with this value, forcing the External Interrupt 1 module to listen explicitly to state transitions occurring on the T2 push-button line.

## Asynchronous Debouncing Algorithm
Mechanical switches bounce rapidly upon contact closure, generating false multi-trigger pulses. To counter this without introducing forbidden blocking delays into the system, a dual-interrupt pipeline is utilized:
1. **Edge Transition Sensed:** The user depresses Button T2, tripping `_INT1Interrupt`. This routine acknowledges the hit, drops its flag, unlocks the Timer 2 local execution gate, and schedules a precise 10 ms single-shot countdown window.
2. **Filter Window Expiration:** During the 10 ms countdown, secondary mechanical noise edges are absorbed safely. Once Timer 2 completes its run, `_T2Interrupt` executes.
3. **State Updates:** The Timer 2 handler disables its own enable gate, drops its hardware flag, forces the clock module to stop counting, and executes a clean assignment toggle on LED2.