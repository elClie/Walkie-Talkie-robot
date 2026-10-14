#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Arduino.h>
#include <U8g2lib.h>
#include "face_data.h"

struct FaceEntry {
    const char* name;
    uint8_t* bitmap;   // pointer to face buffer
};

class OLEDDisplay {
public:
    OLEDDisplay();
    void begin();
    bool setFace(const char* name);
    bool setFace(FaceID id);
    uint8_t addFace(const char* name, uint8_t* bitmap);
    void blink();                               // quick eye close/open
    void animate(FaceID frames[], uint8_t count, unsigned long msPerFrame);
    void update();                              // call in loop
    void clear();

private:
    U8G2_SSD1309_128X64_NONAME2_F_HW_I2C u8g2;
    static const uint8_t MAX_FACES = 16;
    FaceEntry faces[MAX_FACES];
    uint8_t faceCount = 0;
    uint8_t currentFaceIdx = 0;

    // blink state
    unsigned long blinkTimer = 0;
    bool blinking = false;
    static const unsigned long BLINK_INTERVAL = 4000;
    static const unsigned long BLINK_DURATION = 150;

    // animate state
    static const uint8_t ANIM_MAX = 8;
    bool animating = false;
    FaceID animFrames[ANIM_MAX];
    uint8_t animFrameCount = 0;
    uint8_t animCurrentFrame = 0;
    unsigned long animMsPerFrame = 0;
    unsigned long animTimer = 0;

    void drawBuffer(const uint8_t* fb);
    int8_t findFace(const char* name);
};

#endif
