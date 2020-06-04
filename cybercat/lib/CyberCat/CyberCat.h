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
    Len boneLength = 200;
  
    CyberCat(ServoDriver& driver) : driver {driver}
    {
       
    }
    
    static uint8 KA(uint8 shoulderAngle,uint8 height, uint8 distance )
    {
        return KA(shoulderAngle,height/distance);
    }
    
    static uint8 KA(uint8 shoulderAngle,double hdRatio = sqrt(2))
    {
        // formula:
        //  beta = 180 - angle + (double)asin(height/boneLenth - TrigCache.sinus[angle]) * 180 / TrigCache.PI;
        //  beta = 180 - angle + (double)asin(hd - TrigCache.sinus[angle]) * 180 / TrigCache.PI;
        return 180 - shoulderAngle + TrigCache.degree(sin(hdRatio - TrigCache.sinus[shoulderAngle]));
    }
    
    
    struct
    {
        Command up[10]
        {
            {FLS,135},{FRS,135},{BRS,135},{BLS,135},
            {FLK,90},{FRK,90},{BRK,90},{BLK,90},
            {Command::SYN},{Command::END}
        };
        Command down[10]
        {
            {FLS,155},{FRS,155},{BRS,155},{BLS,155},
            {FLK,50},{FRK,50},{BRK,50},{BLK,50},
            {Command::SYN},{Command::END}
        };
        
        Command height[10];
        
        
        Command bounce[20]
        {
            {FLS,120},{FLK,60},{BLS,135},{BLK,KA(115)},
            {FRS,115},{FRK,KA(115)},{BRS,120},{BRK,60},
            {Command::SYN},
            {FLS,115},{FLK,KA(115)},{BLS,120},{BLK,60},
            {FRS,120},{FRK,60},{BRS,115},{BRK,KA(115)},
            {Command::SYN},{Command::END}
        };

        
        Command walk[21]
        {
            {FLS,145},{BRS,145},{FRS,135},{BLS,135},
            {FRK, 85},{BLK, 85},
            {Command::SYN},
            {FRK, 90},{BLK, 90},
            {Command::SYN},
            {FLS,135},{BRS,135},{FRS,145},{BLS,145},
            {FLK, 85},{BRK, 85},
            {Command::SYN},
            {FLK, 90},{BRK, 90},
            {Command::SYN},
            {Command::END}
        };
        
        
        Command walk_end[9]
        {
            {FRS,135},{BLS,135},
            {FRK, 85},{BLK, 85},
            {Command::SYN},
            {FRK, 90},{BLK, 90},
            {Command::SYN},
            {Command::END}
        };
        
        Command run[21]
        {
            {FLS,150},{BRS,150},{FRS,130},{BLS,130},
            {FRK, 85},{BLK, 85},
            {Command::SYN},
            {FRK, KA(130)},{BLK, KA(130)},
            {Command::SYN},
            {FLS,130},{BRS,130},{FRS,150},{BLS,150},
            {FLK, 85},{BRK, 85},
            {Command::SYN},
            {FLK, KA(130)},{BRK, KA(130)},
            {Command::SYN},
            {Command::END}
        };
        
        Command run_end[9]
        {
            {FRS,130},{BLS,130},
            {FRK, 85},{BLK, 85},
            {Command::SYN},
            {FRK, KA(130)},{BLK, KA(130)},
            {Command::SYN},
            {Command::END}
        };
        
        
    } com;
    
    
    void up()
    {
        driver.add(com.up);
    }
    
    void down()
    {
        driver.add(com.down);
    }
    
    void bounce(int count = 5)
    {
        driver.add(com.up);
        while(--count > 0)
        {
            driver.add(com.bounce);
        }
        driver.add(com.up);
    }
    
    void walk(int count = 5)
    {
        driver.add(com.up);
        while(--count > 0)
        {
            driver.add(com.walk);
        }
        driver.add(com.walk_end);
    }
    
    void run(int count = 5)
    {
        driver.add(com.up);
        while(--count > 0)
        {
            driver.add(com.run);
        }
        driver.add(com.run_end);
    } 
    
    void height(Len height)
    {
        const Degree angle = TrigCache.degree(asin((double)height / 2 / boneLength));
        const Degree sa = 180 - angle;
        const Degree ka = 2 * angle;
        
        com.height[0] = {FLS,sa};
        com.height[1] = {BRS,sa};
        com.height[2] = {FRS,sa};
        com.height[3] = {BLS,sa};
        com.height[4] = {FLK,ka};
        com.height[5] = {BRK,ka};
        com.height[6] = {FRK,ka};
        com.height[7] = {BLK,ka};
        com.height[8] = {Command::END};
        com.height[9] = {Command::END};
        LOG("Set height %d mm" , height);
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
    
    
    // TODO API Calls ..
    
    // Len heightFront();
    
    // Len heightBack();
   
};

#endif /* CyberCat_h */
