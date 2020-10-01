//
//  PWMDriver.h
//  CyberCat
//
//  Created by Goran Stojanovic on 19/05/2020.
//  Copyright © 2020 embware. All rights reserved.
//


#ifndef PWMServoDriver_h
#define PWMServoDriver_h

#define MIN_PULSE_WIDTH 110
#define MAX_PULSE_WIDTH 520
#define FREQUENCY 50

#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180


#include "Types.h"

#define CUSTOM_SDA_SCL

#ifdef CATHARDWARE
    #include <Wire.h>
    #include <Adafruit_PWMServoDriver.h>

    Adafruit_PWMServoDriver *pwm;

    #define CUSTOM_SDA_SCL_PINS


    #ifdef CUSTOM_SDA_SCL_PINS
    #define CUSTOM_SDA 21
    #define CUSTOM_SCL 13
    TwoWire i2cWire = TwoWire(0);
    #endif // CUSTOM_SDA_SCL_PINS
#endif



struct PWMServoDriver
{
    int angle2pwm[SERVO_MAX_ANGLE+1];
    
    PWMServoDriver()
    {
        Degree pulseAngle;
        for (pulseAngle = SERVO_MIN_ANGLE; pulseAngle <= SERVO_MAX_ANGLE; pulseAngle ++)
        {
            angle2pwm[pulseAngle] = map_prop (pulseAngle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
        }
    }
    
    void setup()
    {
#ifdef CATHARDWARE
    #ifdef CUSTOM_SDA_SCL_PINS
        i2cWire.begin(CUSTOM_SDA,CUSTOM_SCL,100000);
        pwm = new Adafruit_PWMServoDriver(0x40,i2cWire);
    #else
        pwm = new Adafruit_PWMServoDriver();
    #endif // CUSTOM_SDA_SCL_PINS
        pwm->begin();
        pwm->setPWMFreq(FREQUENCY);
#endif
    }
    
    
    inline void setServoAngle(uint8 servoIndex, Degree angle)
    {
        //LOG3("Servo:%d angle:%d pulse:%d",servoIndex,angle,angle2pwm[angle]);
#ifdef CATHARDWARE
        pwm->setPWM(servoIndex * 2, 0, angle2pwm[angle]); // (* 2) - to use every second pin on pwd driver connector
#endif
    }
};

#endif /* PWMServoDriver_h */
