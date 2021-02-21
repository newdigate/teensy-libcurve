#include "DrawingCanvas_st7735.h"

void DrawingCanvas_st7735::writePixel(int x, int y, uint16_t color) {
    _tft->drawPixel(x,y,color);
}

uint16_t DrawingCanvas_st7735::readPixel(int x, int y) {
    if(useFramebuffer())
        return _tft->readPixel(x, y);
    else
        return _backgroundColor;
}

bool DrawingCanvas_st7735::useFramebuffer() {
    return _tft->getFrameBuffer() != NULL;
}
