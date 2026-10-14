#include "servo_arm.h"
#include "pin_config.h"

void ServoArm::begin(int minUs, int maxUs) {
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    servoLeft.setPeriodHertz(50);
    servoRight.setPeriodHertz(50);
    servoLeft.attach(SERVO_ARM_L, minUs, maxUs);
    servoRight.attach(SERVO_ARM_R, minUs, maxUs);
    setAngle(0);
}

void ServoArm::setAngle(int16_t newAngle) {
    angle = constrain(newAngle, 0, 180);
    int16_t l = angle + tilt;
    int16_t r = (180 - angle) + tilt;
    servoLeft.write(constrain(l, 0, 180));
    servoRight.write(constrain(r, 0, 180));
}

void ServoArm::setTilt(int16_t offset) {
    tilt = constrain(offset, -30, 30);
    setAngle(angle);
}

void ServoArm::center() {
    setAngle(90);
    tilt = 0;
}

void ServoArm::sweep(int16_t from, int16_t to, unsigned long ms) {
    sweepFrom = constrain(from, 0, 180);
    sweepTo = constrain(to, 0, 180);
    sweepDuration = ms;
    sweepStart = millis();
    sweeping = true;
}

void ServoArm::update() {
    if (!sweeping) return;
    unsigned long elapsed = millis() - sweepStart;
    if (elapsed >= sweepDuration) {
        setAngle(sweepTo);
        sweeping = false;
        return;
    }
    float t = (float)elapsed / sweepDuration;
    int16_t pos = sweepFrom + (sweepTo - sweepFrom) * t;
    setAngle(pos);
}

bool ServoArm::isSweeping() { return sweeping; }
int16_t ServoArm::currentAngle() { return angle; }
int16_t ServoArm::currentTilt() { return tilt; }
