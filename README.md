# Gesture Glove

Gesture Glove is a wearable human–computer interaction device that combines an inertial measurement unit (IMU), Hall-effect sensors, magnets, and Bluetooth Low Energy to detect hand and finger gestures.

The prototype is designed around the **Seeed Studio XIAO nRF52840** and focuses on detecting three-finger movement using Hall-effect sensors placed near the back of the hand and magnets attached to the distal phalanges.

> **Status:** In development. Sensor testing and prototype hardware evaluation are underway.

## Features

- Wearable gesture-based input device
- IMU mounted on the back of the hand
- Pitch and roll tracking
- Hall-effect sensing for finger curl detection
- Magnetic position sensing using small permanent magnets
- Three-finger input support
- Bluetooth Low Energy communication
- Configurable gesture thresholds
- Privacy-friendly, camera-free interaction
- Potential support for PC HID input

## Project Goals

The project is intended to explore the design of a compact wearable input device, including:

- Human–computer interaction
- Wearable sensor integration
- Embedded firmware development
- Hall-effect sensor characterization
- IMU-based hand orientation tracking
- Bluetooth Low Energy communication
- Gesture classification
- Real-time input processing
- Hardware prototyping and testing

## Repository Structure

```text
Gesture-Glove/
├── firmware/        # XIAO nRF52840 firmware
├── hardware/        # Schematics, PCB, wiring, and mechanical files
├── docs/            # Design notes, testing, and technical documentation
├── data/            # Sensor measurements and experimental data
├── media/           # Photos, diagrams, and project media
├── README.md
└── LICENSE
```

### Firmware

The firmware is being developed for the **Seeed Studio XIAO nRF52840**.

Planned firmware responsibilities include:

- Hall-effect sensor acquisition
- IMU data acquisition
- Sensor calibration
- Finger curl threshold detection
- Gesture recognition
- Pitch and roll estimation
- Bluetooth Low Energy communication
- Optional USB or Bluetooth HID output
- Sensor status and diagnostic output

A planned firmware organization is:

```text
firmware/
├── include/
├── lib/
├── src/
├── test/
└── platformio.ini
```

## Hardware Platform

The current prototype is based around the **Seeed Studio XIAO nRF52840**.

The board was selected because it provides:

- Bluetooth Low Energy
- Built-in IMU
- Small wearable form factor
- Low-power operation
- ARM Cortex-M4 microcontroller
- USB programming and serial communication
- Sufficient analog and digital interfaces for prototype testing

The XIAO nRF52840 will coordinate the IMU, Hall-effect sensors, gesture-processing logic, and wireless communication.

## Sensor Configuration

The glove uses two primary sensing methods.

### IMU

The IMU is mounted on the back of the hand and is used to estimate hand orientation.

The initial implementation focuses on:

- Hand pitch
- Hand roll
- Orientation changes
- Hand rotation and movement context

The IMU can provide additional information that helps distinguish similar finger configurations performed in different hand orientations.

### Hall-Effect Sensors

Hall-effect sensors are used to detect changes in magnetic field strength as the fingers move.

The sensors are positioned on or near the back of the hand, while magnets are attached to the backs of the distal phalanges. As a finger curls or extends, the distance and alignment between the magnet and sensor changes.

The initial prototype uses threshold-based detection:

- Finger extended
- Finger curled
- Transition between states

Future versions may use the analog sensor readings to estimate a larger range of finger movement.

## Finger Configuration

The current design focuses on three fingers:

- Thumb
- Index finger
- Middle finger

The sensor arrangement may be revised during testing to improve:

- Sensor range
- Signal strength
- Repeatability
- Mechanical comfort
- Finger independence
- Resistance to cross-talk

## Gesture Detection

The first prototype is intended to detect simple, reliable gestures rather than continuously track every point of finger movement.

Potential gestures include:

- Finger curl combinations
- Pinch-like movements
- Directional flicks
- Hand rotation
- Hand tilt
- Custom multi-finger commands

The final gesture set will depend on sensor testing, calibration quality, and classification accuracy.

