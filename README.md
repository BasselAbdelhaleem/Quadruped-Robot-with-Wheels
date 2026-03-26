# 🤖 Quadruped-Robot-with-Wheels

> A four-legged robot with mecanum wheels at its feet — combining the terrain adaptability of legged locomotion with the speed and efficiency of wheeled drive.

<div align="center">
  <table style="width: 100%;">
    <tr>
      <td style="width: 60%; vertical-align: top;">
        <img src="https://github.com/user-attachments/assets/d3ecca32-4f69-4e50-ac95-dd422a21d0ac" height="250"></img>
      </td>
      <td style="width: 40%; vertical-align: top;">
        <img src="https://github.com/user-attachments/assets/2b54ad78-1648-4194-a348-db9c3ca9c86a" height="250"></img>
      </td>
    </tr>
  </table>
</div>

---

## 🧠 What Is This?

This is a custom-designed and fabricated quadruped robot that replaces the conventional foot end-effector with **mecanum wheels** — enabling both legged locomotion over uneven terrain and omnidirectional wheeled movement on flat surfaces. The robot is designed to be fully **modular**, with any component replaceable without modification to surrounding parts, making it ideal for research and iterative development.

Inspired by industry platforms like **Swiss-Mile (ETH Zurich)** and **Spot (Boston Dynamics)**, QuadWheel was built from the ground up using accessible materials and manufacturing methods — proving that advanced robot design is achievable outside of well-funded labs.

This project covers the complete hardware lifecycle — from concept and CAD to fabrication, assembly, and embedded low-level motor control.

---

## 🎥 Demo

<table style="width: 100%;">
  <tr>
    <td style="width: 33%; vertical-align: top;">
      <video src="https://github.com/user-attachments/assets/62846b2c-08e2-43ef-a53b-30cdd5eee27c"></video>
    </td>
    <td style="width: 33%; vertical-align: top;">
      <video src="https://github.com/user-attachments/assets/35af5436-f93d-4f27-a48a-da1c4396e53f"></video>
    </td>
    <td style="width: 33%; vertical-align: top;">
      <video src="https://github.com/user-attachments/assets/56d87e05-a18e-4513-a69d-81bef750ae9e"></video>
    </td>
  </tr>
</table>

---

## ✨ Key Features

- **Leg-wheel hybrid locomotion** — mecanum wheels as end-effectors enable both legged walking and omnidirectional rolling
- **Novel knee mechanism** — hip-mounted motor drives the knee via a lead screw, minimizing leg inertia and power consumption
- **3 DOF per leg** — hip abduction/adduction, hip flexion/extension, and knee flexion/extension
- **Incremental encoder feedback** — custom-designed optical encoder wheels on each joint for position and speed sensing
- **Modular architecture** — every subsystem (leg, motor hub, wheel bracket) is independently replaceable
- **Multi-material construction** — aluminum frame and structural links with 3D-printed enclosures and encoder components
- **Low-level stepper motor control** — closed-loop position and speed control via Arduino

---

## 🏗️ System Architecture

```
┌──────────────────────────────────────────────┐
│         HIGH-LEVEL CONTROL  (Future)         │
│   Gait Planning · Navigation · Vision        │
└───────────────────┬──────────────────────────┘
                    │ Serial / Communication
┌───────────────────▼──────────────────────────┐
│        LOW-LEVEL CONTROL  (Arduino)          │
│  Stepper Control · Encoder Feedback · PWM    │
└───────────────────┬──────────────────────────┘
                    │
┌───────────────────▼──────────────────────────┐
│              ACTUATORS & SENSORS             │
│  12× Stepper Motors · Optical Encoders       │
│  4× DC Wheel Motors · Motor Drivers          │
└──────────────────────────────────────────────┘
```

---

## 🔧 Hardware

### Platform Overview

| Property | Value |
|---|---|
| **Locomotion Type** | Leg-Wheel Hybrid |
| **Legs** | 4 |
| **DOF per Leg** | 3 (ab/ad + hip fl/ex + knee fl/ex) |
| **Total Joint Motors** | 12 × NEMA 17 Stepper |
| **Wheel Motors** | 4 × High-RPM DC Motor |
| **End Effector** | 120mm Mecanum Wheel |
| **Total Weight** | ~6 kg |
| **Max Leg Extension** | 665 mm |

### Mechanical Structure

#### Body
The chassis consists of an **aluminum square tube frame** (20mm × 20mm × 2mm wall) providing structural rigidity while keeping weight low. 3D-printed protective plates enclose the body and mount the legs, while a central electronics carriage houses all control hardware behind a removable cover for easy access.

| Body Component | Material | Quantity |
|---|---|---|
| Main frame tubes | Aluminum (2mm wall) | 10 |
| Protective side plates | 3D-printed PLA | 4 |
| Electronics carriage | 3D-printed PLA | 1 |
| Face plates | 3D-printed PLA | 2 |

#### Leg Design

Each leg has five major subsystems:

