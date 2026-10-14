#include <Arduino.h>
#include "pin_config.h"
#include "motor_driver.h"
#include "servo_arm.h"
#include "oled_display.h"
#include "voice_cmd.h"

MotorDriver motors;
ServoArm arm;
OLEDDisplay display;
VoiceCommand voice;

enum RobotState : uint8_t
{
    STATE_IDLE,
    STATE_MOVING,
    STATE_ARM_BUSY,
    STATE_EXPRESSING,
    STATE_DANCING,
    STATE_SLEEP
};

RobotState state = STATE_IDLE;

static unsigned long danceTimer = 0;
static uint8_t danceStep = 0;

void handleVoiceCommand(VoiceCmd cmd)
{
    switch (cmd)
    {
    case VoiceCmd::WAKE:
        display.setFace(FACE_NEUTRAL);
        state = STATE_IDLE;
        break;

    case VoiceCmd::FORWARD:
        motors.forward(-200);
        display.setFace(FACE_HAPPY);
        state = STATE_MOVING;
        break;

    case VoiceCmd::BACKWARD:
        motors.backward(-200);
        display.setFace(FACE_SAD);
        state = STATE_MOVING;
        break;

    case VoiceCmd::LEFT:
        motors.turnLeft(200);
        state = STATE_MOVING;
        break;

    case VoiceCmd::RIGHT:
        motors.turnRight(200);
        state = STATE_MOVING;
        break;

    case VoiceCmd::STOP:
        motors.stop();
        arm.setAngle(0);
        display.setFace(FACE_NEUTRAL);
        state = STATE_IDLE;
        break;

    case VoiceCmd::HI:
    {
        FaceID wave[] = {FACE_WAVE_1, FACE_WAVE_2, FACE_WAVE_3};
        display.animate(wave, 3, 250);
        state = STATE_EXPRESSING;
        break;
    }

    case VoiceCmd::HAPPY:
        display.setFace(FACE_HAPPY);
        state = STATE_EXPRESSING;
        break;

    case VoiceCmd::SAD:
        display.setFace(FACE_SAD);
        state = STATE_EXPRESSING;
        break;

    case VoiceCmd::ANGRY:
        display.setFace(FACE_ANGRY);
        state = STATE_EXPRESSING;
        break;

    case VoiceCmd::ARM_UP:
        arm.sweep(arm.currentAngle(), 30, 600);
        state = STATE_ARM_BUSY;
        break;

    case VoiceCmd::SLEEP:
        motors.stop();
        arm.center();
        display.setFace(FACE_SLEEP);
        state = STATE_SLEEP;
        break;

    case VoiceCmd::DANCE:
        danceStep = 0;
        danceTimer = millis();
        state = STATE_DANCING;
        break;

    default:
        break;
    }
}

void updateDance()
{
    if (state != STATE_DANCING)
        return;

    unsigned long now = millis();
    if (now - danceTimer < 300)
        return;
    danceTimer = now;

    switch (danceStep)
    {
    case 0:
        motors.rotate(180);
        display.setFace(FACE_HAPPY);
        break;
    case 1:
        motors.rotate(-180);
        break;
    case 2:
        motors.forward(200);
        break;
    case 3:
        motors.backward(200);
        break;
    case 4:
        motors.stop();
        arm.sweep(30, 150, 400);
        state = STATE_EXPRESSING;
        return;
    }
    danceStep++;
    if (danceStep > 4)
        danceStep = 0;
}

void printHelp();
void handleSerial();
void testServo(int angle);

void setup()
{
    Serial.begin(115200);
    delay(100);

    motors.begin();
    arm.begin();
    display.begin();
    voice.begin();
    voice.setDebug(true);

    Serial.println("[ROBOT] Minirobot ready.");
}

void loop()
{
    handleSerial();

    if (voice.hasCommand())
    {
        VoiceCmd cmd = voice.getCommand();
        if (cmd != VoiceCmd::NONE && cmd != VoiceCmd::UNKNOWN)
        {
            handleVoiceCommand(cmd);
            Serial.print("VC02: ");
            Serial.println((int)cmd, HEX);
        }
    }

    arm.update();
    display.update();
    updateDance();

    if (state == STATE_ARM_BUSY && !arm.isSweeping())
    {
        state = STATE_IDLE;
    }
}
void handleSerial()
{
    if (!Serial.available())
        return;

    char cmd = Serial.read();

    switch (cmd)
    {

    case 's':
    case 'S':
        testServo(Serial.parseInt());
        break;
    case 'a':
        Serial.println('arm up');
        handleVoiceCommand(VoiceCmd::ARM_UP);
        break;

    case 'h':
    case 'H':
        printHelp();
        break;
    }
}
void printHelp()
{
    Serial.println("Commands:");
    Serial.println("  t       - tare (zero) the scale");
    Serial.println("  c       - calibrate (enter known weight via serial)");
    Serial.println("  s<ang>  - servo test (e.g. s90)");
    Serial.println("  m<pos>  - stepper move to absolute position (e.g. m1600)");
    Serial.println("  r<del>  - stepper move relative (e.g. r200 or r-200)");
    Serial.println("  d       - toggle debug mode (stops load cell polling)");
    Serial.println("  h       - print this help");
}
void testServo(int angle)
{
    angle = constrain(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    arm.setAngle(angle);
    Serial.print("Servo -> ");
    Serial.println(angle);
}