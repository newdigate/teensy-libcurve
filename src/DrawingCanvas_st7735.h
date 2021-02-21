//
// Created by Nicholas Newdigate on 13/04/2020.
//
#ifndef TEENSY_LIBCURVE_DRAWING_CANVAS_ST7735_H
#define TEENSY_LIBCURVE_DRAWING_CANVAS_ST7735_H

#include "Arduino.h"

#include <ST7735_t3.h> // Hardware-specific library
#include "DrawingCanvas.h"

class DrawingCanvas_st7735 : public DrawingCanvas {
public:
    DrawingCanvas_st7735(
            ST7735_t3 &tft) : DrawingCanvas(),
            _tft(&tft) {
    };

    void drawLine(float x0, float y0, float x1, float y1, uint16_t color, LineEndpointStyle startPointStyle, LineEndpointStyle endPointStyle) override {
        if (_useAntialiasing)
            DrawingCanvas::drawLine(x0, y0, x1, y1, color, startPointStyle, endPointStyle);
        else
            _tft->drawLine(x0, y0, x1, y1, color);
    }
protected:
    bool useFramebuffer() override;
private:
    ST7735_t3 *_tft;
    void writePixel(int x, int y, uint16_t color) override;
    uint16_t readPixel(int x, int y) override;
};

#endif //TEENSY_LIBCURVE_DRAWING_CANVAS_ST7735_H
