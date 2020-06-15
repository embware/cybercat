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

const uint8 FLS = 0; // Shoulder Front Left
const uint8 FRS = 1; // Shoulder Front Right
const uint8 BLS = 2; // Shoulder Back Left
const uint8 BRS = 3; // Shoulder Back Right
const uint8 FLK = 4; // Knee Front Left
const uint8 FRK = 5; // Knee Front Right
const uint8 BLK = 6; // Knee Back Left
const uint8 BRK = 7; // Knee Back Left

struct CyberCat
{
    ServoDriver& driver;
    Len boneLength;
    Len heightMax;
    Len heightMin;
    
    CyberCat(ServoDriver& driver, Len boneLength = 200) : driver {driver}, boneLength { boneLength}
    {
       heightMax = 2 * boneLength;
       heightMin = 2 * boneLength * TrigCache.sinus[driver.config[FLK].min/2];
        
       LOG3("Cat config | bone: %d mm, height [%d mm, %d mm]",boneLength,heightMin,heightMax);
    }
    
    inline static uint8 KA(uint8 shoulderAngle,uint8 height, uint8 distance )
    {
        return KA(shoulderAngle,height/distance);
    }
    
    inline static uint8 KA(uint8 shoulderAngle,double hdRatio = sqrt(2))
    {
        // formula:
        //  beta = 180 - angle + (double)asin(height/boneLenth - TrigCache.sinus[angle]) * 180 / TrigCache.PI;
        //  beta = 180 - angle + (double)asin(hd - TrigCache.sinus[angle]) * 180 / TrigCache.PI;
        return 180 - shoulderAngle + TrigCache.degree(sin(hdRatio - TrigCache.sinus[shoulderAngle]));
    }
    
    
    struct
    {
        
        Command height[10];
        
        Command forward[25];
        /*
        {
            {FLS, si + delta}, {FRS, si}, {BLS, si}, {BRS, si + delta},
            {FLK,  ki + delta}, {FRK, ki - updelta}, {BLK, ki - updelta}, {BRK,  ki + delta},
            {Command::SYN},
                        {FRK,  ki}, {BLK,  ki},
            {Command::SYN},
            {FLS, si},  {FRS, si + delta}, {BLS, si + delta}, {BRS, si},
            {FLK,  ki - updelta},  {FRK,  ki + delta}, {BLK,  ki + delta}, {BRK,  ki - updelta},
            {Command::SYN},
            {FLK,  ki},                          {BRK,  ki},
            {Command::SYN},
            {Command::END}
        };
         */
        
        
    } com;
    
    
    void up()
    {
        height(boneLength * sqrt(2));
    }
    
    void down()
    {
        height(boneLength/2);
    }
   
    void stand()
    {
        driver.add(com.height);
    }
    
    void forward()
    {
        driver.add(com.forward);
    }
    
    void height(Len height, bool sync = true)
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

        const Degree angle = TrigCache.degree(asin((double)height / 2 / boneLength));
        const Degree sa = 180 - angle;  //sa - shoulder angle
        const Degree ka = 2 * angle; // ka - knee angle
        
        com.height[0] = {FLS,sa};
        com.height[1] = {BRS,sa};
        com.height[2] = {FRS,sa};
        com.height[3] = {BLS,sa};

        com.height[4] = {FLK,ka};
        com.height[5] = {BRK,ka};
        com.height[6] = {FRK,ka};
        com.height[7] = {BLK,ka};
      
        if (sync)
        {
            com.height[8] = {Command::SYN};
            com.height[9] = {Command::END};
        }
        else
        {
           com.height[8] =  {Command::END};
        }
                
        LOG("Set height %d mm" , height);
        
        driver.add(com.height);
        update_forward(sa,ka,10,5);
    }
    
  
    inline void update_stand(Degree shoulder_init, Degree knee_init)
    {
        int index = 0;
        com.forward[index++] = {FLS, shoulder_init};
        com.forward[index++] = {FRS, shoulder_init};
        com.forward[index++] = {BLS, shoulder_init};
        com.forward[index++] = {BRS, shoulder_init};
        
        com.forward[index++] = {FLK, knee_init};
        com.forward[index++] = {FRK, knee_init};
        com.forward[index++] = {BLK, knee_init};
        com.forward[index++] = {BRK, knee_init};
        
        com.forward[index++] = {Command::SYN};
        com.forward[index++] = {Command::END};
        
    }
    inline void update_forward(Degree shoulder_init, Degree knee_init, Degree move, Degree legup)
    {
        int index = 0;
        com.forward[index++] = {FLS, shoulder_init + move};
        com.forward[index++] = {FRS, shoulder_init};
        com.forward[index++] = {BLS, shoulder_init};
        com.forward[index++] = {BRS, shoulder_init + move};
        
        com.forward[index++] = {FLK, knee_init + move};
        com.forward[index++] = {FRK, knee_init - legup};
        com.forward[index++] = {BLK, knee_init - legup};
        com.forward[index++] = {BRK, knee_init + move};
        
        com.forward[index++] = {Command::SYN};
        
        com.forward[index++] = {FRK, knee_init};
        com.forward[index++] = {BLK, knee_init};
        
        com.forward[index++] = {Command::SYN};
        
        com.forward[index++] = {FLS, shoulder_init};
        com.forward[index++] = {FRS, shoulder_init + move};
        com.forward[index++] = {BLS, shoulder_init + move};
        com.forward[index++] = {BRS, shoulder_init};

        com.forward[index++] = {FLK,knee_init - legup};
        com.forward[index++] = {FRK, knee_init + move};
        com.forward[index++] = {BLK, knee_init + move};
        com.forward[index++] = {BRK, knee_init - legup};
               
        com.forward[index++] = {Command::SYN};
               
        com.forward[index++] = {FLK, knee_init};
        com.forward[index++] = {BRK, knee_init};
        
        com.forward[index++] = {Command::SYN};
        com.forward[index++] = {Command::END};
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
