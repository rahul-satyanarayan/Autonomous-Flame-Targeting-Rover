# Autonomous Flame-Targeting Firefighting Rover

An autonomous, mobile robotics platform engineered around the **Arduino Uno** and **Adafruit L293D Motor Shield v1**. The rover performs dynamic multi-sector sweeps using a servo-mounted analog infrared flame photodiode, localizes thermal hotspots using threshold evaluation, and executes closed-loop differential drive maneuvers toward the fire source.

---

## Live Demonstration

<p align="center">
  <img src="docs/flame_rover_demo.gif" alt="Autonomous Flame Rover Demonstration" width="80%">
</p>

---

## Technical Specifications & Features

- **5-Sector Scanning Kinematics:** A 9g micro-servo pans the flame receiver across $0^\circ$, $45^\circ$, $90^\circ$, $135^\circ$, and $180^\circ$ to acquire spatial infrared intensity vectors.
- **Dynamic Threshold Classification:**
  - **Ambient Threshold ($> 975$ ADC):** Safe state; vehicle remains halted in standby mode.
  - **Target Tracking ($100 - 975$ ADC):** Determines the lowest ADC return (highest IR irradiance) and engages directional approach bursts.
  - **Critical Proximity Halt ($< 100$ ADC):** Fire zone reached; triggers full motor cutoff to maintain safe standoff distance.
- **Calibrated Differential Motor Drive:**
  - Compensates for chassis and gearbox asymmetry via calibrated left/right PWM bias (`L_MAX = 155`, `R_MAX = 120`).
  - Controlled pivot rates (`L_PVPOT_SPEED = 75`, `R_PVPOT_SPEED = 60`) for stable rotational alignment[cite: 41].
- **Isolated Power Architecture:** High-current DC motors driven through the shield's `EXT_PWR` rail, preventing microcontroller resets from inductive motor kickback.

---

## System Architecture & Circuit Schematic

### Circuit Connection Diagram
<p align="center">
  <img src="docs/Autonomous%20Flame-Targeting%20Rover%20Schematic%20Connection%20Diagram.png" alt="Autonomous Flame-Targeting Rover Schematic" width="100%">
</p>

> **Vector Document:** The high-resolution vector PDF is archived under [`docs/Autonomous Flame-Targeting Rover Schematic Connection Diagram.pdf`](docs/Autonomous%20Flame-Targeting%20Rover%20Schematic%20Connection%20Diagram.pdf).

---

## Hardware Pinout Matrix

| Subsystem / Component | Module Pin | Shield / Arduino Pin | Voltage Rail | Description |
| :--- | :--- | :--- | :--- | :--- |
| **Scanner Servo Motor** | **PWM Signal** | **Pin 9 (SERVO_2 Header)** | `5VDC` Regulated | Sweep actuator ($0^\circ - 180^\circ$ scan cycles)[cite: 41] |
| | **VCC / GND** | `5V` / `GND` | `5VDC` Rail | Servo power distribution |
| **Analog Flame Sensor** | **Analog Output** | **Analog Pin A2** | `5VDC` Logic | IR photodiode analog feedback ($0 - 1023$ ADC)[cite: 41] |
| | **VCC / GND** | `5V` / `GND` | `5VDC` Logic | Sensor bias rail |
| **Left Drive Motor (BO)** | **Motor Leads (+/-)** | **Shield Terminal M3** | Up to `12VDC` (PWM) | Left wheel propulsion (L293D H-Bridge Channel)[cite: 41] |
| **Right Drive Motor (BO)** | **Motor Leads (+/-)** | **Shield Terminal M2** | Up to `12VDC` (PWM) | Right wheel propulsion (L293D H-Bridge Channel)[cite: 41] |
| **Power Management** | **Battery Leads (+/-)** | **Shield EXT_PWR Block** | `7.4V - 11.1V` Li-ion | High-current isolated motor supply rail |

---

## Directional Actuation Truth Table

| Target Sector | Sweep Angle | Movement Action | Left Motor (M3) | Right Motor (M2) | Duration |
| :---: | :---: | :--- | :--- | :--- | :---: |
| **Far Right** | $0^\circ$ | Hard Pivot Right | `FORWARD` (Speed: 75) | `BACKWARD` (Speed: 60) | $500\text{ ms}$[cite: 41] |
| **Mid Right** | $45^\circ$ | Soft Turn Right | `FORWARD` (Speed: 155) | `FORWARD` (Speed: 90) | $400\text{ ms}$[cite: 41] |
| **Center** | $90^\circ$ | Straight Drive | `FORWARD` (Speed: 155) | `FORWARD` (Speed: 120) | $400\text{ ms}$[cite: 41] |
| **Mid Left** | $135^\circ$ | Soft Turn Left | `FORWARD` (Speed: 75) | `FORWARD` (Speed: 120) | $400\text{ ms}$[cite: 41] |
| **Far Left** | $180^\circ$ | Hard Pivot Left | `BACKWARD` (Speed: 75) | `FORWARD` (Speed: 60) | $500\text{ ms}$[cite: 41] |
| **Standby / Close**| Any | Full Halt | `RELEASE` (Off) | `RELEASE` (Off) | Continuous[cite: 41] |

---

## Software & Library Requirements

Libraries declared in `libraries.txt`:
- `AFMotor` (Adafruit Motor Shield v1 Library)[cite: 41]
- `Servo` (Built-in Arduino AVR Core library)[cite: 41]

---

## Project Structure

```text
Autonomous-Flame-Targeting-Rover/
├── .gitignore
├── LICENSE
├── libraries.txt
├── README.md
├── src/
│   └── Flame_Detection_Rover_AFM.ino
└── docs/
    ├── flame_rover_demo.gif
    ├── Autonomous Flame-Targeting Rover Schematic Connection Diagram.pdf
    └── Autonomous Flame-Targeting Rover Schematic Connection Diagram.png