#include <Arduino.h>
#include "DrawingCanvas.h"
#include "DrawingCanvas_st7735.h"

#ifdef BUILD_FOR_OPENGL_EMULATOR
#include <st7735_opengl.h>
st7735_opengl tft = st7735_opengl(true, 10);
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

    if (useFrameBuffer)
        tft.updateScreenAsync(false);

    tft.fillScreen(bgcolor);
    uint startTime = micros();
    for (int j=0; j<5;j++)
        for (int i=0; i<10;i++) {
            // horizontal line
            canvas.drawLine(10.0f+i/9.0f + j * 20.0f, 00.0f + i * 10.0f + j / 4.0f,
                            25.0f+i/9.0f + j * 20.0f, 00.0f + i * 10.0f + j / 4.0f,
                            color,
                            LineEndpointStyle::None,
                            LineEndpointStyle::None);

            canvas.drawLine(10.0f+i/9.0f + j * 20.0f, 03.0f + i * 10.0f + j / 4.0f,
                            25.0f+i/9.0f + j * 20.0f, 03.0f + i * 10.0f + j / 4.0f,
                            color,
                            LineEndpointStyle::AntiAliased,
                            LineEndpointStyle::AntiAliased);

            canvas.drawLine(10.0f+i/9.0f + j * 20.0f, 06.0f + i * 10.0f + j / 4.0f,
                            25.0f+i/9.0f + j * 20.0f, 06.0f + i * 10.0f + j / 4.0f,
                            color,
                            LineEndpointStyle::NoneAntialiased,
                            LineEndpointStyle::NoneAntialiased);
        }
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