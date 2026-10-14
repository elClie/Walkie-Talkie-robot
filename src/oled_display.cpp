#include <string.h>
#include "oled_display.h"
#include "face_data.h"

OLEDDisplay::OLEDDisplay() : u8g2(U8G2_R0, U8X8_PIN_NONE) {}

void OLEDDisplay::begin() {
    u8g2.begin();
    u8g2.setContrast(255);

    // Register built-in faces
    generateFaces();
    for (uint8_t i = 0; i < FACE_COUNT; i++) {
        addFace("", getFaceBuffer((FaceID)i));
    }

    // Set name for each built-in face
    static const char* faceNames[FACE_COUNT] = {
        "neutral", "happy", "sad", "angry",
        "surprised", "blink", "sleep",
        "wave1", "wave2", "wave3"
    };
    for (uint8_t i = 0; i < FACE_COUNT; i++) {
        faces[i].name = faceNames[i];
    }

    currentFaceIdx = FACE_NEUTRAL;
    drawBuffer(getFaceBuffer(FACE_NEUTRAL));
    blinkTimer = millis();
}

bool OLEDDisplay::setFace(const char* name) {
    int8_t idx = findFace(name);
    if (idx < 0) return false;
    currentFaceIdx = idx;
    drawBuffer(faces[idx].bitmap);
    return true;
}

bool OLEDDisplay::setFace(FaceID id) {
    if (id >= faceCount) return false;
    currentFaceIdx = id;
    drawBuffer(faces[id].bitmap);
    return true;
}

uint8_t OLEDDisplay::addFace(const char* name, uint8_t* bitmap) {
    if (faceCount >= MAX_FACES) return 0;
    uint8_t id = faceCount++;
    faces[id].name = name;
    faces[id].bitmap = bitmap;
    return id;
}

void OLEDDisplay::blink() {
    blinking = true;
    drawBuffer(faces[FACE_BLINK].bitmap);
}

void OLEDDisplay::animate(FaceID frames[], uint8_t count, unsigned long msPerFrame) {
    if (count < 2 || count > ANIM_MAX) return;
    animating = true;
    memcpy(animFrames, frames, count * sizeof(FaceID));
    animFrameCount = count;
    animCurrentFrame = 0;
    animMsPerFrame = msPerFrame;
    animTimer = millis();
    drawBuffer(faces[frames[0]].bitmap);
}

void OLEDDisplay::update() {
    unsigned long now = millis();

    // Blink timer
    if (!animating) {
        if (blinking) {
            if (now - blinkTimer >= BLINK_DURATION) {
                blinking = false;
                drawBuffer(faces[currentFaceIdx].bitmap);
                blinkTimer = now;
            }
        } else {
            if (now - blinkTimer >= BLINK_INTERVAL) {
                blinkTimer = now;
                blink();
            }
        }
    }

    // Animation timer
    if (animating) {
        if (now - animTimer >= animMsPerFrame) {
            animTimer = now;
            animCurrentFrame++;
            if (animCurrentFrame >= animFrameCount) {
                animating = false;
                drawBuffer(faces[currentFaceIdx].bitmap);
                blinkTimer = now;
                return;
            }
            drawBuffer(faces[animFrames[animCurrentFrame]].bitmap);
        }
    }
}

void OLEDDisplay::clear() {
    u8g2.clearBuffer();
    u8g2.sendBuffer();
}

void OLEDDisplay::drawBuffer(const uint8_t* fb) {
    u8g2.clearBuffer();
    memcpy(u8g2.getBufferPtr(), fb, FACE_BYTES);
    u8g2.sendBuffer();
}

int8_t OLEDDisplay::findFace(const char* name) {
    for (uint8_t i = 0; i < faceCount; i++) {
        if (strcmp(faces[i].name, name) == 0) return i;
    }
    return -1;
}
