
#ifndef ServoCalibration_h
#define ServoCalibration_h

#include "Types.h"
#include "ServoDriver.h"

// Calibration 
#define EEPROM_SIZE                 512
#define NVM_CALIBRATION_ADDR        0

struct ServoCalibration
{
    uint8 servoIndex;
    Degree offset;
    ServoDriver &driver;

    ServoCalibration(ServoDriver & driver) : servoIndex {0} , offset{0},  driver {driver}
    {
    }

    void selectServo()
    {
        // Move servo to show it is selected for calibration
        driver.setServoAngle(servoIndex,85);
        delay(200);
        driver.setServoAngle(servoIndex,90);
        delay(350);
    }

    void updateServo()
    {
        // Move servo to show it is selected for calibration
        driver.setServoAngle(servoIndex,90);
        delay(350);
    }

    // Get in calibration position. All servos at 90 angle
    void updateAllServos()
    {
        for (uint8 index =0; index< SERVO_NO; index++)
        {
            driver.setServoAngle(index,90);
        }
        delay(350);
    }
      

    void nextServo()
    {
        if (++servoIndex >= SERVO_NO)
        {
            servoIndex = 0;
        }
    }

    void offsetIncrease()
    {
        offset++;
        driver.calibrate(servoIndex,offset);
        updateServo();
    }

    void offsetDecrease()
    {
        offset--;
        driver.calibrate(servoIndex,offset);
        updateServo();
    }

    void nvmRead()
    {
#ifndef CATSIM
        EEPROM.begin(EEPROM_SIZE);

        Degree offsets[SERVO_NO];
        EEPROM.readBytes(NVM_CALIBRATION_ADDR,offsets,sizeof(offsets));
        for (uint8 index = 0; index < SERVO_NO; index++)
        {
            driver.config[index].offset =  offsets[index];
            LOG2("[NVM] Calibration offset [%d] -> %d",index,driver.config[index].offset);
        }
#endif
    }
    
    void nvmWrite()
    {
        
        Degree offsets[SERVO_NO];
        for (uint8 index = 0; index< SERVO_NO; index++)
        {
            offsets[index] = driver.config[index].offset;
#ifndef CATSIM
        EEPROM.writeBytes(NVM_CALIBRATION_ADDR,offsets,sizeof(offsets));
        EEPROM.commit();
        LOG("[NVM] Save calibration configuration. (%d bytes)", sizeof(offsets));
#endif
        }
    }
          
};

#endif // ServoCalibration_h 
