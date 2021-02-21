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

uint16_t color = ST7735_WHITE;
uint16_t bgcolor = ST7735_BLACK;

#ifdef BUILD_FOR_OPENGL_EMULATOR
void my_yield() {
    if(!tft.shouldClose()) {
        tft.update();
    }
}
#endif

bool useFrameBuffer = false;

void setup() {
#ifdef BUILD_FOR_OPENGL_EMULATOR
    initialize_mock_arduino();
    yield_impl = my_yield;
#endif

    tft.initR(INITR_GREENTAB);
    tft.setRotation(3);
    if (useFrameBuffer)
        tft.useFrameBuffer(true);

    if (useFrameBuffer)
        tft.updateScreenAsync(false);

    tft.fillScreen(bgcolor);
    uint startTime = micros();
/*    tft.drawPixel(10, 2, ST7735_RED);
    tft.drawPixel(15, 4, ST7735_RED);
    */
    canvas.drawLine(10.0f, 03.4f,
                    15.25f, 3.4f,
                    color,
                    LineEndpointStyle::NoneAntialiased,
                    LineEndpointStyle::NoneAntialiased);

    canvas.drawLine(15.25f, 3.5f,
                    25.0f, 3.5f,
                    color,
                    LineEndpointStyle::NoneAntialiased,
                    LineEndpointStyle::NoneAntialiased);

    uint stopTime = micros();
    Serial.printf("done in %i microseconds\n", stopTime - startTime);

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