# CamCar-Red-Ball-Following-Robot-Car-Raspberry-Pi-Initio-Platform-
A classic computer-vision-controlled robot car that uses the Raspberry Pi camera to detect and follow a bright red ball in real time.
Features

Live colour-blob tracking using the lightweight QuickBlob library
Real-time JPEG capture with raspistill (200×200 px, high frame-rate mode)
Simple but robust red-ball filter (works reliably indoors)
Proportional turning to keep the ball centred (halign ±0.15 threshold)
Ultrasonic distance control to maintain a safe distance (60-100 cm)
Obstacle avoidance using left/right IR sensors
Multi-threaded design: one thread continuously grabs and analyses frames, main thread handles motor control and UI
Ncurses-based on-screen debugging (blob size, alignment, distance, state)

Hardware

Initio 4WD robot chassis (or any car using the Initio library)
Raspberry Pi (2/3/4) with Pi Camera
Ultrasonic sensor (HC-SR04) on the front
Two IR obstacle sensors (left & right)

Key files

camcar.c – main control loop and state machine
detect_blob.c/h – camera capture + interface to QuickBlob
quickblob.c/h – fast incremental connected-component labelling (LGPL)
Makefile – native and cross-compilation targets

How to build & run (on the Pi)
Bashmake          # native compile
sudo ./camcar # run (needs root for wiringPi/initio)
Place a bright red ball in front of the car and watch it follow while keeping a safe distance and avoiding obstacles!
