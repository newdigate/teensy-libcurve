#include "DrawingCanvas.h"
#include "alphaBlendRGB565.h"

void DrawingCanvas::drawLine(float x0, float y0, float x1, float y1, uint16_t color) {
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        DrawingCanvas_swap(x0, y0);
        DrawingCanvas_swap(x1, y1);
    }
    if (x0 > x1) {
        DrawingCanvas_swap(x0, x1);
        DrawingCanvas_swap(y0, y1);
    }

    float dx, dy, gradient;
    dx = x1 - x0;
    dy = y1 - y0;
    if (dx == 0.0f) {
        gradient = 1.0;
    } else
        gradient = dy / dx;

    // handle first endpoint
    float xend = x0;
    float yend = y0 + gradient * (xend - x0);
    float intery = yend; // first y-intersection for the main loop
    int16_t xpxl1 = xend; // this will be used in the main loop

    xend = x1;
    yend = y1 + gradient * (xend - x1);
    int16_t xpxl2 = xend; //this will be used in the main loop


    // main loop
    if (steep) {
        for (int16_t x=xpxl1; x <= xpxl2; x++) {
            uint8_t alpha = rem_part(intery) * 255.0f;
            int16_t interyx = int16_t(intery);
            if (alpha > 0) {
                uint16_t backgroundColor = getPixel(interyx, x);
                uint16_t firstPixelColor = alphaBlendRGB565(color, backgroundColor, alpha);
                drawPixel(interyx, x, firstPixelColor);
            }

            uint8_t alpha2 = frac_part(intery) * 255.0f;
            if (alpha2 > 0) {
                uint16_t backgroundColor2 = getPixel(interyx + 1, x);
                uint16_t secondPixelColor = alphaBlendRGB565(color, backgroundColor2, alpha2);
                drawPixel(interyx + 1, x, secondPixelColor);
            }
            intery += gradient;
        }
    }
    else {
        for (int16_t x=xpxl1; x <= xpxl2; x++) {
            uint8_t alpha = rem_part(intery) * 255.0;
            int16_t interyy = int16_t(intery);
            if (alpha > 0) {
                uint16_t backgroundColor = getPixel(x, interyy);
                uint16_t firstPixelColor = alphaBlendRGB565(color, backgroundColor, alpha);
                drawPixel(x, interyy, firstPixelColor);
            }

            uint8_t alpha2 = frac_part(intery) * 255.0;
            if (alpha2 > 0) {
                uint16_t backgroundColor2 = getPixel(x, interyy + 1);
                uint16_t secondPixelColor = alphaBlendRGB565(color, backgroundColor2, alpha2);
                drawPixel(x, interyy + 1, secondPixelColor);
            }
            intery += gradient;
        }
    }
}

void DrawingCanvas::drawCurve(float delta, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, uint16_t color) {
    float x = p0x, y = p0y;
    int16_t iterations = std::ceil( 1.0 / delta );

    for( int it = 1; it < iterations+1; it++ )
    {
        float i = it * delta;
        if (i > 1.0)
            i = 1.0;

        // The Green Line
        float xa = getPt( p0x , p1x , i );
        float ya = getPt( p0y , p1y , i );
        float xb = getPt( p1x , p2x , i );
        float yb = getPt( p1y , p2y , i );

        // The Black Dot
        float x2 = getPt( xa , xb , i );
        float y2 = getPt( ya , yb , i );

        drawLine( x, y, x2, y2, color );
        x = x2;
        y = y2;
    }
}

void DrawingCanvas::drawCurve(float delta, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, uint16_t color) {
    float x = p0x, y = p0y;

    int16_t iterations = std::ceil( 1.0 / delta );

    for( int it = 1; it < iterations+1; it++ )
    {
        float i = it * delta;
        if (i > 1.0)
            i = 1.0;
        // The Green Lines
        float xa = getPt( p0x , p1x , i );
        float ya = getPt( p0y , p1y , i );
        float xb = getPt( p1x , p2x , i );
        float yb = getPt( p1y , p2y , i );
        float xc = getPt( p2x , p3x , i );
        float yc = getPt( p2y , p3y , i );

        // The Blue Line
        float xm = getPt( xa , xb , i );
        float ym = getPt( ya , yb , i );
        float xn = getPt( xb , xc , i );
        float yn = getPt( yb , yc , i );

        // The Black Dot
        float x2 = getPt( xm , xn , i );
        float y2 = getPt( ym , yn , i );

        drawLine( x, y, x2, y2, color );
        x = x2;
        y = y2;
    }
}
