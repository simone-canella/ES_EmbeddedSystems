# Exercise 22: Advanced Timing and Real-Time Deadlines

## Overview
This project finalizes the hardware timer library by implementing drivers capable of handling long delays and detecting deadline violations in real-time execution loops.

## Key Features
* **Extended Delay Support:** The `tmr_wait_ms` function uses a chunking algorithm to process delays. By dividing the requested time into 200 ms segments, the driver prevents 16-bit register overflow in the Period Register (PRx) while ensuring accuracy for durations exceeding hardware limits.
* **Deadline Miss Detection:** The `tmr_wait_period` function captures the state of the interrupt flag (TxIF) immediately upon entry. If the flag is already 1, the function reports that the previous computational task exceeded its allotted period.
* **Status Feedback:** This mechanism allows the application to monitor real-time performance and trigger error indicators (like LED2) when the execution budget is exhausted.

## Hardware Mapping
* **LED1 (Heartbeat):** Blinks to indicate the main loop is running.
* **LED2 (Error):** Illuminates if `tmr_wait_period` returns a 1, signaling a missed real-time deadline.