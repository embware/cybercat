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
Degree startAngles[8] {150,150,150,150,60,60,60,60};

ServoDriver driver {servoSpeed,startAngles};
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
      delay(10); // Must set some delay because loop overtake time and core 0 task watchdog is trigered
      if ( PS4.data.button.up )
      {
          LOG("%s","Cat up!");
          cat.up();
          delay(100);
      }
      
      if ( PS4.data.button.down )
      {
        LOG("%s","Cat down!");
        cat.down();
        delay(100);
      }
          

      if ( PS4.data.button.right )
      {
        LOG("%s","Cat run!");
        cat.run();
        delay(250);
      }

      if ( PS4.data.button.left )
      {
        LOG("%s","Cat walk!");
        cat.walk();
        delay(250);
      }
      
     
     if ( PS4.data.button.circle )
      {
        LOG("%s","Cat bounce!");
        cat.bounce();
      
        delay(100);
      }

       if ( PS4.data.button.square )
      {
        LOG("%s","Cat up-down-up!");
        cat.up();
        cat.down();
         cat.up();
        cat.down();
        cat.up();
      
        delay(100);
      }

      if ( PS4.data.button.triangle )
      {
        LOG("%s","Cat rumble!");
        PS4.setRumble(1, 255);
        PS4.sendToController();
        delay(250);
        PS4.setRumble(0, 0);
        PS4.sendToController();
        delay(50);
        PS4.setRumble(1, 255);
        PS4.sendToController();
        delay(250);
        PS4.setRumble(0, 0);
        PS4.setLed(66,0,0);
        PS4.sendToController();
        delay(1000);
      }
          
      if (PS4.data.button.r2)
      {
         LOG("Batt: %d / 16",PS4.data.status.battery);
         
         delay(1000);
      }
     

      if (abs(PS4.data.analog.stick.lx) > 10 ) 
      {
          LOG("Stick left x= %d",PS4.data.analog.stick.lx);
          delay(100);
          
      }
      if ( abs(PS4.data.analog.stick.ly) > 10 ) 
      {
          LOG("Stick left y= %d",PS4.data.analog.stick.ly);
          delay(100);
      }
      
      if ( abs(PS4.data.analog.stick.rx) > 10 ) 
      {
          cat.down();
          LOG("Stick right x= %d",PS4.data.analog.stick.rx);
          delay(1000);
      }
      if ( abs(PS4.data.analog.stick.ry) > 10 ) 
      {
          cat.up();
          LOG("Stick right y= %d",PS4.data.analog.stick.ry);
          delay(1000);
          
      }
  }
  else
  {
     
     // This delay is to make the Serial Print more human readable
     // Remove it when you're not trying to see the output
     delay(5000);
    
     LOG("Loop @core %d",xPortGetCoreID());
   }
  
 
  
}
