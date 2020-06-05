#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#include "esp_log.h"
#include "CyberCat.h"
#include <PS4Controller.h>

#define DRIVER_TASK_NAME "DriverTask"
#define DRIVER_TASK_PRIORITY  (0) /* Need to be 0 <- highest priority */
#define DRIVER_TASK_STACK_SIZE  16*1024 /* Bytes */
#define DRIVER_TASK_HANDLE  NULL
#define DRIVER_TASK_CORE (0)

#define PS4_CONTROLLER_MAC (char*) "aa:bb:cc:dd:ee:ff" 

void driverTask(void *ptrParam)
{
  LOG("DriverTask @core %d",xPortGetCoreID());
  ServoDriver *pdriver = (ServoDriver*) ptrParam;
  for(;;)
  {
      pdriver->actuate();
  }
}

ServoSpeed servoSpeed = 300;
ServoDriver driver {servoSpeed};
CyberCat cat {driver};

void setup() 
{


  LOG2("  RAM: %d/%d KB",ESP.getFreeHeap()/1024,ESP.getHeapSize()/1024);
  LOG2("PSRAM: %d/%d KB",ESP.getFreePsram()/1024, ESP.getPsramSize()/1024);

  
  LOG("Setup PS4 Controller MAC: %s",PS4_CONTROLLER_MAC);
  PS4.begin(PS4_CONTROLLER_MAC);
  
  LOG("%s","Setup driver.");
  driver.setup();
  
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
  if(PS4.isConnected()) 
  {  
      // Must set some delay because loop overtake time and core 0 task watchdog is trigered
      delay(10); 

      if ( PS4.data.button.up )
      {
          LOG("%s","Cat up!");
          cat.up();
          delay(200);
      }
      
      if ( PS4.data.button.down )
      {
        LOG("%s","Cat down!");
        cat.down();
        delay(200);
      }
          
      if ( PS4.data.button.right )
      {
        LOG("%s","Cat run!");
        cat.run();
        delay(200);
      }

      if ( PS4.data.button.left )
      {
        LOG("%s","Cat walk!");

        cat.walk();
        delay(200);
      }
      
     if ( PS4.data.button.circle )
      {
        LOG("%s","Cat bounce!");
        cat.bounce();
        delay(200);
      }

      if ( PS4.data.button.square )
      {
        LOG("%s","Cat up-down-up=down-up!");
        cat.up();
        cat.down();
        cat.up();
        cat.down();
        cat.up();      
        delay(200);
      }

      if ( PS4.data.button.cross )
      {
        LOG("%s","Cat forward!");
        cat.forward();
        delay(200);
      }

      if ( PS4.data.button.triangle )
      {
        LOG("%s","Cat rumble!");
        PS4.setLed(0x10,0,0);
        PS4.setRumble(1, 0xff);
        PS4.sendToController();
        delay(350);
        PS4.setRumble(0xff, 0);
        PS4.sendToController();
        delay(350);
        PS4.setRumble(0, 0);
        PS4.sendToController();
        
      }
          
      if (PS4.data.button.r2)
      {
         LOG("Batt: %d / 16",PS4.data.status.battery);
         cat.height(300);
         delay(200);
      }
    
      if (abs(PS4.data.analog.stick.ly) > 10) 
      {
        if (cat.idle())
        {
          LOG("Left Stick | y = %d", PS4.data.analog.stick.ly);          
          // stick.ly --->  value from (-0x7F) to (+0x7F)          
          cat.height(cat.heightMin + (127 + PS4.data.analog.stick.ly) * (cat.heightMax  - cat.heightMin) / 255, false);
          delay(200);
        }

      }
      
      if ( abs(PS4.data.analog.stick.rx) > 10 ) 
      {
        if (cat.idle())
        {        
          LOG("Right Stick | x= %d",PS4.data.analog.stick.rx);
          cat.down();
        }
         
      }

      if ( abs(PS4.data.analog.stick.ry) > 10 ) 
      {
        if (cat.idle())
        {
          LOG("Right Stick | y= %d",PS4.data.analog.stick.ry);
          cat.up();
        } 
      }
  }
  else
  {
     
     // This delay is to make the Serial Print more human readable
     // Remove it when you're not trying to see the output
     delay(5000);
    
     LOG("Check in @core %d ...",xPortGetCoreID());
   }  
}
