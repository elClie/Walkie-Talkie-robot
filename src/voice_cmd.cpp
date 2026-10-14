#include "voice_cmd.h"
#include "pin_config.h"

void VoiceCommand::setDebug(bool on)
{
    debugEnabled = on;
}

void VoiceCommand::begin(uint32_t baud)
{
    Serial2.begin(baud, SERIAL_8N1, VC02_RX, VC02_TX);
    reset();
}

bool VoiceCommand::hasCommand()
{
    while (Serial2.available() && len < BUF_SIZE)
    {
        buf[len++] = Serial2.read();
    }

    if (debugEnabled && len > 0)
    {
        Serial.print("[UART2] ");
        for (uint8_t i = 0; i < len; i++)
        {
            Serial.print(buf[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }

    VoiceCmd cmd = parseBuffer();
    if (cmd != VoiceCmd::NONE)
    {
        pendingCmd = cmd;
        if (debugEnabled)
        {
            Serial.print("debuging in voice: ");
            Serial.println((int)cmd);
        }
        reset();
        return true;
    }

    if (len >= BUF_SIZE)
    {
        flush();
    }

    return false;
}

VoiceCmd VoiceCommand::getCommand()
{
    VoiceCmd cmd = pendingCmd;
    pendingCmd = VoiceCmd::NONE;
    return cmd;
}

VoiceCmd VoiceCommand::parseBuffer()
{
    if (len == 0)
        return VoiceCmd::NONE;

    switch (buf[0])
    {
    case 0x00:
        return VoiceCmd::WAKE;

    case 0x01:
        if (len < 2)
            return VoiceCmd::NONE;
        switch (buf[1])
        {
        case 0x01:
            return VoiceCmd::FORWARD;
        case 0x02:
            return VoiceCmd::BACKWARD;
        case 0x03:
            return VoiceCmd::LEFT;
        case 0x04:
            return VoiceCmd::RIGHT;
        case 0x05:
            return VoiceCmd::STOP;
        default:
            return VoiceCmd::UNKNOWN;
        }

    case 0x02:
        if (len < 2)
            return VoiceCmd::NONE;
        switch (buf[1])
        {
        case 0x01:
            return VoiceCmd::HI;
        case 0x02:
            return VoiceCmd::HAPPY;
        case 0x03:
            return VoiceCmd::SAD;
        case 0x04:
            return VoiceCmd::ANGRY;
        default:
            return VoiceCmd::UNKNOWN;
        }

    case 0x03:
        if (len < 2)
            return VoiceCmd::NONE;
        switch (buf[1])
        {
        case 0x01:
            return VoiceCmd::ARM_UP;
        case 0x02:
            return VoiceCmd::SLEEP;
        case 0x03:
            return VoiceCmd::DANCE;
        default:
            return VoiceCmd::UNKNOWN;
        }

    default:
        return VoiceCmd::UNKNOWN;
    }
}

void VoiceCommand::reset()
{
    len = 0;
}

void VoiceCommand::flush()
{
    memset(buf, 0, BUF_SIZE);
    len = 0;
}