**1. Hip Joint (ab/ad) — Fixed Motor Hub**
Mounts the leg to the body and enables lateral swing. Houses the optical encoder disk and speed sensor pair.

**2. Hip Joint (fl/ex) — Rotating Motor Hub**
Drives the leg forward and backward. Coupled to the fixed motor hub via a D-shaft coupler, with a bearing support on the body side for smooth, supported rotation.

**3. Knee Mechanism — Lead Screw Drive**
A stepper motor at the hip rotates a lead screw which drives a linear slider along the upper leg. Two connecting plates link the slider to the lower leg, converting linear motion into knee rotation. This design keeps motor mass close to the hip pivot, **minimizing leg inertia** — the same approach used in Spot and Mini Cheetah.

```
Motor (rotation) → Lead Screw → Slider (linear) → Connecting Plates → Knee (rotation)
```

**4. Upper Leg Frame**
Four aluminum plates in a hollow truss pattern — strong under bending but material-efficient. Mounts the linear bearing block (for the slider) and pivots the lower leg.

**5. Lower Leg + Wheel Assembly**
An aluminum rod with a slotted profile that accepts:
- **Wheel bracket** — U-shaped plate holding the mecanum wheel on a bearing-supported rod
- **Wheel motor hub** — Houses a high-RPM DC motor; slides along the slot to tension the drive belt

The wheel motor drives the mecanum wheel via a **pulley-and-belt transmission**, keeping the motor mass on the leg body rather than at the foot — again minimizing rotational inertia.

### Mecanum Wheels

Mecanum wheels replace the conventional foot, providing **two degrees of freedom from a single motor**. By coordinating all four wheels, the robot can move forward, sideways, diagonally, or rotate in place — dramatically improving mobility on flat surfaces.

The wheel design was adapted from an open-source model and modified:
- Diameter scaled from 80mm → **120mm** for the robot's size
- Roller pin diameter changed from 3mm → **5mm** for local availability
- Mounting interface redesigned for belt-driven rod (rather than direct motor shaft)

```
Wheel directions achievable:
  Forward / Backward / Left / Right
  Clockwise / Anticlockwise rotation
  Diagonal movement in all directions
```

### Motor Selection & Gearbox

**Torque analysis (worst-case scenarios):**

| Scenario | Motor | Required Torque |
|---|---|---|
| Lifting leg (COM at knee) | Hip fl/ex | **6.23 Nm** |
| Lifting body with four legs | Knee | 3.5 Nm |
| Carrying body on two legs | Hip ab/ad | 1.91 Nm |

No single off-the-shelf motor available locally met the 6.23 Nm requirement. After evaluating DC, stepper, and brushless options across torque, size, weight, and bidirectional driver availability, **NEMA 17 stepper motors** were selected as the best balance — with a **1:15 planetary gearbox** added to reach the required output torque with a 1.2× safety factor.

| Motor | Max Torque | Speed | Count |
|---|---|---|---|
| NEMA 17 Stepper + 1:15 Gearbox | ~7.5 Nm (output) | — | 12 (joints) |
| High-RPM DC Motor | — | High | 4 (wheels) |

### Encoder Design

Custom **incremental optical encoders** were designed and 3D-printed for each joint motor:

- An encoder disk with **evenly spaced slots** (35mm diameter) mounts on the motor shaft
- Two photoelectric speed sensors are positioned on the disk at a **5° phase offset** to detect both speed and direction of rotation
- The quadrature signal (A/B channels) is read by the Arduino to compute joint position and velocity

This was a deliberate design challenge — servo motors with built-in encoders were intentionally avoided to demonstrate proficiency with raw actuator control.

### Manufacturing Methods

| Process | Applied To |
|---|---|
| **Welding** | Aluminum frame assembly |
| **Laser Cutting** | Side plates, top/bottom plates, connecting plates, encoder disks, wheel bracket |
| **Lathe (turning)** | Slider body, lower leg, wheel bracket neck |
| **Milling** | Slots, holes, flats on lower leg and slider |
| **3D Printing** | Covers, motor hubs, bearing extensions, encoder disks, mecanum rollers |
| **Bending** | Upper leg top/bottom plates, wheel bracket plate |

---

## 💻 Low-Level Control (Arduino)

### Stepper Motor Control

Each of the 12 NEMA 17 stepper motors is driven via a dedicated **stepper motor driver**. The Arduino generates step and direction signals to command precise angular positions.

- **Open-loop stepping** for commanded position targets
- **Closed-loop speed regulation** using encoder feedback to verify actual joint velocity
- Step resolution tunable via microstepping settings on the driver

### Encoder Feedback

The custom incremental encoders produce quadrature (A/B) pulse trains. The Arduino reads these via **interrupt-driven routines** for accurate, low-latency position and velocity estimation:

```cpp
// Quadrature decoding — direction from phase relationship
void encoderISR() {
  int a = digitalRead(ENC_A);
  int b = digitalRead(ENC_B);
  if (a == b) encoderCount++;
  else        encoderCount--;
}
```

