#include "DrawingCanvas.h"
#include "alphaBlendRGB565.h"
//https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm
void DrawingCanvas::drawLine(float x0, float y0, float x1, float y1, uint16_t color, LineEndpointStyle startPointStyle, LineEndpointStyle endPointStyle) {
    bool steep = DrawingCanvas_abs(y1 - y0) > DrawingCanvas_abs(x1 - x0);
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
    float yend = y0 + gradient * (x1 - x0);
    float intery = y0; // first y-intersection for the main loop
    int16_t xpxl1 = x0; // this will be used in the main loop
    int16_t xpxl2 = x1; //this will be used in the main loop

    if (startPointStyle == LineEndpointStyle::AntiAliased) {
        if (steep) {
            drawPixel(y0, x0, alphaBlendRGB565(color, _backgroundColor, (rem_part(yend) * rem_part(x0) * 255.0f)));
            drawPixel(y0 + 1, x0, alphaBlendRGB565(color, _backgroundColor, (frac_part(yend) * frac_part(x0) * 255.0f)));
        } else {
            drawPixel(xpxl1, y0, alphaBlendRGB565(color, _backgroundColor, rem_part(yend) * rem_part(x0) * 255.0f));
            drawPixel(xpxl1, y0 + 1, alphaBlendRGB565(color, _backgroundColor, frac_part(yend) * frac_part(x0) * 255.0f));
        }
        xpxl1++;
        intery += gradient; // first y-intersection for the main loop
    } else if  (startPointStyle == LineEndpointStyle::None) {
        xpxl1++;
        intery += gradient;
    }

    if (endPointStyle == LineEndpointStyle::AntiAliased) {
        // handle second endpoint
        yend = y0 + gradient * (x0 - x1);
        if (steep) {
            drawPixel(y1,        x1, alphaBlendRGB565(color, _backgroundColor, rem_part(yend) * rem_part(x1) * 255.0f));
            drawPixel(y1 + 1, x1, alphaBlendRGB565(color, _backgroundColor, frac_part(yend)* frac_part(x1) * 255.0f));
        } else {
            drawPixel(xpxl2,        y1, alphaBlendRGB565(color, _backgroundColor, rem_part(yend) * rem_part(x1) * 255.0f));
            drawPixel(xpxl2, y1 + 1, alphaBlendRGB565(color, _backgroundColor, frac_part(yend) * frac_part(x1) * 255.0f));
        }
        xpxl2-=1;
    }else if  (endPointStyle == LineEndpointStyle::None) {
        xpxl2-=1;
    }

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

void DrawingCanvas::drawCurve(float delta, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, uint16_t color, LineEndpointStyle startPointStyle, LineEndpointStyle endPointStyle) {
    float x = p0x, y = p0y;
    int16_t iterations = std::ceil( 1.0 / delta );

    bool firstPoint = true;

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

        bool endPoint = it == iterations;

        drawLine( x,    y,
                  x2,   y2,
                  color,
                  firstPoint? startPointStyle : LineEndpointStyle::None,
                  endPoint? endPointStyle : LineEndpointStyle::None );
        x = x2;
        y = y2;
        firstPoint = false;
    }
}
/*
void DrawingCanvas::drawCurve(float delta, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, uint16_t color, LineEndpointStyle startPointStyle, LineEndpointStyle endPointStyle) {
    float x = p0x, y = p0y;

    int16_t iterations = std::ceil( 1.0 / delta );

    bool firstPoint = true;

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
        bool endPoint = it == iterations;

        drawLine( x, y,
                  x2, y2,
                  color,
                  firstPoint? startPointStyle : LineEndpointStyle::NoneAntialiased,
                  endPoint? endPointStyle : LineEndpointStyle::NoneAntialiased);
        x = x2;
        y = y2;
        firstPoint = false;
    }
}
*/

