# Exercise 8: dsPIC33 Cooperative Time-Triggered Scheduler & Telemetry Framework

This repository contains a real-time, time-triggered cooperative scheduling framework and a non-blocking ASCII stream frame parser designed for the Microchip dsPIC33 microcontroller architecture. The system orchestrates concurrent vehicle tasks—such as kinematics calculations, input debouncing, and automated chassis signaling—without the overhead of a formal RTOS.

## Features
* **Cooperative Time-Triggered Scheduler:** Leverages function pointers and generic object tracking arguments to execute modular task loops deterministically.
* **Phase-Shifted Load Inversion:** Minimizes peak CPU worst-case execution time (WCET) by staggering task initialization counters.
* **Non-Blocking NMEA-Style Parsing:** Safely offloads characters from an interrupt-driven UART circular ring buffer using a lightweight state-machine parser.
* **Automated Safety Cascades:** Features built-in hardware error checks, bounding limit saturations, and automatic brake/hazard indicators.

---

## Project Architecture & Task Matrix

The execution pipeline runs on a baseline **1 ms Heartbeat** derived from Hardware Timer 1. Tasks are scaled across the timing grid using clock division constraints ($T = N \times \text{Heartbeat}$):

| Task Name | Target Period ($T$) | Tick Multiplier ($N$) | Offset ($n$) | Objective / Responsibility |
| :--- | :--- | :--- | :--- | :--- |
| **`task_parse_uart`** | 1 ms | 1 HB | 0 | Unloads UART ring buffer, runs the ASCII state machine parser, and updates telemetric variables. |
| **`task_poll_button`**| 10 ms | 10 HB | -1 | Polls pin `RE8` to evaluate falling edges for software debouncing and toggles master power. |
| **`task_update_pwm`** | 10 ms | 10 HB | 0 | Updates wheel output commands at 100 Hz based on active kinematic configurations. |
| **`task_manage_lights`**| 250 ms | 250 HB | 0 | Governs the chassis signaling state machine and generates 1 Hz / 2 Hz blink oscillations. |

---

## Telemetry Protocol Specification

The embedded system acts as a passive slave monitoring an ASCII-text NMEA style framing protocol stream over its serial lines:

### 1. Control Packet Structure
`$REF,speed,yawrate*`

* **`$`** : Absolute start-of-frame packet delimiter.
* **`REF`** : Message prefix type indicator identifier.
* **`speed`** : Textual integer mapping linear velocity demand bounds (`-100` to `100`).
* **`yawrate`** : Textual integer mapping differential rotational angular bounds (`-100` to `100`).
* **`*`** : Absolute end-of-frame payload seal delimiter.

### 2. Handshake Verification Feedback Loop
* **Success Handshake:** If an incoming message matches the strict template constraints and parameters fall inside valid mathematical bounds, the system updates its internal structures and instantly replies to the host PC with `OK\r\n`.
* **Failure Handshake:** If any character is malformed, missing delimiter coordinates, or contains values out-of-bounds, the execution block drops the packet and replies with `ERR\r\n`.

---

## Automated Vehicle Signaling Matrix

The chassis lighting and signaling arrays shift automatically based on the master operational states of the vehicle:

* **State A: Vehicle Stopped (`motor_active == false`)**
  * **Brakes (RF0):** Driven continuously **ON**.
  * **Low-Intensity Hazards (RG1):** Driven continuously **OFF**.
  * **Direction Indicators (RB8 & RF1):** Toggled together out-of-phase at **1 Hz** to signal a static hazard condition.
* **State B: Vehicle Moving (`motor_active == true`)**
  * **Brakes (RF0):** Driven continuously **OFF**.
  * **Low-Intensity Hazards (RG1):** Driven continuously **ON**.
  * **Steering Indicators (RB8 & RF1):**
    * If `yawrate > 50` (Turning Left): `RF1` is killed, and `RB8` flashes at **2 Hz**.
    * If `yawrate < -50` (Turning Right): `RB8` is killed, and `RF1` flashes at **2 Hz**.
    * If `yawrate` falls between `-50` and `50` (Straight Line): Both directional indicators remain **OFF**.

---

## Directory Configuration Reference

To compile the codebase successfully, verify your project link maps are arranged using this exact directory file configuration. Header includes rely on this structure to prevent compilation blocker conflicts:

```text
├── library/
│   ├── pwm.h
│   ├── scheduler.h
│   ├── timer.h
│   └── uart.h
└── src/
    └── main.c