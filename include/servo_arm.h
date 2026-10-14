#ifndef SERVO_ARM_H
#define SERVO_ARM_H

#include <Arduino.h>
#include <ESP32Servo.h>

#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180
#define SERVO_NEUTRAL 90

class ServoArm {
public:
    void begin(int minUs = 500, int maxUs = 2400);
    void setAngle(int16_t angle);      // 0-180, both servos in sync
    void setTilt(int16_t offset);      // ±30 differential tilt
    void sweep(int16_t from, int16_t to, unsigned long ms = 500);
    void center();
    void update();                      // call in loop for sweep animation
    bool isSweeping();
    int16_t currentAngle();
    int16_t currentTilt();

private:
    Servo servoLeft;
    Servo servoRight;
    int16_t angle = 90;
    int16_t tilt = 0;

    // sweep state
    bool sweeping = false;
    int16_t sweepFrom, sweepTo;
    unsigned long sweepDuration;
    unsigned long sweepStart;
};

#endif
