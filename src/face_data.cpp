#include "face_data.h"
#include <string.h>

// ── Framebuffer helpers (vertical byte packing, U8g2 convention) ──

static void setPixel(uint8_t* fb, int x, int y) {
    if (x < 0 || x >= FACE_W || y < 0 || y >= FACE_H) return;
    int page = y / 8;
    int bit = y % 8;
    fb[x + page * FACE_W] |= (1 << bit);
}

static void clearFb(uint8_t* fb) {
    memset(fb, 0, FACE_BYTES);
}

static void drawLine(uint8_t* fb, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    while (true) {
        setPixel(fb, x0, y0);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

static void drawFilledCircle(uint8_t* fb, int cx, int cy, int r) {
    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            if (x * x + y * y <= r * r) {
                setPixel(fb, cx + x, cy + y);
            }
        }
    }
}

static void drawEllipse(uint8_t* fb, int cx, int cy, int rx, int ry) {
    for (int y = -ry; y <= ry; y++) {
        for (int x = -rx; x <= rx; x++) {
            if ((x * x * ry * ry + y * y * rx * rx) <= (rx * rx * ry * ry)) {
                setPixel(fb, cx + x, cy + y);
            }
        }
    }
}

// ── Eye positions ──
#define EYE_L_X  40
#define EYE_R_X  88
#define EYE_Y     28
#define EYE_R      7

// ── Face generators ──

static void genNeutral(uint8_t* fb) {
    clearFb(fb);
    drawFilledCircle(fb, EYE_L_X, EYE_Y, EYE_R);
    drawFilledCircle(fb, EYE_R_X, EYE_Y, EYE_R);
    drawLine(fb, 50, 45, 78, 45);
    drawFilledCircle(fb, EYE_L_X, EYE_Y, 3);
    drawFilledCircle(fb, EYE_R_X, EYE_Y, 3);
}

static void genHappy(uint8_t* fb) {
    clearFb(fb);
    drawFilledCircle(fb, EYE_L_X, EYE_Y, EYE_R);
    drawFilledCircle(fb, EYE_R_X, EYE_Y, EYE_R);
    drawLine(fb, 45, 43, 50, 48);
    drawLine(fb, 50, 48, 64, 50);
    drawLine(fb, 64, 50, 78, 48);
    drawLine(fb, 78, 48, 83, 43);
    drawFilledCircle(fb, EYE_L_X, EYE_Y - 2, 3);
    drawFilledCircle(fb, EYE_R_X, EYE_Y - 2, 3);
}

static void genSad(uint8_t* fb) {
    clearFb(fb);
    drawFilledCircle(fb, EYE_L_X, EYE_Y, EYE_R);
    drawFilledCircle(fb, EYE_R_X, EYE_Y, EYE_R);
    drawLine(fb, 48, 48, 55, 45);
    drawLine(fb, 55, 45, 64, 44);
    drawLine(fb, 64, 44, 73, 45);
    drawLine(fb, 73, 45, 80, 48);
    drawFilledCircle(fb, EYE_L_X, EYE_Y + 2, 3);
    drawFilledCircle(fb, EYE_R_X, EYE_Y + 2, 3);
    drawLine(fb, EYE_L_X - 12, EYE_Y + 14, EYE_L_X - 10, EYE_Y + 10);
    drawLine(fb, EYE_R_X + 12, EYE_Y + 14, EYE_R_X + 10, EYE_Y + 10);
}

static void genAngry(uint8_t* fb) {
    clearFb(fb);
    drawFilledCircle(fb, EYE_L_X, EYE_Y, EYE_R);
    drawFilledCircle(fb, EYE_R_X, EYE_Y, EYE_R);
    drawLine(fb, 50, 46, 64, 43);
    drawLine(fb, 64, 43, 78, 46);
    drawLine(fb, 64, 43, 64, 48);
    drawLine(fb, EYE_L_X - 12, EYE_Y - 14, EYE_L_X + 4, EYE_Y - 10);
    drawLine(fb, EYE_R_X + 12, EYE_Y - 14, EYE_R_X - 4, EYE_Y - 10);
    drawFilledCircle(fb, EYE_L_X, EYE_Y, 3);
    drawFilledCircle(fb, EYE_R_X, EYE_Y, 3);
}