## Design Targets

The current design targets are:

| Metric | Target |
|---|---:|
| Gesture accuracy | At least 90% |
| Input latency | Less than 100 ms |
| False-positive rate | Less than 5% |
| Battery life | At least 2 hours |
| Finger inputs | Three fingers |
| Orientation inputs | Pitch and roll |

These values are preliminary engineering targets and may change as prototype testing produces measured results.

## Hardware Development

The hardware portion of the project may include:

- Hall-effect sensor breakout boards
- Custom sensor wiring
- XIAO nRF52840 carrier board
- Battery and power circuitry
- Glove or wearable mounting system
- Sensor brackets and protective covers
- Magnet mounts for the distal phalanges
- Custom PCB design in a later revision

A planned organization is:

```text
hardware/
├── schematics/
├── pcb/
└── mechanical/
    ├── stl/
    └── step/
```

## Hall-Effect Sensor Testing

Several linear Hall-effect sensors are being evaluated for the prototype, including:

- A1304
- A1324
- A1325
- A1326

Testing focuses on:

- Magnetic field response
- Usable sensor range
- Output voltage range
- Sensor saturation
- Magnet size and strength
- Finger movement repeatability
- Sensor-to-sensor variation
- Resolution near the detection threshold

The initial prototype prioritizes reliable detection of finger states. Continuous finger-position tracking may be evaluated after the threshold-based system is validated.

## Magnets

Small N52 neodymium magnets are being evaluated for attachment to the distal phalanges.

Magnet testing considers:

- Magnet dimensions
- Magnetic field strength
- Sensor spacing
- Finger comfort
- Mechanical alignment
- Detection range
- Signal resolution
- Saturation near the sensor

The magnet mounting method will be refined as the glove design develops.

## Bluetooth Low Energy

Bluetooth Low Energy is intended to provide wireless communication between the glove and a host device.

Potential wireless functions include:

- Transmitting detected gestures
- Sending pitch and roll data
- Configuring thresholds
- Streaming sensor data during testing
- Connecting to a PC or mobile device
- Acting as a custom HID input device

## Applications

Potential applications include:

- Presentation control
- Media control
- Accessibility input
- Virtual reality interaction
- Robotics control
- Drone or vehicle control
- Custom computer interfaces
- Experimental human–computer interaction research

## Development

Firmware development is being performed using a compatible embedded development environment such as **Arduino IDE** or **PlatformIO**.

A typical firmware workflow is:

```bash
cd firmware
code .
```

Sensor measurements and test data may be exported for analysis using tools such as:

- Serial Monitor
- Serial Plotter
- Python
- Excel
- MATLAB

Generated build files and temporary outputs should not be tracked by Git.

## Current Progress

- [x] Project concept and system goals
- [x] Selection of the XIAO nRF52840
- [x] Initial IMU placement identified
- [x] Initial three-finger configuration defined
- [x] Hall-effect sensor candidates identified
- [x] Magnet sizes selected for initial testing
- [x] Preliminary Hall-effect sensor testing
- [ ] Single-sensor MCU data logging
- [ ] Sensor response plots and range comparison
- [ ] Sensor calibration system
- [ ] Magnet mounting design
- [ ] Glove mounting design
- [ ] Three-sensor prototype
- [ ] Finger curl threshold detection
- [ ] IMU pitch and roll processing
- [ ] Gesture classification
- [ ] Bluetooth Low Energy communication
- [ ] Host-device input integration
- [ ] Battery and power system integration
- [ ] Wearable enclosure or mounting system
- [ ] Accuracy and latency testing
- [ ] Final documentation

## Planned Documentation

As development continues, this repository will include documentation covering:

- System architecture
- Hardware selection
- Sensor placement
- Hall-effect sensor testing
- Magnet selection
- Sensor calibration
- IMU orientation processing
- Gesture-detection algorithms
- Bluetooth Low Energy communication
- Power and battery design
- Mechanical mounting
- Testing methodology
- Accuracy and latency results
- Future PCB revisions

## License

See [LICENSE](LICENSE) for licensing information.
