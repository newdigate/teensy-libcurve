#include <Arduino.h>
#include "DrawingCanvas.h"
#include "DrawingCanvas_st7735.h"

#ifdef BUILD_FOR_OPENGL_EMULATOR
#include <st7735_opengl.h>
st7735_opengl tft = st7735_opengl(true,10);
void my_yield() {
    if(!tft.shouldClose()) {
        tft.updateScreen();
    }
}
#else
#include "ST7735_t3.h"
ST7735_t3 tft = ST7735_t3(1,2,3);
#endif

DrawingCanvas_st7735 canvas(tft);

void testcurves(float x, float y, float amp);
void testlines(float x, float y,float phase);

void drawTestCurve(const float *points, uint16_t color, uint16_t pointscolor);
void drawTestLines(const float *points, uint16_t color, uint16_t pointscolor);

uint16_t color = ST7735_WHITE;
uint16_t bgcolor = ST7735_BLACK;
uint16_t pointscolor = ST7735_RED;

bool useFrameBuffer = true;
uint16_t phase = 0;

void setup() {
#ifdef BUILD_FOR_OPENGL_EMULATOR
    initialize_mock_arduino();
    yield_impl = my_yield;
#endif
    tft.initR(INITR_GREENTAB);
    tft.setRotation(3);
    if (useFrameBuffer)
        tft.useFrameBuffer(true);
}
bool isPaused = false;

void loop() {

    if (Serial.available()) {
        if (Serial.read() == ' ') {
            isPaused = !isPaused;
        }
    }

    if (!isPaused) {
        phase++;

        float x_offset = 0.0f;
        float y_offset = 0.0f;
        if (useFrameBuffer)
            tft.updateScreenAsync(false);
        tft.fillScreen(bgcolor);
        uint startTime = micros();
        testcurves(x_offset, y_offset, sin(phase / 320.0f));
        testlines(x_offset, y_offset, phase * M_PI / 3200);
        uint stopTime = micros();

        //Serial.printf("done in %i microseconds\n", stopTime - startTime);

        if (useFrameBuffer) {
            tft.updateScreenAsync(true);
            tft.updateScreen();
        }
        phase++;
    }
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

void testcurves(float x, float y, float amp) {
    float points[8] = {0.0f + x, 60.0f + y,
                       40.0f + x, 60.0f + y + 220.0f * amp,
                       80.0f + x, 60.0f + y - 220.0f * amp,
                       120.0f + x, 60.0f + y };
    drawTestCurve(points, color, pointscolor);
}

void testlines(float x, float y, float phase) {

    for (int i=0; i< 16;i++){
        float y2 = 20.0f * std::sin(phase + 2 * i * M_PI/16.0 );
        float x2 = 20.0f * std::cos(phase + 2 * i * M_PI/16.0 );
        float points5[4] = {90.0f + x, 20.0f+ y,
                            90.0f + x + x2 , 20.0f + y+ y2};

        drawTestLines(points5, color, pointscolor);
    }
}

void drawTestCurve(const float *points, uint16_t color, uint16_t pointscolor) {
    canvas.drawCurve(1/16.0f,
                  points[0], points[1],
                  points[2], points[3],
                  points[4], points[5],
                  points[6], points[7],
                  color, None, None);
    tft.drawPixel(points[0], points[1], pointscolor);
    tft.drawPixel(points[2], points[3], pointscolor);
    tft.drawPixel(points[4], points[5], pointscolor);
    tft.drawPixel(points[6], points[7], pointscolor);
}

void drawTestLines(const float *points, uint16_t color, uint16_t pointscolor) {
    tft.drawPixel(points[0], points[1], pointscolor);
    tft.drawPixel(points[2], points[3], pointscolor);

    uint start = micros();
    canvas.drawLine( points[0], points[1],
                  points[2], points[3],
                  color,
                     None, None);
    uint stop = micros();
    //Serial.printf("drawTestLines: (%2f,%2f) -> (%2f,%2f): %u\n", points[0], points[1],  points[2], points[3], stop - start);
}
