
# Exercise5(task-1): SPI Driver Initialization and Sensor Authentication Pipeline

## Overview
This component implements a synchronous serial architecture using the dsPIC33 SPI1 peripheral to initialize and authenticate an external 9DOF 3 Click sensor board. The application handles multi-stage power transitions and executes a full-duplex hardware shift register exchange to verify physical device connectivity.

## SPI Peripheral Configuration
* **Bus Topography:** Operates in Master Mode with an 8-bit word frame width. 
* **Signal Remapping:** * `MISO1` (Data Input) is bound to `RA1` via remappable line `RPI17`.
  * `MOSI1` (Data Output) is bound to `RF13` via remappable line `RP109`.
  * `SCK1` (Bus Clock) is bound to `RF12` via remappable line `RP108`.
* **Timing and Polarity:** Clock parameters are locked to an Idle-High level ($CKP = 1$). Frequency division is throttled via a 1:1 Primary and 5:1 Secondary scaling network.
* **Chip Selection:** Managed explicitly using digital output line `RD6` (Chip Select 3), keeping all neighboring components high (inactive) on the shared bus line to avoid bus contention.

## Core Operation Protocol Sequence
1. **Power Mitigation:** The magnetometer is awakened from Suspend Mode by issuing a write instruction containing `0x01` to power register `0x4B`.
2. **Temporal Budgeting:** The application halts execution for $3\text{ ms}$ to provide the required stabilization window while the sensor's digital core boots up.
3. **Operational Mode Configuration:** A write transaction containing `0x00` is issued to operation register `0x4C` to establish Active Normal Mode.
4. **Hardware Authentication Check:** A full-duplex read operation targets register `0x40`. Forcing the address MSB high ($0x40 \mid 0x80 = 0xC0$) instructs the slave to prepare its identification data. The master then transmits a dummy byte ($0x00$) to clock out the static hardware code. The resulting value ($0x32$) is converted to text and pushed out through the UART port.