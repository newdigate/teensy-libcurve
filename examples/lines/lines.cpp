#include <Arduino.h>
#include "DrawingCanvas.h"
#include "DrawingCanvas_st7735.h"

#ifdef BUILD_FOR_OPENGL_EMULATOR
#include <st7735_opengl.h>
st7735_opengl tft = st7735_opengl(true);
#else
#include "ST7735_t3.h"
ST7735_t3 tft = ST7735_t3(1,2,3);
#endif


DrawingCanvas_st7735 canvas(tft);
float p = 3.1415926;

void testVerticleLine(float x, float y);
void testHorizontalLine(float x, float y);

void drawTestHLines(const float *points, uint16_t color, uint16_t pointscolor);
void drawTestVLines(const float *points, uint16_t color, uint16_t pointscolor);

void testVerticleLines();
void testHorizontalLines();

uint16_t color = ST7735_WHITE;
uint16_t bgcolor = ST7735_BLACK;
uint16_t pointscolor = ST7735_RED;

#ifdef BUILD_FOR_OPENGL_EMULATOR
void my_yield() {
    if(!tft.shouldClose()) {
        tft.update();
    }
}
#endif

bool useFrameBuffer = true;

void setup() {
#ifdef BUILD_FOR_OPENGL_EMULATOR
    initialize_mock_arduino();
    yield_impl = my_yield;
#endif

    tft.initR(INITR_GREENTAB);
    tft.setRotation(3);
    if (useFrameBuffer)
        tft.useFrameBuffer(true);

    tft.fillScreen(bgcolor);
    testVerticleLines();
    testHorizontalLines();
}

void loop() {
    delay(100);
}

#ifdef BUILD_FOR_OPENGL_EMULATOR
int main() {
    setup();
    while (!tft.shouldClose()) {
        loop();
    }
}
#endif

void testVerticleLines() {
    if (useFrameBuffer)
        tft.updateScreenAsync(false);

    uint startTime = micros();
    testVerticleLine(10.0f, 10.0f);
    uint stopTime = micros();
    Serial.printf("done in %i microseconds\n", stopTime - startTime);

    if (useFrameBuffer) {
        tft.updateScreenAsync(true);
        tft.updateScreen();
    }
}
void testHorizontalLines() {
    if (useFrameBuffer)
        tft.updateScreenAsync(false);

    uint startTime = micros();
    testHorizontalLine(10.0f,10.0f);
    uint stopTime = micros();

    Serial.printf("done in %i microseconds\n", stopTime - startTime);

    if (useFrameBuffer) {
        tft.updateScreenAsync(true);
        tft.updateScreen();
    }
}
void testVerticleLine(float x, float y) {
    float points[4] = {10.0f + x, 10.0f + y,
                       10.0f + x, 20.0f + y};
    drawTestVLines(points, color, pointscolor);
}

void testHorizontalLine(float x, float y) {
    float points[4] = {100.0f + x, 10.0f + y,
                       110.0f + x, 10.0f + y};
    drawTestHLines(points, color, pointscolor);
}

void drawTestHLines(const float *points, uint16_t acolor, uint16_t pointcolor) {
    tft.drawPixel(points[0], points[1]-1, pointcolor);
    tft.drawPixel(points[2], points[3]-1, pointcolor);

    uint start = micros();
    canvas.drawLine(points[0], points[1],
                  points[2], points[3],
                  acolor,
                  LineEndpointStyle::AntiAliased,
                  LineEndpointStyle::AntiAliased);

    canvas.drawLine(points[0], points[1] + 2.0f,
                    points[2], points[3] + 2.0f,
                    acolor,
                    LineEndpointStyle::None,
                    LineEndpointStyle::AntiAliased);

    canvas.drawLine(points[0], points[1] + 4.0f,
                    points[2], points[3] + 4.0f,
                    acolor,
                    LineEndpointStyle::AntiAliased,
                    LineEndpointStyle::None);

    canvas.drawLine(points[0], points[1] + 6.0f,
                    points[2], points[3] + 6.0f,
                    acolor,
                    LineEndpointStyle::None,
                    LineEndpointStyle::None);
    uint stop = micros();
    Serial.printf("drawTestHLines: (%2f,%2f) -> (%2f,%2f): %u\n", points[0], points[1],  points[2], points[3], stop - start);
}

void drawTestVLines(const float *points, uint16_t acolor, uint16_t pointcolor) {
    tft.drawPixel(points[0]-1, points[1], pointcolor);
    tft.drawPixel(points[2]-1, points[3], pointcolor);

    uint start = micros();
    canvas.drawLine( points[0], points[1],
                     points[2], points[3],
                     acolor,
                     LineEndpointStyle::AntiAliased,
                    LineEndpointStyle::AntiAliased);

    canvas.drawLine( points[0]+2.0f, points[1],
                     points[2]+2.0f, points[3],
                     color,
                     LineEndpointStyle::None,
                     LineEndpointStyle::AntiAliased);

    canvas.drawLine( points[0]+4.0f, points[1],
                     points[2]+4.0f, points[3],
                     color,
                     LineEndpointStyle::AntiAliased,
                     LineEndpointStyle::None);

    uint stop = micros();


    Serial.printf("drawTestHLines: (%2f,%2f) -> (%2f,%2f): %u\n",  points[0], points[1],  points[2], points[3], stop - start);
}
