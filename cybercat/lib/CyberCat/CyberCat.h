//
//  CyberCat.h
//  CyberCat
//
//  Created by Goran Stojanovic on 10/02/2020.
//  Copyright © 2020 embware. All rights reserved.
//

#ifndef CyberCat_h
#define CyberCat_h

#include "Types.h"
#include "Coord.h"
#include "ServoDriver.h"
#include "Model.h"
#include "CommandManager.h"
#include "StateManager.h"
#include "ServoCalibration.h"

struct CyberCat
{
    ServoDriver& driver;
    Len boneLength;
    Len heightMax;
    Len heightMin;
    CommandManger com;
    
    CyberCat(ServoDriver& driver, Len boneLength = 200) : driver {driver}, boneLength { boneLength}
    {
       heightMax = 2 * boneLength;
       heightMin = 2 * boneLength * TrigCache.sinus[driver.config[FLK].min/2];
        
       LOG3("Cat config | bone: %d mm, height [%d mm, %d mm]",boneLength,heightMin,heightMax);
    }
    
    void up()
    {
        height(boneLength * sqrt(2));
    }
    
    void down()
    {
        height(boneLength/2);
    }
   
    void standby()
    {
        driver.add(com.height);
    }
    
    void forward()
    {
        driver.add(com.forward);
    }
    
    void backward()
    {
        driver.add(com.backward);
    }
    
#ifdef CATSIM
    void test()
    {
        driver.add(com.test);
    }
#endif
    
    void height(Len height)
    {
        if (height > heightMax)
        {
            LOG2("Height %d over limt %d",height, heightMax);
            height = heightMax;
        }

        if (height < heightMin)
        {
            LOG2("Height %d under limt %d",height, heightMin);
            height = heightMin;
        }
        LOG("Set height %d mm" , height);
        
        const Degree angle = TrigCache.degree(asin((double)height / 2 / boneLength));
        const Degree sa = 180 - angle;  //sa - shoulder angle
        const Degree ka = 2 * angle; // ka - knee angle
        
        com.update_height(sa,ka);
        
        driver.add(com.height);
    }
    
   
    inline bool idle()
    {
        return driver.idle();
    }
    
    Len heightFront()
    {
        Model model {driver.servoAngle[FLS],driver.servoAngle[FLK],boneLength};
        return model.posFoot.y;
    }
    
    Len heightBack()
    {
        Model model {driver.servoAngle[BRS],driver.servoAngle[BRK],boneLength};
        return model.posFoot.y;
    }
   
};

#endif /* CyberCat_h */
