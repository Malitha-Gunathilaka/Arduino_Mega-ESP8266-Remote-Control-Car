# Arduino_Mega+ESP8266 Remote Control Car

This project involves creating a remote-controlled car using an Arduino Mega and an ESP8266 module. The car is designed to navigate and count shuttlecocks, making it ideal for applications such as sports training or inventory management. The project leverages various sensors and components to achieve its functionality.

## Key Features
- **Motor Control**: The car uses four DC motors controlled via the AFMotor library to move forward, backward, and turn left or right.
- **Ultrasonic Sensor**: An ultrasonic sensor (NewPing library) is used to detect obstacles and prevent collisions.
- **Servo Motor**: A servo motor is used to rotate a mechanism that counts and stores shuttlecocks in different boxes.
- **IR Sensor**: An IR sensor detects the presence of shuttlecocks.
- **Wi-Fi Connectivity**: The ESP8266 module provides Wi-Fi connectivity, allowing remote control via a web interface.
- **Manual and Automatic Modes**: The car can be operated in manual mode via remote commands or in automatic mode to cover a predefined area.
- **Shuttlecock Counting**: The system counts shuttlecocks and stores them in boxes, rotating the servo motor to switch boxes when one is full.

## Components Used
- Arduino Mega
- ESP8266 Wi-Fi module
- AFMotor shield
- Ultrasonic sensor (HC-SR04)
- Servo motor
- IR sensor
- DC motors

## Software
- **Arduino Sketch (mega.ino)**: Handles motor control, sensor readings, and shuttlecock counting.
- **ESP8266 Sketch (wifi.ino)**: Manages Wi-Fi connectivity, web server, and communication with the Arduino Mega.

## How It Works
1. **Setup**: The Arduino Mega initializes the motors, sensors, and servo. The ESP8266 connects to a Wi-Fi network and starts a web server.
2. **Manual Mode**: Commands are sent from a web interface to control the car's movement and speed. The car stops if an obstacle is detected.
3. **Automatic Mode**: The car follows a predefined path, counting shuttlecocks and storing them in boxes. It stops when all boxes are full.
4. **Web Interface**: Users can control the car and switch between manual and automatic modes via a web interface hosted by the ESP8266.

## Getting Started

### Prerequisites
- Arduino IDE
- ESP8266 board package installed in Arduino IDE
- AFMotor library
- NewPing library
- Servo library

### Installation
1. **Clone the repository**:
    ```sh
    git clone https://github.com/yourusername/Arduino_Mega-ESP8266-Remote-Control-Car.git
    cd Arduino_Mega-ESP8266-Remote-Control-Car
    ```

2. **Upload the Arduino Sketch**:
    - Open `mega.ino` in the Arduino IDE.
    - Select the correct board and port for your Arduino Mega.
    - Upload the sketch to the Arduino Mega.

3. **Upload the ESP8266 Sketch**:
    - Open `wifi.ino` in the Arduino IDE.
    - Select the correct board and port for your ESP8266 module.
    - Upload the sketch to the ESP8266.

### Usage
1. **Power on the Arduino Mega and ESP8266 module**.
2. **Connect to the Wi-Fi network** specified in the `wifi.ino` file.
3. **Open a web browser** and navigate to the IP address printed in the Serial Monitor to access the control interface.
4. **Use the web interface** to control the car and switch between manual and automatic modes.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments
- Libraries used: AFMotor, NewPing, Servo, ESP8266WiFi, ESP8266WebServer
- Special thanks to the open-source community for providing the tools and libraries that made this project possible.
