//
// Created by Nicholas Newdigate on 13/04/2020.
//
#ifndef TEENSY_LIBCURVE_DRAWING_CANVAS_H
#define TEENSY_LIBCURVE_DRAWING_CANVAS_H

#include <cmath>
#include <vector>
#include "Arduino.h"

#ifndef DrawingCanvas_swap
#define DrawingCanvas_swap(a, b) { typeof(a) t = a; a = b; b = t; }
#endif
enum LineEndpointStyle {
    None = 0,
    AntiAliased = 1,
    NoneAntialiased = 2
};

class DrawingCanvas {
public:
    DrawingCanvas() { };
    virtual void drawPixel(int x, int y, uint16_t color) { writePixel(x,y, color); };
    virtual void drawLine(float x0, float y0, float x1, float y1, uint16_t color, LineEndpointStyle startPointStyle, LineEndpointStyle endPointStyle);
    virtual void drawCurve(float delta, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, uint16_t color, LineEndpointStyle startPointStyle, LineEndpointStyle endPointStyle);
    virtual void drawCurve(float delta, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, uint16_t color, LineEndpointStyle startPointStyle, LineEndpointStyle endPointStyle);
    float getCurveLength(float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float &longestXSection, float &longestYSection);
    //virtual void drawArc(float x, float y, float r, float startRadians, float endRadians);
    virtual void setBackgroundColor(uint16_t backgroundColor) {
        _backgroundColor = backgroundColor;
    }

    virtual uint16_t getPixel(int x, int y) {
        if(useFramebuffer())
            return readPixel(x, y);
        else
            return _backgroundColor;
    };

protected:
    uint16_t _backgroundColor = 0;
    bool _useAntialiasing = true;
    virtual bool useFramebuffer() { return false; };
    virtual uint16_t readPixel(int x, int y) = 0;
    virtual void writePixel(int x, int y, uint16_t color) = 0;

    inline float getPt( float n1 , float n2 , float perc )
    {
        float diff = n2 - n1;

        return n1 + ( diff * perc );
    }

    inline float frac_part(float x) {
        return x - std::floor(x);
    }

    inline float rem_part(float x) {
        return 1.0f - frac_part(x);
    }
private:
    float getCurveLength(float delta, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float &longestXSection, float &longestYSection, vector<float> &points);
    float getCurveLengthWithMaxDistance(float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float maxDistance, float &longestXSection, float &longestYSection, vector<float> &points );
};

#endif //TEENSY_LIBCURVE_DRAWING_CANVAS_H
