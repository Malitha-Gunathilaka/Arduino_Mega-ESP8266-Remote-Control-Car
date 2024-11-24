#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

// Motor control constants and objects
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// Define pins for the ultrasonic sensor
#define TRIGGER_PIN  22
#define ECHO_PIN     23
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

int speed = 255;
bool objectDetected = false;

// Servo and IR sensor constants and objects
const int irSensorPin = 2;
const int servoPin = 9;
const int boxes = 10;
const int shuttlecocksPerBox = 10;
const int degreePerBox = 36;

int shuttlecockCount = 0;
int totalShuttlecockCount = 0; // Total shuttlecock count
int boxCount = 0;
bool shuttlecockPresent = false;

Servo myServo;
int currentAngle = 0;

// Automatic movement constants
const int forwardTimeLong = 3000; // Time to move forward 1 meters (1m =3s)
const int forwardTimeShort = 500; // Time to move forward 0.17 meters 
const int turnTime = 550; // Time to turn 90 degrees 
const int numPasses = 4; // Number of passes to cover the area

bool autoMode = false; // Flag to switch between manual and automatic mode

void setup() {
  // Motor control setup
  stopMotors();
  Serial3.begin(9600); // Communication with ESP8266 using Serial3
  setMotorSpeed(speed);

  // Servo and IR sensor setup
  pinMode(irSensorPin, INPUT);
  myServo.attach(servoPin);
  myServo.write(currentAngle);
  Serial.begin(9600); // For debugging on the Serial Monitor
  Serial.println("System Initialized");
}

void checkShuttlecockCount() {
  int irSensorValue = digitalRead(irSensorPin);
  if (irSensorValue == HIGH && !shuttlecockPresent) {
    shuttlecockPresent = true;  // Mark shuttlecock as detected
    shuttlecockCount++;  // Increment shuttlecock count
    totalShuttlecockCount++; // Increment total shuttlecock count
    sendShuttlecockCount();  // Send updated count
    
    // Check if the current box is full
    if (shuttlecockCount == shuttlecocksPerBox) {
      shuttlecockCount = 0; // Reset the shuttlecock count for the next box
      boxCount++; // Increment the box count
      
      // Rotate the servo to the next box
      currentAngle += degreePerBox;
      if (currentAngle > 360) { // Ensure the angle doesn't exceed 360 degrees
        currentAngle = 360;
      }
      myServo.write(currentAngle);

       // Check if all boxes are full
      if (boxCount == boxes) {
        Serial.println("All boxes are full. System will halt.");
        while (true) {} // Stop the system indefinitely
      } else {
        delay(1000); // Wait for the servo to rotate to the next box
      }
    }
  } else if (irSensorValue == LOW) {
    shuttlecockPresent = false; // Reset detection state
  }
}

void runManualMode() {
  // Motor control loop logic
  int distance = sonar.ping_cm();
  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance > 0 && distance < 5) {
    objectDetected = true;
    stopMotors();
    Serial3.println('O');
  } else {
    objectDetected = false;
  }

  if (Serial3.available() > 0) {
    String input = Serial3.readStringUntil('\n');
    Serial.print("Received command: ");
    Serial.println(input);
    
    // Trim any whitespace or newline characters from the input
    input.trim();
    Serial.print("Trimmed command: ");
    Serial.println(input);

    if (input.startsWith("SPEED:")) {
        speed = input.substring(6).toInt();
        setMotorSpeed(speed);
        Serial.print("Speed set to: ");
        Serial.println(speed);
    } else if (input == "AUTO") {
        autoMode = true;
        Serial.println("Switched to automatic mode");
        Serial.print("Auto mode flag: ");
        Serial.println(autoMode);
    } else if (input == "MANUAL") {
        autoMode = false;
        Serial.println("Switched to manual mode");
        Serial.print("Auto mode flag: ");
        Serial.println(autoMode);
    } else {
        char command = input.charAt(0);
        Serial.print("Executing command: ");
        Serial.println(command);
        switch (command) {
            case 'F':
                if (objectDetected) {
                    Serial3.println('O');
                } else {
                    moveForward();
                }
                break;
            case 'B':
                moveBackward();
                break;
            case 'L':
                turnLeft();
                break;
            case 'R':
                turnRight();
                break;
            case 'S':
                stopMotors();
                break;
            default:
                Serial.println("Unknown command");
                break;
        }
    }
  }

  // Call shuttlecock counting function
  checkShuttlecockCount();

  delay(200);
}

void runAutoMode() {
    Serial.println("Starting automatic mode sequence");
    for (int i = 0; i < numPasses; i++) {
        // Move forward for 4 meters
        Serial.print("Pass ");
        Serial.print(i + 1);
        Serial.println(": Moving forward for 1 meter...");
        moveForward();
        for (int j = 0; j < forwardTimeLong / 200; j++) {
            delay(200);
            checkShuttlecockCount();
        }

        // Alternate turns after each pass
        if (i < numPasses - 1) { // Avoid turning after the last pass
            if (i % 2 == 0) {
                Serial.println("Turning right 90 degrees...");
                turnRight();
                for (int j = 0; j < turnTime / 200; j++) {
                    delay(200);
                    checkShuttlecockCount();
                }

                Serial.println("Moving sideways for 0.17 meters...");
                moveForward();
                for (int j = 0; j < forwardTimeShort / 200; j++) {
                    delay(200);
                    checkShuttlecockCount();
                }

                Serial.println("Turning right 90 degrees to align for the next pass...");
                turnRight();
                for (int j = 0; j < turnTime / 200; j++) {
                    delay(200);
                    checkShuttlecockCount();
                }
            } else {
                Serial.println("Turning left 90 degrees...");
                turnLeft();
                for (int j = 0; j < turnTime / 200; j++) {
                    delay(200);
                    checkShuttlecockCount();
                }

                Serial.println("Moving sideways for 0.17 meters...");
                moveForward();
                for (int j = 0; j < forwardTimeShort / 200; j++) {
                    delay(200);
                    checkShuttlecockCount();
                }

                Serial.println("Turning left 90 degrees to align for the next pass...");
                turnLeft();
                for (int j = 0; j < turnTime / 200; j++) {
                    delay(200);
                    checkShuttlecockCount();
                }
            }
        }
    }

    // Stop the robot
    Serial.println("Finished covering the area. Stopping the robot.");
    stopMotors();
    autoMode = false; // Reset autoMode flag after completing the sequence
}

void loop() {
    Serial.print("Auto mode flag: ");
    Serial.println(autoMode);
    if (autoMode) {
        Serial.println("Running in automatic mode");
        runAutoMode();
    } else {
        Serial.println("Running in manual mode");
        runManualMode();
    }
}

// Motor control functions
void executeCommand(char command) {
  switch (command) {
    case 'B':
      moveBackward();
      break;
    case 'L':
      turnLeft();
      break;
    case 'R':
      turnRight();
      break;
    case 'S':
      stopMotors();
      break;
  }
}

void moveForward() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void moveBackward() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void turnLeft() {
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}

void turnRight() {
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
}

void stopMotors() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void setMotorSpeed(int speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor3.setSpeed(speed);
  motor4.setSpeed(speed);
}

void sendShuttlecockCount() {
  Serial3.print("TOTAL_SHUTTLE:");
  Serial3.println(totalShuttlecockCount); // Send total shuttlecock count only
}