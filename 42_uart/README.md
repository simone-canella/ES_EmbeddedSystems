# Exercise 4(task-2): Advanced Cyclic Executive Real-Time Command Parser and Telemetry System

## Overview
This project establishes a high-performance, predictable Real-Time Embedded Architecture on the dsPIC33 microcontroller platform. Utilizing a strict 100 Hz ($10\text{ ms}$) cyclic executive master loop managed by `Timer 1`, the system executes a simulated deterministic processing workload while concurrently executing non-blocking serial character string parsing and dual-channel asynchronous edge-triggered push-button telemetry pipelines.

## Deterministic Real-Time Clock Budgeting
The main cyclic loop enforces strict temporal deterministic bounds to ensure zero execution deadline omissions under high-stress processing profiles:
* **Loop Period:** Exactly $10\text{ ms}$ ($100\text{ Hz}$ frequency), tracked via `Timer 1` period-match events.
* **Algorithmic Workload:** Preamble calls to `algorithm()` utilize `Timer 2` to burn exactly $7\text{ ms}$ of calculation runtime at the absolute beginning of every loop iteration.
* **Processing Headroom:** This structure leaves a precise **$3\text{ ms}$ execution window** per cycle. Within this headroom, the CPU extracts streaming data from software ring buffers, manages LED states, evaluates command strings, and updates clock parameters before waiting for the next periodic boundary via `tmr_wait_period`.

## Asynchronous Multi-Channel Interrupt Matrix
To isolate time-consuming mechanical contact bounce delays and high-frequency communication updates from the critical $10\text{ ms}$ main execution loop, all asynchronous inputs are offloaded into dedicated hardware peripheral interrupt vectors:

### 1. Serial Transceiver Pipeline (UART1)
* **Configuration:** 9600 bps, 8-bit data, no parity, 1 Stop bit.
* **Reception Engine:** Managed via `_U1RXInterrupt`. Incoming data characters are continuously extracted from the hardware FIFO and pushed into a RAM-based software circular buffer. If the software queue is full, a dummy read flushes the hardware FIFO to prevent an infinite ISR loop trap.
* **Non-Blocking Sliding Window Parser:** Inside the loop headroom, the foreground thread drains characters from the RX ring buffer. A 3-character local FIFO string array shifts incoming bytes sequentially to match strings without blocking the main loop context.

### 2. Push-Button Telemetry Matrix
* **Button 2 (RE8 / RPI88):** Hardwired to the remappable **`INT1`** external interrupt pin. A falling-edge press event vectors to `_INT1Interrupt`, which starts a single-shot 10ms hardware debounce tracking clock via `Timer 3`. Upon the `Timer 3` elapsed match (`_T3Interrupt`), the system safely formats and transmits the total character count payload (`"C=xx\n"`) out through the serial line.
* **Button 3 (RE9 / RPI89):** Hardwired to the remappable **`INT2`** external interrupt pin. A falling-edge press event vectors to `_INT2Interrupt`, kickstarting a single-shot 10ms hardware debounce tracking clock via `Timer 4`. Upon the `Timer 4` elapsed match (`_T4Interrupt`), the system safely formats and transmits the total deadline miss payload (`"D=yy\n"`) through the serial line.

## Core Functional Behaviors & Protocol Spec
* **Default Power-On Cadence:** At system initialization, LED2 (`RG9`) begins blinking automatically at a frequency of **2.5 Hz** (alternating states exactly every 20 cycles / $200\text{ ms}$).
* **Terminal Command Strings:**
  * Sending the string `"LD1"` toggles the current state of LED1 (`RA0`).
  * Sending the string `"LD2"` suspends or resumes the active 2.5 Hz blinking cadence on LED2 (`RG9`).
* **Telemetry Data String Packets:**
  * **Button 2 Press:** Transmits `"C=xx\n"`, where `xx` represents the running total count of characters extracted by the driver loop.
  * **Button 3 Press:** Transmits `"D=yy\n"`, where `yy` represents the accumulated total number of real-time deadline misses captured by `tmr_wait_period`.