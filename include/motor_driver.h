#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

class MotorDriver {
public:
    void begin();
    void setMotor(uint8_t ch, int16_t speed); // ch 0-3, speed -255..255
    void setAll(int16_t a, int16_t b, int16_t c, int16_t d);
    void forward(int16_t speed);
    void backward(int16_t speed);
    void turnLeft(int16_t speed);
    void turnRight(int16_t speed);
    void rotate(int16_t speed);
    void stop();
    void setPolarity(uint8_t ch, int8_t dir);

private:
    struct MotorPin {
        uint8_t gpioA, gpioB;
        uint8_t chanA, chanB;
    };
    MotorPin pins[4];
    int8_t polarity[4] = {1, 1, -1, -1};  // per-motor direction flip
    void rawSet(uint8_t ch, int16_t speed);
};

#endif
