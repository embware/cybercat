
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
    ServoDriver &driver;
    Degree calibrationAngle;

    ServoCalibration(ServoDriver & driver) : servoIndex {0} , driver {driver}, calibrationAngle {(Degree)90}
    {
    }

    void selectServo()
    {
        // Move servo to show it is selected for calibration
        LOG("Calibrate servo [%d]",servoIndex);
        delay(driver.setServoAngle(servoIndex,calibrationAngle - 5));
        delay(100);
        delay(driver.setServoAngle(servoIndex,calibrationAngle));
        delay(100);
    }

    void updateServo()
    {
        // Move servo to show it is selected for calibration
        delay(driver.setServoAngle(servoIndex,calibrationAngle));
        delay(100);
    }

    // Get in calibration position. All servos at 90 angle
    void updateAllServos()
    {
        for (uint8 index =0; index< SERVO_NO; index++)
        {
            driver.setServoAngle(index,calibrationAngle);
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

    void prevServo()
    {
        if (--servoIndex > SERVO_NO) // unsigned int max 255
        {
            servoIndex = SERVO_NO - 1;
        }
    }

    void offsetIncrease()
    {
        Degree offset = driver.config[servoIndex].offset;
        offset++;
        driver.calibrate(servoIndex, offset);
        updateServo();
        LOG2("Selected servo: %d offest: %d",servoIndex, offset);
    }

    void offsetDecrease()
    {
        Degree offset = driver.config[servoIndex].offset;
        offset--;
        driver.calibrate(servoIndex,offset);
        updateServo();
        LOG2("Selected servo: %d offest: %d",servoIndex, offset);
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
