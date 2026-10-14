#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// ── Motor H-Bridge (2 PWM pins per motor) ──
#define MOTOR_A_PWMA  14
#define MOTOR_A_PWMB  18

#define MOTOR_B_PWMA  19
#define MOTOR_B_PWMB  23

#define MOTOR_C_PWMA  25
#define MOTOR_C_PWMB  26

#define MOTOR_D_PWMA  27
#define MOTOR_D_PWMB  32

// ── Servo Arm ──
#define SERVO_ARM_L   4
#define SERVO_ARM_R   13

// ── OLED I2C (SSD1309 128×64) ──
#define OLED_SDA      21
#define OLED_SCL      22

// ── VC-02 Voice Module UART2 ──
#define VC02_RX       16
#define VC02_TX       17

// ── Reserved ──
// GPIO 19, 5, 15, 4 available for button/LED/buzzer

#endif
