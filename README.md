# CamCar: Autonomous Red Ball Following Robot with Raspberry Pi & Initio Platform

![Robot Car](https://img.shields.io/badge/Project-Robot%20Car-blue.svg)
![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi%20%7C%20Initio-red.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

An intelligent robot car leveraging Raspberry Pi and the Initio platform to autonomously detect and follow a red ball while navigating obstacles. This project showcases real-time computer vision, sensor integration, and embedded systems programming for a robust robotic application.

## ✨ Features

*   **Red Ball Detection & Tracking:** Utilizes computer vision techniques to identify and continuously track a red ball in its environment.
*   **Autonomous Navigation:** The robot intelligently adjusts its movement (forward, backward, spin left/right) to follow the detected red ball.
*   **Obstacle Avoidance:** Integrated IR and ultrasonic sensors enable the robot to detect and react to obstacles, ensuring safe operation.
*   **Multi-threaded Architecture:** Employs POSIX threads for concurrent execution of camera blob detection and car control logic, enhancing real-time performance.
*   **Console Interface:** A `curses`-based text interface provides live status updates on blob information, distance, and obstacle detection.

## 🛠️ Tech Stack

*   **Hardware:**
    *   Raspberry Pi (any model with camera interface)
    *   Initio Robot Platform
    *   Raspberry Pi Camera Module
    *   IR Obstacle Sensors
    *   Ultrasonic Distance Sensor
*   **Software:**
    *   **Language:** C
    *   **Operating System:** Raspberry Pi OS (formerly Raspbian)
    *   **Libraries:**
        *   `initio.h`: For controlling the Initio robot platform's motors and sensors.
        *   `curses.h`: For creating the console-based user interface.
        *   `pthread`: POSIX threads for concurrent programming.
        *   `detect_blob.h`: Custom library for image processing and blob detection.

## 🚀 Getting Started

These instructions will get you a copy of the project up and running on your Raspberry Pi for development and testing purposes.

### Prerequisites

Ensure your Raspberry Pi is running Raspberry Pi OS and has the following installed:

*   A C compiler (e.g., GCC, usually pre-installed).
*   `libncurses-dev`: For the `curses` library.
    ```bash
    sudo apt-get update
    sudo apt-get install libncurses-dev
    ```
*   Initio Robot Platform libraries (refer to Initio documentation for installation).

### Installation

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/MuhammadHaseebRafique/CamCar-Red-Ball-Following-Robot-Car-Raspberry-Pi-Initio-Platform-
    cd CamCar-Red-Ball-Following-Robot-Car-Raspberry-Pi-Initio-Platform-
    ```

2.  **Compile the project:**
    The project includes a `Makefile` for easy compilation. If a `Makefile` is not present, you can use the `compile.sh` script or compile manually.
    ```bash
    make
    # Or if using compile.sh
    # chmod +x compile.sh
    # ./compile.sh
    ```

### Running the Robot

Execute the compiled program:

```bash
./camcar
```

Press `q` to quit the application.

## 🧠 Architecture / How It Works

The CamCar operates on a multi-threaded design to achieve real-time responsiveness:

1.  **Worker Thread (`worker` function):**
    *   Continuously captures frames from the Raspberry Pi camera.
    *   Performs image processing using `cameraSearchBlob` (from `detect_blob.h`) to identify and locate the largest red blob.
    *   Updates shared blob data (size, horizontal/vertical alignment) in a thread-safe manner using a mutex.

2.  **Main Thread (`camcar` function):**
    *   Monitors the shared blob data and sensor readings (IR and ultrasonic).
    *   **Obstacle Avoidance:** If IR sensors detect an obstacle or the ultrasonic sensor indicates proximity below `OBSTACLE_THRESHOLD`, the robot stops or moves backward.
    *   **Ball Following Logic:**
        *   If a sufficiently large red blob is detected (`BLOB_SIZE_THRESHOLD`):
            *   Adjusts steering (`initio_SpinLeft`, `initio_SpinRight`) based on the blob's horizontal alignment (`halign`).
            *   Adjusts speed (`initio_DriveForward`, `initio_DriveBackward`) based on the distance to the blob (`DIST_MIN`, `DIST_MAX`).
        *   If no blob is detected, the robot enters a search state, potentially spinning to find the ball.
    *   Updates the `curses`-based console display with current status.

## 🤝 Contributing

Contributions are welcome! Please feel free to fork the repository, make improvements, and submit pull requests. For major changes, please open an issue first to discuss what you would like to change.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 📧 Contact

**Muhammad Haseeb Rafique**

*   [GitHub Profile](https://github.com/MuhammadHaseebRafique)
*   [LinkedIn Profile](https://www.linkedin.com/in/muhammadhaseebrafique/)

Project Link: [https://github.com/MuhammadHaseebRafique/CamCar-Red-Ball-Following-Robot-Car-Raspberry-Pi-Initio-Platform-](https://github.com/MuhammadHaseebRafique/CamCar-Red-Ball-Following-Robot-Car-Raspberry-Pi-Initio-Platform-)
