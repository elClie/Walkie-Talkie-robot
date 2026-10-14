#ifndef FACE_DATA_H
#define FACE_DATA_H

#include <Arduino.h>

#define FACE_W 128
#define FACE_H 64
#define FACE_BYTES ((FACE_W * FACE_H) / 8)

enum FaceID : uint8_t {
    FACE_NEUTRAL,
    FACE_HAPPY,
    FACE_SAD,
    FACE_ANGRY,
    FACE_SURPRISED,
    FACE_BLINK,
    FACE_SLEEP,
    FACE_WAVE_1,
    FACE_WAVE_2,
    FACE_WAVE_3,
    FACE_COUNT
};

void generateFaces();
uint8_t* getFaceBuffer(FaceID id);

#endif
