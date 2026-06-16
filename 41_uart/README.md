# Exercise 4(task-1): Non-Blocking Interrupt-Driven UART Driver with Circular Software Buffers

## Overview
This subsystem provides an entirely non-blocking asynchronous serial communication interface for the dsPIC33 architecture, routed explicitly through MikroBUS Slot 2. By moving away from basic status flag polling traps, this implementation decouples production from consumption through background Interrupt Service Routines (ISRs) and independent software circular queues allocated in RAM.

## Architectural Map
* **Data Links:** Formatted for 9600 bps baud timing parameters with an 8-bit word width, no parity, and 1 Stop bit configuration.
* **Microcontroller Pins:** The physical RX hardware channel is bound to `RD11` (Input) via remappable line `RPI75`. The physical TX hardware channel is bound to `RD0` (Output) via remappable line `RP64`.

## Thread Safety and Concurrency Countermeasures
To guarantee system stability across multi-threaded asynchronous events, the driver implements three critical data integrity mechanisms:
1. **Memory Synchronization:** Index pointers and underlying data storage matrices are declared with the `volatile` qualifier to prevent compiler register optimization caching and force direct RAM memory fetches during execution.
2. **Foreground Race Protection:** Critical buffer manipulations inside the foreground string queuing function are isolated using local interrupt masks (`IEC0bits.U1TXIE`), blocking background handlers from preempting multi-step pointer calculations.
3. **Hardware Overflow Deflection:** The background reception handler actively resolves hardware saturation bugs. If the software queue is full, the ISR executes a dummy read of `U1RXREG` to flush the internal hardware FIFO, preventing the system from hanging in an infinite loop.