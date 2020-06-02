//
//  Coord.h
//  CyberCat
//
//  Created by Goran Stojanovic on 06/02/2020.
//  Copyright © 2020 embware. All rights reserved.
//

#ifndef Coord_h
#define Coord_h

#include <cmath>
#include "Types.h"

static struct TrigonometryCacheTable
{
    #define MAX_ANGLE (Degree) 360
   
    double sinus[MAX_ANGLE + 1];
    double cosinus[MAX_ANGLE + 1];
    double radian[MAX_ANGLE + 1];

    TrigonometryCacheTable()
    {
        for (Degree angle=0; angle <= MAX_ANGLE; angle++)
        {
            radian[angle] =  PI * angle / 180;
            sinus[angle] = sin(radian[angle]);
            cosinus[angle] = cos(radian[angle]);
        }
    }
    
    inline Degree degree(double angleRadian)
    {
        return round(angleRadian * 180 / PI);
    }
    
} TrigCache;

struct Coord
{
    float x;
    float y;
    
    Coord operator+(Coord c)
    {
        x += c.x;
        y += c.y;
        return *this;
    }
    
    bool operator==(Coord c)
    {
        return x==c.x && y == c.y;
    }
    
    bool operator!=(Coord c)
    {
        return !(*this == c);
    }
    
    void translate(Degree angle, const float distance)
    {
        double angle_rad = TrigCache.radian[angle];
        x += distance * cos(angle_rad);
        y += distance * sin(angle_rad);
    }
    
    
    void cacheTranslate(const Degree angle, const float distance)
    {
        x += TrigCache.cosinus[angle] * distance;
        y += TrigCache.sinus[angle] * distance;
    }
};

#endif /* Coord_h */