Joint angle is tracked by counting encoder pulses and mapping to degrees using the disk's slot count.

### Motor Driver Interfacing

Each stepper driver receives:
- `STEP` — pulse per microstep
- `DIR` — rotation direction
- `EN` — enable/disable motor hold torque

The DC wheel motors are driven via separate motor drivers using PWM speed control and direction pins.

---

## 📐 Kinematics Overview

Each leg has 3 active joints providing a 3D workspace:

| Joint | DOF | Motion |
|---|---|---|
| Hip ab/ad | Rotation about body axis | Swings leg laterally |
| Hip fl/ex | Rotation about transverse axis | Swings leg forward/backward |
| Knee fl/ex | Rotation about transverse axis | Extends/retracts lower leg |

**Creeping gait** (statically stable): legs raised and lowered sequentially — at least 3 legs always on the ground. This is the planned initial gait for higher-level control implementation.

---

## 📁 Repository Structure

```
QuadWheel/
├── cad/
│   ├── body/                   # Body frame, covers, electronics carriage
│   ├── leg/                    # Upper leg, lower leg, slider, connecting plates
│   ├── wheel/                  # Mecanum wheel, bracket, motor hub
│   ├── encoder/                # Encoder disk, motor hub designs
│   └── gearbox/                # Modified 1:15 planetary gearbox
├── arduino/
│   ├── stepper_control/        # Joint position & speed control
│   ├── encoder_reader/         # Quadrature interrupt decoding
│   └── motor_driver_interface/ # Step/dir/enable signal generation
├── engineering_drawings/       # Full dimensioned drawings for all parts
├── media/                      # Photos, videos, demo footage
├── docs/
│   └── thesis.pdf              # Full design & fabrication documentation
└── README.md
```

---

## 🚀 Getting Started

### Hardware Requirements

- 12× NEMA 17 Stepper Motor + 1:15 Gearbox
- 12× Stepper Motor Driver (e.g., A4988 or DRV8825)
- 4× High-RPM DC Motor
- 4× DC Motor Driver
- Arduino Mega (recommended for pin count)
- 24× Photoelectric Speed Sensors (2 per joint)
- Custom encoder disks (3D print from `/cad/encoder/`)

### Software Setup

```bash
# Clone the repository
git clone https://github.com/yourusername/QuadWheel.git
cd QuadWheel/arduino

# Open in Arduino IDE
# Upload stepper_control/stepper_control.ino to your Arduino
```

### Running Joint Control

```cpp
// Example: Command hip flexion joint to 45 degrees
setJointAngle(HIP_FLEX_FL, 45.0);

// Example: Read current joint position from encoder
float angle = getJointAngle(HIP_FLEX_FL);
```

---

## 🔮 Future Work

The hardware and low-level firmware provide a solid foundation. Planned next steps:

- **Gait controller** — implement creeping (statically stable) and trotting gaits
- **Inverse kinematics** — foot-space to joint-space mapping for trajectory following
- **High-level autonomy** — integrate a single-board computer (Raspberry Pi / Jetson) for navigation
- **Brushless motor upgrade** — replace steppers with bidirectional brushless motors for higher power density
- **TPU roller replacement** — reprint mecanum wheel rollers in flexible filament for better grip
- **Sensor integration** — add IMU for body orientation, depth camera for terrain perception

---

## 🧩 Skills Demonstrated

| Domain | Demonstrated Skills |
|---|---|
| **Mechanical Design** | 3-DOF leg kinematics, lead-screw knee mechanism, belt-drive transmission, modular assembly |
| **CAD & Engineering Drawing** | Full robot CAD, toleranced engineering drawings for every custom part |
| **Manufacturing** | Welding, laser cutting, CNC lathe, milling, 3D printing — end-to-end fabrication |
| **Embedded Systems** | Arduino stepper control, quadrature encoder decoding, interrupt-driven ISRs |
| **Actuator Selection** | Torque analysis, gear ratio calculation, motor benchmarking under real constraints |
| **System Design** | Modular architecture for research-oriented iterative development |

---

## 📄 Documentation

The full design rationale, torque calculations, material selection, and fabrication process are documented in the thesis paper:

📄 [`docs/thesis.pdf`](docs/thesis.pdf) — *Design and Implementation of Quadruped with Wheels*, GUC Mechatronics Engineering, 2023

---

## 👤 Author

**Bassel Khaled Abdelhaleem**
Mechatronics Engineering — German University in Cairo (GUC)
Supervised by: Assoc. Prof. Dr. Eng. Amir Roushdy

📧 your.email@example.com
🔗 [LinkedIn](https://linkedin.com/in/yourprofile)

---

## 📄 License

This project is licensed under the MIT License — see [LICENSE](LICENSE) for details.

---

<p align="center">
  <i>Designed from scratch. Fabricated by hand. Driven by curiosity.</i>
</p>
