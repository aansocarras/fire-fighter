// code generated after following this guide:  https://racheldebarros.com/esp32-projects/connect-your-game-controller-to-an-esp32/
#include <Arduino.h>
#include <Bluepad32.h>

#define LED 2
#define LEFT_FRONT_MOTOR_PIN_1 12
#define LEFT_FRONT_MOTOR_PIN_2 14
#define LEFT_FRONT_MOTOR_PIN_PWM 13

#define LEFT_BACK_MOTOR_PIN_PWM 27
#define LEFT_BACK_MOTOR_PIN_1 26
#define LEFT_BACK_MOTOR_PIN_2 25

// TODO: map these pins
#define RIGHT_FRONT_MOTOR_PIN_1 25
#define RIGHT_FRONT_MOTOR_PIN_2 33
#define RIGHT_FRONT_MOTOR_PIN_PWM 32

// TODO: map these pins
#define RIGHT_BACK_MOTOR_PIN_PWM 16
#define RIGHT_BACK_MOTOR_PIN_1 17
#define RIGHT_BACK_MOTOR_PIN_2 5

typedef enum {
    FORWARD,
    BACKWARD
} direction_e;

// Define the motor type
typedef enum {
    FRONT_LEFT,
    BACK_LEFT,
    FRONT_RIGHT,
    BACK_RIGHT
} motor_e;

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// This callback gets called any timd at the same time.
void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            ControllerProperties properties = ctl->getProperties();
            Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id, properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }

    if (!foundEmptySlot) {
        Serial.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController) {
        Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

// ========= SEE CONTROLLER VALUES IN SERIAL MONITOR ========= //

void dumpGamepad(ControllerPtr ctl) {
    Serial.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
        "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
        ctl->index(), // Controller Index
        ctl->dpad(), // D-pad
        ctl->buttons(), // bitmask of pressed buttons
        ctl->axisX(), // (-511 - 512) left X Axis
        ctl->axisY(), // (-511 - 512) left Y axis
        ctl->axisRX(), // (-511 - 512) right X axis
        ctl->axisRY(), // (-511 - 512) right Y axis
        ctl->brake(), // (0 - 1023): brake button
        ctl->throttle(), // (0 - 1023): throttle (AKA gas) button
        ctl->miscButtons(), // bitmask of pressed "misc" buttons
        ctl->gyroX(), // Gyro X
        ctl->gyroY(), // Gyro Y
        ctl->gyroZ(), // Gyro Z
        ctl->accelX(), // Accelerometer X
        ctl->accelY(), // Accelerometer Y
        ctl->accelZ() // Accelerometer Z
    );
}

void move_motor(motor_e motor, uint8_t val, direction_e dir) {
    switch (motor) {
    case FRONT_LEFT:
        analogWrite(LEFT_FRONT_MOTOR_PIN_PWM, val);
        digitalWrite(LEFT_FRONT_MOTOR_PIN_1, dir);
        digitalWrite(LEFT_FRONT_MOTOR_PIN_2, !dir);
        break;
    case BACK_LEFT:
        analogWrite(LEFT_BACK_MOTOR_PIN_PWM, val);
        digitalWrite(LEFT_BACK_MOTOR_PIN_1, dir);
        digitalWrite(LEFT_BACK_MOTOR_PIN_2, !dir);
        break;
    case FRONT_RIGHT:
        analogWrite(RIGHT_FRONT_MOTOR_PIN_PWM, val);
        digitalWrite(RIGHT_FRONT_MOTOR_PIN_1, dir);
        digitalWrite(RIGHT_FRONT_MOTOR_PIN_2, !dir);
        break;
    case BACK_RIGHT:
        analogWrite(RIGHT_BACK_MOTOR_PIN_PWM, val);
        digitalWrite(RIGHT_BACK_MOTOR_PIN_1, dir);
        digitalWrite(RIGHT_BACK_MOTOR_PIN_2, !dir);
        break;
    }
}
// ========= GAME CONTROLLER ACTIONS SECTION ========= //

