//
//  Model.h
//  CyberCat
//
//  Created by Goran Stojanovic on 09/02/2020.
//  Copyright © 2020 embware. All rights reserved.
//

#ifndef Model_h
#define Model_h

#include "Coord.h"
#include <cmath>

struct Model
{
    Coord posKnee;
    Coord posShoulder;
    Coord posFoot;
    Degree angleShoulder;
    Degree angleKnee;
    Len boneLength;
  

    Model(Coord posShoulder = {0,0}, Degree angleShoulder = 135, Degree angleKnee = 90, Len boneLentgth = 200)
    : posShoulder {posShoulder}, angleShoulder {angleShoulder}, angleKnee {angleKnee}, boneLength {boneLentgth}
    {
        update();
    }
    
    Model(Degree angleShoulder, Degree angleKnee, Len boneLentgth = 200)
    : Model {{0,0}, angleShoulder, angleKnee, boneLentgth}
    {}

    void update()
    {
        posKnee = posShoulder;
        posKnee.cacheTranslate(angleShoulder, boneLength);
        posFoot = posKnee;
        // posFoot.cacheTranslate(angleShoulder + angleKnee -180, boneLength);
        posFoot.cacheTranslate(angleShoulder + angleKnee , - boneLength);
    }
    
    void shoulder(Degree angle)
    {
        angleShoulder = angle;
        update();
    }
    
    void knee(Degree angle)
    {
        angleKnee = angle;
        update();
    }
    
    void set(Degree shoulderAngle, Degree kneeAngle)
    {
        angleShoulder = shoulderAngle;
        angleKnee = kneeAngle;
        update();
    }
   
    inline Degree footAngle()
    {
        return angleShoulder + angleKnee - 180;
    }
    
    inline Degree kneeAngle(const Len height)
    {
        return 180 - angleShoulder + asin(height/boneLength - TrigCache.sinus[angleShoulder]) * 180 / PI;
    }
   
};


#endif /* Model_h */
