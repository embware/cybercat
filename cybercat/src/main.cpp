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


int counter = 0;


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
    // Must set some delay because loop overtake time and core 0 task watchdog is trigered
    delay(10); 

    if (! state.check(STATE_CONNECTED))
    {
      state.set(STATE_CONNECTED);

      LOG("%s","PS4 connected...");
      PS4.setLed(0,0,0xff);
      PS4.setRumble(0xff, 0x00);
      PS4.sendToController();
      delay(250);
      PS4.setLed(0x10,0,0);
      PS4.setRumble(0x0,0x0);
      PS4.sendToController();
      // Bring cat up now
      cat.up();
      return;
    }

    if (!state.check(STATE_CALIBRATION) && PS4.data.button.options)
    {
      // Enter calibration mode
      state.set(STATE_CALIBRATION);

      PS4.setLed(0xff,0,0);
      PS4.sendToController();
      // Get in calibration position. All servos at 90 angle
      calibration.updateAllServos();
      calibration.selectServo();
    }

    if (state.check(STATE_CALIBRATION))
    {
      if (PS4.data.button.options)
      {
        calibration.nextServo(); 
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
      else if ( PS4.data.button.cross)
      {
        // Exit calibration mode
        calibration.nvmWrite(); // Store calibration config to be set on device start
        state.clear(STATE_CALIBRATION);
        PS4.setLed(0x10,0,0);
        PS4.sendToController();
        cat.up();
      }
      return;
    }
    
    if (state.check(STATE_CONNECTED))
    {
      if (PS4.data.button.right )
      {
        LOG("%s","Cat right!");
        cat.up();
        delay(200);
        
      }
      else if ( PS4.data.button.left )
      {
        LOG("%s","Cat left!");
        cat.down();
        delay(200);
      }
      else if ( PS4.data.button.up )
      {
        LOG("%s","Cat forward!");
        cat.forward();
        delay(200);
      }
      else if ( PS4.data.button.down )
      {
        LOG("%s","Cat backward!");
        cat.backward();
        delay(200);
      }
      else if (abs(PS4.data.analog.stick.ly) > 10) 
      { 
        LOG("Left Stick | y = %d", PS4.data.analog.stick.ly);
        delay(200);  
      }
      else if (cat.idle())
      {
        //LOG("Cat idle | height %d mm" , cat.heightFront());
        cat.standby();
        delay(200);
      }   
    }    
  }
  else
  {
    
    // This delay is to make the Serial Print more human readable
    // Remove it when you're not trying to see the output
    delay(500);
    if (counter % 20 == 0)
    {
      state.set(STATE_OFFLINE);
      LOG("Check @ core_%d",xPortGetCoreID());
    } 
  }  
}
