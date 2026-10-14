#ifndef VOICE_CMD_H
#define VOICE_CMD_H

#include <Arduino.h>

enum class VoiceCmd : uint8_t {
    NONE,
    WAKE,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    STOP,
    HI,
    HAPPY,
    SAD,
    ANGRY,
    ARM_UP,
    SLEEP,
    DANCE,
    UNKNOWN
};

class VoiceCommand {
public:
    void begin(uint32_t baud = 115200);
    bool hasCommand();
    VoiceCmd getCommand();
    void setDebug(bool on);

private:
    static const uint8_t BUF_SIZE = 16;
    uint8_t buf[BUF_SIZE];
    uint8_t len = 0;
    VoiceCmd pendingCmd = VoiceCmd::NONE;
    bool debugEnabled = false;

    VoiceCmd parseBuffer();
    void reset();
    void flush();
};

#endif
