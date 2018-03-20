/*
* Sample code to control a two wheel drive's robot movement.
*/

#include <SoftwareSerial.h>
#include <ArduinoCommander.h>


// CONSTANTS

// pins
const int BLUETOOTH_TX = 8;  // TX-O pin of bluetooth mate, Arduino D2
const int BLUETOOTH_RX = 7;  // RX-I pin of bluetooth mate, Arduino D3
const int LEFT_EN = 6; // PWM pin required
const int RIGHT_EN = 5; // PWM pin required
const int A_1 = 13;
const int A_2 = 12;
const int A_3 = 4;
const int A_4 = 2;

// ranges
const int CONTROL_MIN = 0;
const int CONTROL_MAX = 99;
const int CONTROL_MID = CONTROL_MAX / 2;
const int THROTTLE_MIN = 0;
const int X_ANGLE_MIN = -180;
const int X_ANGLE_MAX = 180;
const int Y_ANGLE_MIN = -90;
const int Y_ANGLE_MAX = 90;

// thresholds
const int TURN_THRESHOLD = 0;
const int MOVE_THRESHOLD = 0;

// calibration variables
const int RIGHT_OFFSET = 0;
const int LEFT_OFFSET = 0;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoCommander phone(bluetooth);

void move(int throttle, int steering) {
    // moves the robot based on the throttle and steering variables

    if (abs(throttle - CONTROL_MID) < MOVE_THRESHOLD) {
        goForward(0, 0); // don't move
    }
    else if (throttle > CONTROL_MID) {
        int spd = map(throttle, CONTROL_MID, CONTROL_MAX, THROTTLE_MIN, 255);
        goForward(spd, steering);
    }
    else {
        int spd = map(throttle, CONTROL_MID, -CONTROL_MIN, THROTTLE_MIN, 255);
        goBackward(spd, steering);
    }
}

void goForward(int throttle, int steering) {
    digitalWrite(A_1, LOW);
    digitalWrite(A_2, HIGH);
    digitalWrite(A_4, LOW);
    digitalWrite(A_3, HIGH);

    int leftThrottle, rightThrottle;
    if (abs(steering - CONTROL_MID) > TURN_THRESHOLD) {
        int i;
        if (steering > CONTROL_MID) {
            // turn right4
            int mappedSteering = map(steering, CONTROL_MID, CONTROL_MAX, CONTROL_MIN, CONTROL_MID);
            rightThrottle = throttle*((double)(CONTROL_MID - mappedSteering) / CONTROL_MID);
            leftThrottle = throttle;
        }
        else {
            // turn left
            int mappedSteering = map(steering, CONTROL_MID, CONTROL_MIN, CONTROL_MIN, CONTROL_MID);
            rightThrottle = throttle;
            leftThrottle = throttle*((double)(CONTROL_MID - mappedSteering) / CONTROL_MID);
        }
    }
    else {
        int i;
        // go straight
        leftThrottle = throttle;
        rightThrottle = throttle;
    }

    analogWrite(LEFT_EN, leftThrottle - LEFT_OFFSET);
    analogWrite(RIGHT_EN, rightThrottle - RIGHT_OFFSET);
}

void goBackward(int throttle, int steering) {
    digitalWrite(A_1, HIGH);
    digitalWrite(A_2, LOW);
    digitalWrite(A_4, HIGH);
    digitalWrite(A_3, LOW);

    int leftThrottle, rightThrottle;
    if (abs(steering - CONTROL_MID) > TURN_THRESHOLD) {
        int i;
        if (steering > CONTROL_MID) {
            // turn right4
            int mappedSteering = map(steering, CONTROL_MID, CONTROL_MAX, CONTROL_MIN, CONTROL_MID);
            rightThrottle = throttle*((double)(CONTROL_MID - mappedSteering) / CONTROL_MID);
            leftThrottle = throttle;
        }
        else {
            // turn left
            int mappedSteering = map(steering, CONTROL_MID, CONTROL_MIN, CONTROL_MIN, CONTROL_MID);
            rightThrottle = throttle;
            leftThrottle = throttle*((double)(CONTROL_MID - mappedSteering) / CONTROL_MID);
        }
    }
    else {
        int i;
        // go straight
        leftThrottle = throttle;
        rightThrottle = throttle;
    }

    analogWrite(LEFT_EN, leftThrottle - LEFT_OFFSET);
    analogWrite(RIGHT_EN, rightThrottle - RIGHT_OFFSET);
}

void setup() {
    // the setup function runs once when you press reset or power the board

    // Begin the serial monitor at 9600bps
    Serial.begin(9600);

    // Start bluetooth serial at 9600
    bluetooth.begin(9600);

    Serial.println("setup complete");
}

void loop() {
    // the loop function runs over and over again until power down or reset

    // check for incoming signal
    phone.checkBluetooth();

    // throttle and steering values goes from 0 to 99.

    // 0 throttle corresponds to maximum negative and 99 corresponds to maximum positive throttle.
    int throttle = phone.getThrottle();

    // 0 steering corresponds to maximum left and 99 corresponds to maximum right
    int steering = phone.getSteering();

    // move the robot
    move(throttle, steering);

}
