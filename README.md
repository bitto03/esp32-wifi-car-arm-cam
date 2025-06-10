# ESP32 Wemos D1 R32 Smart Car with 4-DOF Robotic Arm & Camera Mount

This project is a Wi-Fi-controlled robotic car based on the **ESP32 Wemos D1 R32**, featuring:
- L298N motor drive for movement (forward, backward, left, right, stop)
- A 4-DOF robotic arm controlled 
- A 180-degree servo-mounted camera platform
- Web interface with sliders for servo control and buttons for movement
- Access Point mode (no internet required) — just connect and control!

## Features

- Wi-Fi Access Point control (no router needed)
- Real-time web-based interface using AJAX
- 4-DOF robotic arm (6 servo motors)
- Dedicated servo for 180° camera panning
- Responsive robot car movement with adjustable speed
- Fully mobile-friendly control interface

##  Hardware Required

| Component                | Quantity |
|--------------------------|----------|
| ESP32 Wemos D1 R32       | 1        |
| L298N Motor Driver       | 1        |
| TT Gear Motors (2 or 4)  | 2 or 4   |
| Servo Motors (SG90/MG90S)| 6        |
| 180° Servo (for camera)  | 1        |
| Robot Car Chassis        | 1        |
| Wheels                   | 2 or 4   |
| Power Supply (Battery Pack or 18650) | 1 |
| Jumper Wires             | as needed |
| Breadboard or PCB (optional) | 1     |

##  Pin Configuration

### Motors (connected to L298N):
| Function   | ESP32 Pin |
|------------|-----------|
| ENA        | D10 (GPIO 4)  |
| IN1        | D9 (GPIO 5)   |
| IN2        | D8 (GPIO 18)  |
| IN3        | D7 (GPIO 19)  |
| IN4        | D6 (GPIO 23)  |
| ENB        | D5 (GPIO 27)  |

### Servos:
| Servo No.     | Function               | GPIO Pin |
|---------------|------------------------|----------|
| Servo 0       | Base of robotic arm    | 13       |
| Servo 1       | Arm segment 1          | 12       |
| Servo 2       | Arm segment 2          | 14       |
| Servo 3       | Gripper                | 16       |
| Servo 4       | Special constraint (0-20°) | 26    |
| Servo 5       | Camera mount (180°)    | 25       |

> Note: Servo 4 is limited to 0–20° due to hardware design constraints.

##  Web Interface

- Forward, Backward, Left, Right, Stop buttons
- 6 Servo sliders (0–180°, except Servo 4)
- Real-time interaction using AJAX
- Works on mobile and desktop browsers

### Access Point Info
- **SSID**: `ROBOT_AP`
- **Password**: `12345678`
- **Control URL**: [http://192.168.4.1](http://192.168.4.1)

## Setup Instructions

1. Connect all hardware components.
2. Upload the provided code using Arduino IDE (ESP32 board selected).
3. Power the robot with appropriate batteries.
4. Connect your phone or PC to Wi-Fi network `ROBOT_AP`.
5. Open browser and go to [http://192.168.4.1](http://192.168.4.1).
6. Control your robot and robotic arm in real time.

##  Notes

- Be sure to use an external power supply for motors and servos. The ESP32 cannot supply enough current.
- If your servos jitter or behave erratically, add capacitors near their VCC line.
- Tune servo limits to avoid mechanical strain.

##  Project by

**Basirul Hasin Bitto (Mr. BHB)**  
From BHB Enterprise  . 
University of Asia Pacific,CSE

---

Happy robotics!