void DrawingCanvas::drawCurve(float delta, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, uint16_t color, LineEndpointStyle startPointStyle, LineEndpointStyle endPointStyle) {
    float lX, lY;
    std::vector<float> points;
    float curveLength = getCurveLengthWithMaxDistance(p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y, 1.0f, lX, lY, points);
    int iterations = ceil(curveLength) + 1;
    float delta2 = 1.0f / (ceil(curveLength) + 1);

    bool firstPoint = true;

    int16_t x = p0x, y = p0y;
    std::vector<float> currentPixelXValues;
    std::vector<float> currentPixelYValues;
    bool wasSteep = false;
    bool wasSteepWasSet = false;
    for( int it = 0; it < points.size() / 2; it++ )
    {
        float px = points[it*2];
        float py = points[it*2 + 1];

        if (floor(px) != x || floor(py) != y) {
            float currentPixelYAveragesSum = 0.0f,
                  currentPixelXAveragesSum = 0.0f;
            for (const auto &pY : currentPixelYValues) {
                currentPixelYAveragesSum += pY;
            }
            for (const auto &pX : currentPixelXValues) {
                currentPixelXAveragesSum += pX;
            }
            float currentXAverage = currentPixelXAveragesSum / currentPixelXValues.size();
            float currentYAverage = currentPixelYAveragesSum / currentPixelYValues.size();
            if (!_useAntialiasing) {
                drawPixel(x, y, color);
            } else {
                bool steep = (DrawingCanvas_abs(py - currentYAverage) > DrawingCanvas_abs(px - currentXAverage));
                bool diag = (wasSteepWasSet && wasSteep != steep);
                bool steepCopy = steep;
                if (diag)
                    steep = wasSteep;



                if (steep) {
                    uint8_t alpha2 = rem_part(currentXAverage - x) * 255.0f;
                    if (alpha2 > 1) {
                        uint16_t acolor2 = alphaBlendRGB565(color, _backgroundColor, alpha2);
                        drawPixel(x, y, acolor2);
                    }
                } else {
                    uint8_t alpha2 = rem_part(currentYAverage - y) * 255.0f;
                    if (alpha2 > 1) {
                        uint16_t acolor2 = alphaBlendRGB565(color, _backgroundColor, alpha2);
                        drawPixel(x, y, acolor2);
                    }
                }

                currentPixelXValues.clear();
                currentPixelYValues.clear();
                if (steep) {
                    uint8_t alpha2 = frac_part(currentXAverage - x) * 255.0f;
                    if (alpha2 > 1) {
                        uint16_t acolor2 = alphaBlendRGB565(color, _backgroundColor, alpha2);
                        drawPixel(x + 1, y, acolor2);
                    }
                } else {
                    uint8_t alpha2 = frac_part(currentYAverage - y) * 255.0f;
                    if (alpha2 > 1) {
                        uint16_t acolor2 = alphaBlendRGB565(color, _backgroundColor, alpha2);
                        drawPixel(x, y + 1, acolor2);
                    }
                }

                if (diag)
                    steep = steepCopy;

                wasSteep = steep;
                wasSteepWasSet = true;
            }
            x = px;
            y = py;
        }

        currentPixelXValues.push_back(px);
        currentPixelYValues.push_back(py);
    }

}

float
DrawingCanvas::getCurveLength(float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float &longestXSection, float &longestYSection) {
    float distance = 0.0f, newDistance = -1.0f;
    uint16_t iteration = 1;

    while (DrawingCanvas_abs(distance - newDistance) > 0.0005f)
    {
        distance = newDistance;
        std::vector<float> points;
        newDistance = getCurveLength(1.0f/((iteration*2.0f)+1.0f), p0x,  p0y, p1x, p1y, p2x, p2y, p3x, p3y, longestXSection, longestYSection, points);
        iteration++;
    }

    return newDistance;

}

float
DrawingCanvas::getCurveLength(float delta, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float p3x,
                              float p3y, float &longestXSection, float &longestYSection, std::vector<float> &points) {
    int16_t iterations = std::ceil( 1.0f / delta );

    float sumOfXdistance = 0.0f;
    float sumOfYdistance = 0.0f;
    float x = p0x;
    float y = p0y;
    longestXSection = -1.0f;
    longestYSection = -1.0f;
    points.push_back(x);
    points.push_back(y);

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

        float lengthX = DrawingCanvas_abs(x2 - x);
        float lengthY = DrawingCanvas_abs(y2 - y);

        if (lengthX > longestXSection ){
            longestXSection = lengthX;
        }
        if (lengthY > longestYSection ){
            longestYSection = lengthY;
        }
        sumOfXdistance += lengthX;
        sumOfYdistance += lengthY;
        points.push_back(x2);
        points.push_back(y2);

        x = x2;
        y = y2;
    }

    return std::sqrt(sumOfXdistance * sumOfXdistance +  sumOfYdistance*sumOfYdistance);

}

float DrawingCanvas::getCurveLengthWithMaxDistance(float p0x, float p0y, float p1x, float p1y, float p2x, float p2y,
                                                   float p3x, float p3y, float maxDistance, float &longestXSection,
                                                   float &longestYSection, std::vector<float> &points) {

    longestXSection = 0.0f;
    longestYSection = 0.0f;
    int it = 1;
    float length = 0.0f;
    float delta = 0.5;
    bool maxDistanceIsSet = false;
    while(!maxDistanceIsSet || (longestYSection > maxDistance|| longestXSection > maxDistance) )
    {
        points.clear();
        it++;
        length = getCurveLength(delta, p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y, longestXSection, longestYSection, points );
        if(!maxDistanceIsSet)
            maxDistanceIsSet = true;
        delta /= (longestXSection > longestYSection)? longestXSection : longestYSection;
    }

    return length;
}




/*
void DrawingCanvas::drawArc(float x, float y, float r, float startRadians, float endRadians) {

}
*/