void processGamepad(ControllerPtr ctl) {
    //== PS4 X button = 0x0001 ==//
    if (ctl->buttons() == 0x0001) {
        digitalWrite(LED, HIGH); // Turn the LED on
    }
    if (ctl->buttons() != 0x0001) {
        digitalWrite(LED, LOW); // Turn the LED on
    }

    //== PS4 Square button = 0x0004 ==//
    if (ctl->buttons() == 0x0004) {
        analogWrite(LEFT_FRONT_MOTOR_PIN_PWM, 100);
        digitalWrite(LEFT_FRONT_MOTOR_PIN_1, LOW);
        digitalWrite(LEFT_FRONT_MOTOR_PIN_2, HIGH);
    }
    if (ctl->buttons() != 0x0004) {
    }

    //== PS4 Triangle button = 0x0008 ==//
    if (ctl->buttons() == 0x0008) {
        digitalWrite(LEFT_FRONT_MOTOR_PIN_1, LOW);
        digitalWrite(LEFT_FRONT_MOTOR_PIN_2, LOW);
    }
    if (ctl->buttons() != 0x0008) {
    }

    //== PS4 Circle button = 0x0002 ==//
    if (ctl->buttons() == 0x0002) {
        analogWrite(LEFT_FRONT_MOTOR_PIN_PWM, 255);
        digitalWrite(LEFT_FRONT_MOTOR_PIN_1, HIGH);
        digitalWrite(LEFT_FRONT_MOTOR_PIN_2, LOW);
    }
    if (ctl->buttons() != 0x0002) {
    }

    //== PS4 Dpad UP button = 0x01 ==//
    if (ctl->buttons() == 0x01) {
        // code for when dpad up button is pushed
    }
    if (ctl->buttons() != 0x01) {
        // code for when dpad up button is released
    }

    //==PS4 Dpad DOWN button = 0x02==//
    if (ctl->buttons() == 0x02) {
        // code for when dpad down button is pushed
    }
    if (ctl->buttons() != 0x02) {
        // code for when dpad down button is released
    }

    //== PS4 Dpad LEFT button = 0x08 ==//
    if (ctl->buttons() == 0x08) {
        // code for when dpad left button is pushed
    }
    if (ctl->buttons() != 0x08) {
        // code for when dpad left button is released
    }

    //== PS4 Dpad RIGHT button = 0x04 ==//
    if (ctl->buttons() == 0x04) {
        // code for when dpad right button is pushed
    }
    if (ctl->buttons() != 0x04) {
        // code for when dpad right button is released
    }

    //== PS4 R1 trigger button = 0x0020 ==//
    if (ctl->buttons() == 0x0020) {
        // code for when R1 button is pushed
    }
    if (ctl->buttons() != 0x0020) {
        // code for when R1 button is released
    }

    //== PS4 R2 trigger button = 0x0080 ==//
    if (ctl->buttons() == 0x0080) {
        // code for when R2 button is pushed
    }
    if (ctl->buttons() != 0x0080) {
        // code for when R2 button is released
    }

    //== PS4 L1 trigger button = 0x0010 ==//
    if (ctl->buttons() == 0x0010) {
        // code for when L1 button is pushed
    }
    if (ctl->buttons() != 0x0010) {
        // code for when L1 button is released
    }

    //== PS4 L2 trigger button = 0x0040 ==//
    if (ctl->buttons() == 0x0040) {
        // code for when L2 button is pushed
    }
    if (ctl->buttons() != 0x0040) {
        // code for when L2 button is released
    }

    //== LEFT JOYSTICK - UP ==//
    if (ctl->axisY() <= -25) {
        // code for when left joystick is pushed up
        int value_remap = map(ctl->axisY(), -25, -520, 0, 255);
        move_motor(FRONT_LEFT, value_remap, FORWARD);
    }

    //== LEFT JOYSTICK - DOWN ==//
    else if (ctl->axisY() >= 25) {
        int value_remap = map(ctl->axisY(), 25, 520, 0, 255);
        move_motor(FRONT_LEFT, value_remap, BACKWARD);
    }

    //== LEFT JOYSTICK - NONE ==//
    else {
        move_motor(FRONT_LEFT, 0, FORWARD);
    }

    //== LEFT JOYSTICK - LEFT ==//
    if (ctl->axisX() <= -25) {
        // code for when left joystick is pushed left
    }

    //== LEFT JOYSTICK - RIGHT ==//
    if (ctl->axisX() >= 25) {
        // code for when left joystick is pushed right
    }

    //== LEFT JOYSTICK DEADZONE ==//
    if (ctl->axisY() > -25 && ctl->axisY() < 25 && ctl->axisX() > -25 && ctl->axisX() < 25) {
        // code for when left joystick is at idle
    }

    //== RIGHT JOYSTICK - X AXIS ==//
    if (ctl->axisRX()) {
        // code for when right joystick moves along x-axis
    }

    //== RIGHT JOYSTICK - Y AXIS ==//
    if (ctl->axisRY()) {
        // code for when right joystick moves along y-axis
    }
    dumpGamepad(ctl);
}

void processControllers() {
    for (auto myController : myControllers) {
        if (myController && myController->isConnected() && myController->hasData()) {
            if (myController->isGamepad()) {
                processGamepad(myController);
            } else {
                Serial.println("Unsupported controller");
            }
        }
    }
}

// Arduino setup function. Runs in CPU 1
void setup() {
    Serial.begin(115200);
    Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t* addr = BP32.localBdAddress();
    Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

    pinMode(LED, OUTPUT);
    pinMode(LEFT_FRONT_MOTOR_PIN_1, OUTPUT);
    pinMode(LEFT_FRONT_MOTOR_PIN_2, OUTPUT);
    pinMode(LEFT_FRONT_MOTOR_PIN_PWM, OUTPUT);
    pinMode(RIGHT_FRONT_MOTOR_PIN_1, OUTPUT);
    pinMode(RIGHT_FRONT_MOTOR_PIN_2, OUTPUT);
    pinMode(RIGHT_FRONT_MOTOR_PIN_PWM, OUTPUT);
    // enforce low
    digitalWrite(LED, LOW);
    digitalWrite(LEFT_FRONT_MOTOR_PIN_1, LOW);
    digitalWrite(LEFT_FRONT_MOTOR_PIN_2, LOW);
    digitalWrite(LEFT_FRONT_MOTOR_PIN_PWM, LOW);
    digitalWrite(RIGHT_FRONT_MOTOR_PIN_1, LOW);
    digitalWrite(RIGHT_FRONT_MOTOR_PIN_2, LOW);
    digitalWrite(RIGHT_FRONT_MOTOR_PIN_PWM, LOW);

    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedController, &onDisconnectedController);

    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But it might also fix some connection / re-connection issues.
    BP32.forgetBluetoothKeys();

    // Enables mouse / touchpad support for gamepads that support them.
    // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
    // - First one: the gamepad
    // - Second one, which is a "virtual device", is a mouse.
    // By default, it is disabled.
    BP32.enableVirtualDevice(false);
}

// Arduino loop function. Runs in CPU 1.
void loop() {
    // This call fetches all the controllers' data.
    // Call this function in your main loop.
    bool dataUpdated = BP32.update();
    if (dataUpdated)
        processControllers();

    delay(15);
}