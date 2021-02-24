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

    float x_offset = 0.0f;
    float y_offset = 0.0f;
    if (useFrameBuffer)
        tft.updateScreenAsync(false);
    tft.fillScreen(bgcolor);
    uint startTime = micros();

    canvas.drawCurve(0.1f,
                     0.0f, 64.0f,
                     0.0f, -64.0f,
                     120.0f, 128.0f,
                     120.0f, 70.0f, ST7735_WHITE, None, None);

    uint stopTime = micros();

    //Serial.printf("done in %i microseconds\n", stopTime - startTime);

    if (useFrameBuffer) {
        tft.updateScreenAsync(true);
        tft.updateScreen();
    }

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