
# Exercise6: Autonomous Dual-Channel Background ADC Scan Infrastructure

## Overview
This component implements a completely automated, background analog-to-digital acquisition subsystem using the dsPIC33 hardware ADC1 peripheral. Operating without software multiplexer tracking interventions, the system uses hardware Scan Mode to sample physical battery metrics and an external front-mounted infrared proximity sensor synchronously at 1 kHz, reporting scaled floating-point values over a 10 Hz non-blocking serial UART framework.

## Module Configuration & Hardware Allocation
* **Quantization Formatting:** Module is configured in 10-bit Successive Approximation Mode, outputting integer parameters bounded from 0 to 1023 counts over an instruction-derived conversion clock period ($T_{AD} = 4.5 \cdot T_{CY}$).
* **Gating Controls:** Locked to Free-Running Auto-Sample Start ($ASAM = 1$) and Auto-Conversion End ($SSRC = 7$), letting the internal timing counter trigger conversion after an explicit 16 $T_{AD}$ sample window.
* **Scan Network Topology:** Auto-scan routing ($CSCNA = 1$) is bound to analog lines `AN11` (internal battery monitoring voltage divider) and `AN15` (chassis physical forward IR sensor input pin `RB15`). 
* **Buffer Mapping Logic:** The hardware populates its internal result buffers in absolute ascending channel order. This locks the battery monitor results to `ADC1BUF0` and the proximity sensor results to `ADC1BUF1`. The interrupt mask is scaled to $SMPI = 1$ to cycle the `DONE` conversion complete flag once both hardware slots update.

## Execution Timeline & Down-Sampling Math
1. **Periodic Loop Base:** The master real-time executive architecture runs at 1 kHz ($1\text{ ms}$ interval framework), driving the `adc_converter` state machine at a rate matching the background conversion logic.
2. **Telemetry Frequency Throttling:** A non-blocking loop frame counter accumulates 100 cycles to scale the output down to 10 Hz ($100\text{ ms}$ interval framework). This formats data into an ASCII string buffer before queuing it into the UART ring track.
3. **Mathematical Reconstruction Scales:**
   * **Battery Voltage Monitor:** Decodes raw codes via a $1023.0\text{f}$ scale factor against the $3.3\text{ V}$ ceiling, multiplying by $3.0\text{f}$ to reverse the hardware's 1/3 voltage divider network.
   * **Linearized Distance Tracking:** Converts raw codes to a floating-point voltage ($V$) and passes them through a 4th-order polynomial model. The result is scaled by $100.0\text{f}$ to express the final telemetry metric explicitly in centimeters:
   
$$\$SENS,x,y*$$