static void genSurprised(uint8_t* fb) {
    clearFb(fb);
    drawFilledCircle(fb, EYE_L_X, EYE_Y, EYE_R + 2);
    drawFilledCircle(fb, EYE_R_X, EYE_Y, EYE_R + 2);
    drawFilledCircle(fb, EYE_L_X, EYE_Y, 2);
    drawFilledCircle(fb, EYE_R_X, EYE_Y, 2);
    drawFilledCircle(fb, 64, 48, 8);
}

static void genBlink(uint8_t* fb) {
    clearFb(fb);
    drawLine(fb, EYE_L_X - 8, EYE_Y, EYE_L_X + 8, EYE_Y);
    drawLine(fb, EYE_R_X - 8, EYE_Y, EYE_R_X + 8, EYE_Y);
    drawLine(fb, 50, 45, 78, 45);
}

static void genSleep(uint8_t* fb) {
    clearFb(fb);
    drawLine(fb, EYE_L_X - 10, EYE_Y, EYE_L_X, EYE_Y + 3);
    drawLine(fb, EYE_L_X, EYE_Y + 3, EYE_L_X + 10, EYE_Y);
    drawLine(fb, EYE_R_X - 10, EYE_Y, EYE_R_X, EYE_Y + 3);
    drawLine(fb, EYE_R_X, EYE_Y + 3, EYE_R_X + 10, EYE_Y);
    drawFilledCircle(fb, 64, 46, 3);
    drawLine(fb, 110, 12, 120, 12);
    drawLine(fb, 120, 12, 110, 20);
    drawLine(fb, 110, 20, 120, 20);
    drawLine(fb, 105, 20, 115, 20);
    drawLine(fb, 115, 20, 105, 28);
    drawLine(fb, 105, 28, 115, 28);
}

// ── Wave animation frames (happy face + waving arm) ──

static void drawHappyFace(uint8_t* fb) {
    drawFilledCircle(fb, EYE_L_X, EYE_Y, EYE_R);
    drawFilledCircle(fb, EYE_R_X, EYE_Y, EYE_R);
    drawFilledCircle(fb, EYE_L_X, EYE_Y - 2, 3);
    drawFilledCircle(fb, EYE_R_X, EYE_Y - 2, 3);
    drawLine(fb, 45, 43, 50, 48);
    drawLine(fb, 50, 48, 64, 50);
    drawLine(fb, 64, 50, 78, 48);
    drawLine(fb, 78, 48, 83, 43);
}

static void genWave1(uint8_t* fb) {
    clearFb(fb);
    drawHappyFace(fb);
    drawLine(fb, 18, 34, 18, 54);
    drawFilledCircle(fb, 18, 55, 2);
}

static void genWave2(uint8_t* fb) {
    clearFb(fb);
    drawHappyFace(fb);
    drawLine(fb, 18, 34, 10, 44);
    drawLine(fb, 10, 44, 4, 40);
    drawFilledCircle(fb, 2, 38, 2);
    drawLine(fb, 24, 6, 18, 8);
    drawLine(fb, 20, 4, 14, 6);
    drawLine(fb, 22, 2, 16, 4);
}

static void genWave3(uint8_t* fb) {
    clearFb(fb);
    drawHappyFace(fb);
    drawLine(fb, 18, 34, 18, 54);
    drawFilledCircle(fb, 18, 55, 2);
}

// ── Storage ──

static uint8_t faceBuffers[FACE_COUNT][FACE_BYTES];
static bool facesGenerated = false;

void generateFaces() {
    if (facesGenerated) return;
    genNeutral(faceBuffers[FACE_NEUTRAL]);
    genHappy(faceBuffers[FACE_HAPPY]);
    genSad(faceBuffers[FACE_SAD]);
    genAngry(faceBuffers[FACE_ANGRY]);
    genSurprised(faceBuffers[FACE_SURPRISED]);
    genBlink(faceBuffers[FACE_BLINK]);
    genSleep(faceBuffers[FACE_SLEEP]);
    genWave1(faceBuffers[FACE_WAVE_1]);
    genWave2(faceBuffers[FACE_WAVE_2]);
    genWave3(faceBuffers[FACE_WAVE_3]);
    facesGenerated = true;
}

uint8_t* getFaceBuffer(FaceID id) {
    generateFaces();
    if (id >= FACE_COUNT) return faceBuffers[FACE_NEUTRAL];
    return faceBuffers[id];
}
