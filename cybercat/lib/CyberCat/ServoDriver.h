//
//  ServoDriver.h
//  CyberCat
//
//  Created by Goran Stojanovic on 12/02/2020.
//  Copyright © 2020 embware. All rights reserved.
//

#ifndef ServoDriver_h
#define ServoDriver_h

#include "CommandQueue.h"
#include "PWMServoDriver.h"

#define SERVO_NO    8



struct ServoConfig
{
    Degree min; // min angle allowed
    Degree max; // max angled allowed
    Degree offset; // angle offset for calibration
};

#define DEF_MAX_ANGLE 170
#define DEF_MIN_ANGLE 60

ServoConfig defaultConfig[SERVO_NO] {
    {0,DEF_MAX_ANGLE,0},{0,DEF_MAX_ANGLE,0},{0,DEF_MAX_ANGLE,0},{0,DEF_MAX_ANGLE,0},
    {DEF_MIN_ANGLE,DEF_MAX_ANGLE,0},{DEF_MIN_ANGLE,DEF_MAX_ANGLE,0},{DEF_MIN_ANGLE,DEF_MAX_ANGLE,0},{DEF_MIN_ANGLE,DEF_MAX_ANGLE,0}
};

struct ServoDriver
{
    CommandQueue queue;
    Milliseconds duration;
    ServoSpeed servoSpeed;
    Degree servoAngle[SERVO_NO];
    ServoConfig config[SERVO_NO];
    PWMServoDriver pwmServo;    
    
   
    ServoDriver(ServoSpeed servoSpeed, ServoConfig servoConfig[SERVO_NO]) : servoSpeed {servoSpeed}
    {
        for (int id = 0; id < SERVO_NO; id++)
        {
            config[id]= servoConfig[id];
            servoAngle[id] = 90;
        }
    }
    
    ServoDriver(ServoSpeed servoSpeed)
       : ServoDriver (servoSpeed, defaultConfig)
       {
       }
       
    
    void setup()
    {
        pwmServo.setup();
    }
 
    virtual inline Milliseconds setServoAngle(uint8 servoIndex, Degree angle)
    {
        if (angle >= config[servoIndex].min && angle<= config[servoIndex].max)
        {
            // first engage (write to) servo to minimize delay
            // add calibration offset before calculating to pulse value for servo actuation
            pwmServo.setServoAngle(servoIndex, angle + config[servoIndex].offset);

            Milliseconds delayTime = abs((Milliseconds)(angle - servoAngle[servoIndex])) * servoSpeed/60;
            servoAngle[servoIndex] = angle;
            return delayTime;
        }
        else
        {
            LOG2("ERROR: Servo %d Angle %d out of valid range",servoIndex,angle);
            return 0;
        }
    }
    
    inline void actuate(Command* commands)
    {
        int index = 0;
        Command* command = &commands[index];
        
        while (command->id != Command::END)
        {
            if(command->id == Command::SYN)
            {
                //LOG("com SYN delay:%d", duration + command->value);
                delay(duration + command->value);
                duration = 0;
            }
            else
            {
                //LOG2("com id: %d value:%d",command->id, command->value);
                Milliseconds time = setServoAngle(command->id, command->value);
                if (time > duration)
                {
                    duration = time;
                }
            }
            command = &commands[++index];
        }
        
    }
    
   
    inline void actuate()
    {
        Command *pCommand = queue.pop();
        while (pCommand != nullptr)
        {
            actuate(pCommand);
            pCommand = queue.pop();
        }
    }
    
    void add(Command *commands)
    {
        queue.push(commands);
    }
    
    inline bool idle()
    {
        return queue.empty();
    }

    void calibrate(uint8 servoIndex,Degree angleOffset)
    {
        config[servoIndex].offset = angleOffset;
    }
  
};


#endif /* ServoDriver_h */
