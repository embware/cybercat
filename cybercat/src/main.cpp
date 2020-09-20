#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#include "esp_log.h"
#include "CyberCat.h"
#include <PS4Controller.h>

#define DRIVER_TASK_NAME "DriverTask"
#define DRIVER_TASK_PRIORITY  (0) /* Need to be 0 <- highest priority */
#define DRIVER_TASK_STACK_SIZE  32*1024 /* Bytes */
#define DRIVER_TASK_HANDLE  NULL
#define DRIVER_TASK_CORE (0)

#define PS4_CONTROLLER_MAC (char*) "aa:bb:cc:dd:ee:ff" 


uint8 counter = 0;


ServoSpeed servoSpeed = 300;
ServoDriver driver {servoSpeed};
CyberCat cat {driver};

ServoCalibration calibration {driver};
StateManager state {};


void driverTask(void *ptrParam)
{
  LOG("DriverTask @core %d",xPortGetCoreID());
  ServoDriver *pdriver = (ServoDriver*) ptrParam;
  for(;;)
  {
      pdriver->actuate();
  }
}

void setup() 
{
  LOG2("  RAM: %d/%d KB",ESP.getFreeHeap()/1024,ESP.getHeapSize()/1024);
  LOG2("PSRAM: %d/%d KB",ESP.getFreePsram()/1024, ESP.getPsramSize()/1024);

  
  LOG("Setup PS4 Controller MAC: %s",PS4_CONTROLLER_MAC);
  PS4.begin(PS4_CONTROLLER_MAC);
  
  LOG("%s","Setup driver.");
  
  driver.setup();
  calibration.nvmRead();  //  Read calibration config from NVM / EEPROM
  state.set(STATE_OFFLINE);
  
  
  LOG("%s","Start driver task");
  xTaskCreatePinnedToCore(driverTask, /* Function to implement the task */
              DRIVER_TASK_NAME,       /* Name of the task */
              DRIVER_TASK_STACK_SIZE, /* Stack size in words */
              &driver,                /* Task input parameter */
              DRIVER_TASK_PRIORITY,   /* Priority of the task */
              DRIVER_TASK_HANDLE,     /* Task handle. */
              DRIVER_TASK_CORE);      /* Core where the task should run */

  LOG("%s","Setup finished!");
}


void loop() 
{
  counter++;

  if(PS4.isConnected()) 
  {   
   
    if (state.check(STATE_STANDBY))
    {
      if (PS4.data.analog.stick.ly > 10)
      {
        cat.walk_forward();
        state.set(STATE_FORWARD);
      }
      else if (PS4.data.analog.stick.ly < -10)
      {
        cat.walk_backward();
        state.set(STATE_BACKWARD);
      }
      else if (PS4.data.button.up )
      {
        cat.up();
        state.set(STATE_UP);
      }
      else if (PS4.data.button.down )
      {
        cat.down();
        state.set(STATE_DOWN);
      }
    }
    if (state.check(STATE_FORWARD))
    {
        if (cat.idle())
        {
          if (PS4.data.analog.stick.ly > 10)
          {
            cat.walk_forward();
            state.set(STATE_FORWARD);
          }
          else
          {
            cat.standby();
            state.set(STATE_STANDBY);
          }
        }
    }
    if (state.check(STATE_BACKWARD))
    {
        if (cat.idle())
        {
          if (PS4.data.analog.stick.ly < -10)
          {
            cat.walk_backward();
            state.set(STATE_BACKWARD);
          }
          else
          {
            cat.standby();
            state.set(STATE_STANDBY);
          }
        }
    }
    if (state.check(STATE_UP))
    {
        if (cat.idle())
        {
          if (PS4.data.button.up )
          {
            cat.up();
            state.set(STATE_UP);
          }
          else
          {
            cat.standby();
            state.set(STATE_STANDBY);
          }
        }
    }
    if (state.check(STATE_DOWN))
    {
        if (cat.idle())
        {
          if (PS4.data.button.down )
          {
            cat.down();
            state.set(STATE_DOWN);
          }
          else
          {
            cat.standby();
            state.set(STATE_STANDBY);
          }
        }
    }
    if (state.check(STATE_OFFLINE))
    {
      LOG("%s","PS4 connected...");
      PS4.setRumble(0x00, 0xFF);
      PS4.setLed(0x10,0,0);
      PS4.sendToController(); 
      PS4.setRumble(0x00, 0x00);  
      PS4.sendToController();
      calibration.updateAllServos();
      state.set(STATE_CONNECTED);
    }

    if (state.check(STATE_CONNECTED))
    {
      if (PS4.data.button.triangle)
      {
        cat.up();
        state.set(STATE_STANDBY);
      }
      if (PS4.data.button.options)
      {
        state.set(STATE_CALIBRATION);
        // Enter calibration mode   
        PS4.setLed(0xff,0,0);
        PS4.sendToController();
        // Get in calibration position. All servos at 90 angle
        calibration.updateAllServos();
        calibration.selectServo();
      } 
    }
    

    if (state.check(STATE_CALIBRATION))
    {
      if (PS4.data.button.options)
      {
        calibration.nextServo(); 
        calibration.selectServo();
      }
      else if (PS4.data.button.share)
      {
        calibration.prevServo(); 
        calibration.selectServo();
      }
      else if ( PS4.data.button.up )
      {
        calibration.offsetIncrease();
      }
      else if ( PS4.data.button.down )
      {
          calibration.offsetDecrease();
      }
      else if ( PS4.data.button.circle)
      {
        // Exit calibration mode
        LOG("%s","Store calibration config");
        calibration.nvmWrite(); // Store calibration config to be set on device start
        PS4.setLed(0x10,0,0);
        PS4.sendToController();
        cat.up();
        state.set(STATE_STANDBY);
      }
      else if ( PS4.data.button.cross)
      {
        // Cancel calibration mode
        LOG("%s","Cancel calibration mode");
        PS4.setLed(0x10,0,0);
        PS4.sendToController();
        cat.up();
        state.set(STATE_STANDBY);
      }
    }
  }
  else
  {
    
    // This delay is to make the Serial Print more human readable
    // Remove it when you're not trying to see the output
    delay(100);
    if (counter++ % 30 == 0)
    {
      state.set(STATE_OFFLINE);
      LOG("Check @ core_%d",xPortGetCoreID());
    } 
  }
  // IMPORTANT !
  // Must set some delay because loop overtake time and core 0 task watchdog is trigered
  delay(10); 
   
}
