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

    float x_offset = 0.0f;
    float y_offset = 0.0f;
    if (useFrameBuffer)
        tft.updateScreenAsync(false);
    tft.fillScreen(bgcolor);
    uint startTime = micros();
    canvas.drawLine(64.054863, 116.356781,
                    63.128590, 115.474579,
                    color, NoneAntialiased, NoneAntialiased  );

    canvas.drawCurve(0.1f,
                     63.128590, 115.474579,
                     22.372581, 77.539177,
                     11.566071, 64.482330,
                     11.566071, 43.309109,
                     color, NoneAntialiased, NoneAntialiased  );

    canvas.drawCurve(0.1f,
                     11.566071, 43.309109,
                     11.566071, 25.664846,
                     23.916372,  11.549276,
                     39.354259, 11.549276,
                          color, NoneAntialiased, NoneAntialiased  );

    canvas.drawCurve(0.1f,
                     39.354259, 11.549276,
                     52.013313, 11.549276,
                     59.114746, 19.665655,
                     64.054863, 26.017624,
                     color, NoneAntialiased, NoneAntialiased  );

    canvas.drawCurve(0.1f,
                     64.054863, 26.017624,
                     68.994995, 19.665657,
                     76.096413, 11.549277,
                     88.755478, 11.549277,
                     color, NoneAntialiased, NoneAntialiased  );

    canvas.drawCurve(0.1f,
                     88.755478, 11.549277,
                     104.193359, 11.549277,
                     116.543671, 25.664848,
                     116.543671, 43.309109,
                     color, NoneAntialiased, NoneAntialiased  );
    canvas.drawCurve(0.1f,
                     116.543671, 43.309109,
                     116.543671, 64.482330,
                     105.737152, 77.539169,
                     65.907410, 114.592377,
                     color, NoneAntialiased, NoneAntialiased  );

    canvas.drawLine(65.907410, 114.592377, 64.054863, 116.356781,
                    color, NoneAntialiased, NoneAntialiased  );

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