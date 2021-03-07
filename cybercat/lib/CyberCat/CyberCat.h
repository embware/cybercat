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
    Degree moveAngle;
    
    CyberCat(ServoDriver& driver, Len boneLength = 200) : driver {driver}, boneLength { boneLength} , moveAngle {5}
    {
       heightMax = 2 * boneLength - 20;
       heightMin = 2 * boneLength * TrigCache.sinus[driver.config[FLK].min/2];
        
       LOG4("Cat config | bone: %d mm, height [min %d mm, max %d mm] mov-angle %d deg",boneLength,heightMin,heightMax,moveAngle);
    }
    
    void standup()
    {
        height(sqrt(2) * boneLength);
    }

    void stand_down()
    {
        height(0.5 * boneLength);
    }
    
    void up()
    {
        const Len dHeight = 20;
        height(heightFront() + dHeight);
    }

    void down()
    {
        const Len dHeight = 20;
        height(heightFront() - dHeight);
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
    

    void test()
    {
        driver.add(com.test);
    }

    void step(Len distance)
    {
        // TODO calculate moveAngle based on step distance
    }
    
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
        
        const Degree angle = TrigCache.degree(asin((double)height * 0.5 / boneLength));
        const Degree sa = 180 - angle;  //sa - shoulder angle
        const Degree ka = 2 * angle; // ka - knee angle
        
        com.updateCommands(sa,ka, moveAngle);
        
        driver.add(com.height);
    }
    
    void setMoveAngle(Degree angle)
    {
        moveAngle = angle;
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
