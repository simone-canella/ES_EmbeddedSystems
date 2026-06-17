# Exercise5(task-1): SPI Driver Initialization and Sensor Authentication Pipeline

## Overview
This component implements a synchronous serial architecture using the dsPIC33 SPI1 peripheral to initialize and authenticate an external 9DOF 3 Click sensor board. The application handles multi-stage power transitions and executes a full-duplex hardware shift register exchange to verify physical device connectivity.

## SPI Peripheral Configuration
* **Bus Topography:** Operates in Master Mode with an 8-bit word frame width.
* **Signal Remapping:** * `MISO1` (Data Input) is bound to `RA1` via remappable line `RPI17`.
  * `MOSI1` (Data Output) is bound to `RF13` via remappable line `RP109`.
  * `SCK1` (Bus Clock) is bound to `RF12` via remappable line `RP108`.
* **Timing and Polarity:** Clock parameters are locked to an Idle-High level ($\text{CKP} = 1$). Frequency division is throttled via a 1:1 Primary and 5:1 Secondary scaling network.
* **Chip Selection:** Managed explicitly using digital output line `RD6` (Chip Select 3), keeping all neighboring components high (inactive) on the shared bus line to avoid bus contention.

## Core Operation Protocol Sequence
1. **Power Mitigation:** The magnetometer is awakened from Suspend Mode by issuing a write instruction containing `0x01` to power register `0x4B`.
2. **Temporal Budgeting:** The application halts execution for $3\text{ ms}$ to provide the required stabilization window while the sensor's digital core boots up.
3. **Operational Mode Configuration:** A write transaction containing `0x00` is issued to operation register `0x4C` to establish Active Normal Mode.
4. **Hardware Authentication Check:** A full-duplex read operation targets register `0x40`. Forcing the address MSB high ($0x40 \mid 0x80 = 0xC0$) instructs the slave to prepare its identification data. The master then transmits a dummy byte ($0x00$) to clock out the static hardware code. The resulting value ($0x32$) is converted to text and pushed out through the UART port.

---

# SPI Magnetometer X-Axis Telemetry Streaming

## Technical Specifications & Peripheral Configuration

### 1. SPI Synchronous Module Setup
* **Bus Architecture:** Configured for full-duplex synchronous operation where the master generates the serial clock (`SCK1`) to completely control bit timing.
* **Word Sizing:** Locked to an 8-bit data frame width.
* **Clock Parameters:** Clock polarity is configured to an Idle-High state (`CKP = 1`), matching the sensor's timing requirements.
* **Baud Generation:** Configured with a 1:1 Primary Prescaler and a 5:1 Secondary Prescaler to safely scale the bus frequency relative to the instruction clock ($F_{CY}$).

### 2. Multi-Byte Burst Read Exchange
Magnetic field measurements are sampled continuously within a deterministic 100 ms master frame window ($10\text{ Hz}$ frequency) driven by `Timer 1` period-match events. The X-axis data is read using a multi-byte burst read sequence:
* The master drives the chip select line low and transmits the base register read command header byte `0xC2` (`0x42 | 0x80`), forcing the address most significant bit high to notify the slave of a read access request.
* text
  * While keeping the chip select line low, the master transmits two consecutive dummy bytes (`0x00`) to supply clock pulses to the bus.
  * The slave automatically increments its internal register address pointer, shifting out the LSB data byte from register `0x42` followed immediately by the MSB data byte from register `0x43`.

### 3. Signed 13-Bit Data Reconstruction
The split 8-bit registers are combined into a signed 16-bit integer using bitwise operations:
1. **Status Bit Filter:** The 3 lowest bits of the raw LSB byte (bits 0, 1, and 2) contain internal self-test and hardware flags and are isolated using a bitwise AND mask with `0xF8`.
2. **Byte Alignment:** The raw MSB byte is left-shifted by 8 positions (`msb_byte << 8`) to line up with the upper half of the 16-bit signed container and combined with the masked LSB via a bitwise OR operation.
3. **Arithmetic Sign Extension:** The intermediate variable is right-shifted by 3 slots (`data >> 3`) to scale it to its true magnitude. Because the variable is signed, the compiler executes an arithmetic right shift, copying the sign bit into the upper empty slots to
