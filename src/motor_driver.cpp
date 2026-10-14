#include "motor_driver.h"
#include "pin_config.h"

static const uint8_t MOTOR_PIN_MAP[4][2] = {
    {MOTOR_A_PWMA, MOTOR_A_PWMB},
    {MOTOR_B_PWMA, MOTOR_B_PWMB},
    {MOTOR_C_PWMA, MOTOR_C_PWMB},
    {MOTOR_D_PWMA, MOTOR_D_PWMB},
};

static const uint32_t PWM_FREQ = 5000;
static const uint8_t PWM_RES = 8;

void MotorDriver::begin() {
    uint8_t ch = 4;  // leave channels 0-3 for ESP32Servo
    for (int i = 0; i < 4; i++) {
        pins[i].gpioA = MOTOR_PIN_MAP[i][0];
        pins[i].gpioB = MOTOR_PIN_MAP[i][1];
        pins[i].chanA = ch;
        pins[i].chanB = ch + 1;

        ledcSetup(pins[i].chanA, PWM_FREQ, PWM_RES);
        ledcAttachPin(pins[i].gpioA, pins[i].chanA);
        ledcSetup(pins[i].chanB, PWM_FREQ, PWM_RES);
        ledcAttachPin(pins[i].gpioB, pins[i].chanB);

        ch += 2;
    }
}

void MotorDriver::setMotor(uint8_t ch, int16_t speed) {
    if (ch > 3) return;
    rawSet(ch, constrain(speed, -255, 255));
}

void MotorDriver::setAll(int16_t a, int16_t b, int16_t c, int16_t d) {
    setMotor(0, a);
    setMotor(1, b);
    setMotor(2, c);
    setMotor(3, d);
}

void MotorDriver::forward(int16_t speed) {
    setAll(speed, speed, speed, speed);
}

void MotorDriver::backward(int16_t speed) {
    setAll(-speed, -speed, -speed, -speed);
}

void MotorDriver::turnLeft(int16_t speed) {
    setAll(-speed, speed, -speed, speed);
}

void MotorDriver::turnRight(int16_t speed) {
    setAll(speed, -speed, speed, -speed);
}

void MotorDriver::rotate(int16_t speed) {
    setAll(-speed, speed, speed, -speed);
}

void MotorDriver::stop() {
    setAll(0, 0, 0, 0);
}

void MotorDriver::setPolarity(uint8_t ch, int8_t dir) {
    if (ch > 3) return;
    if (dir == 1 || dir == -1) polarity[ch] = dir;
}

void MotorDriver::rawSet(uint8_t ch, int16_t speed) {
    speed = speed * polarity[ch];  // apply per-motor polarity
    if (speed > 0) {
        ledcWrite(pins[ch].chanA, speed);
        ledcWrite(pins[ch].chanB, 0);
    } else if (speed < 0) {
        ledcWrite(pins[ch].chanA, 0);
        ledcWrite(pins[ch].chanB, -speed);
    } else {
        ledcWrite(pins[ch].chanA, 0);
        ledcWrite(pins[ch].chanB, 0);
    }
}
