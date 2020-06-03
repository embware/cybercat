//
//  Types.h
//  CyberCat
//
//  Created by Goran Stojanovic on 07/02/2020.
//  Copyright © 2020 embware. All rights reserved.
//

#ifndef Types_h
#define Types_h

#include <math.h>

#ifndef uint8
typedef uint8_t uint8;
#endif

#ifndef nullptr
#define nullptr 0
#endif

const double PI = 3.141592653589793238463;

// Instead of Arduino map function (this is more precise)
inline static int map_prop (int x, int in_min, int in_max, int out_min, int out_max)
{
    double value = (x - in_min) * (out_max - out_min);
    value = value / (in_max - in_min) + out_min; 
    return round(value);
}

#ifdef CATSIM

#include <thread>
#define delay(time_ms) std::this_thread::sleep_for(std::chrono::milliseconds((time_ms)))


std::mutex mutex{};

struct Servo { void write(int angle) {} };

#define LOCK    mutex.lock()
#define UNLOCK  mutex.unlock()


#define LOG(form,msg) printf(form,msg); printf("\r\n")
#define LOG2(form,msg1,msg2) printf(form,msg1,msg2); printf("\r\n")
#define LOG3(form,msg1,msg2,msg3) printf(form,msg1,msg2,msg3); printf("\r\n")

#endif

#ifndef CATSIM

#include "FreeRTOS.h"
#include "freertos/task.h"

#define CATHARDWARE ESP32

portMUX_TYPE mutex = portMUX_INITIALIZER_UNLOCKED;
 
#define LOCK    portENTER_CRITICAL(&mutex)
#define UNLOCK  portEXIT_CRITICAL(&mutex)
//#define LOCK    vTaskSuspendAll()
//#define UNLOCK  xTaskResumeAll()

#define LOG_TAG "CAT"
#define LOG(format,msg) ESP_LOGD(LOG_TAG,format,msg)
#define LOG2(format,msg1,msg2) ESP_LOGD(LOG_TAG,format,msg1,msg2)
#define LOG3(format,msg1,msg2,msg3) ESP_LOGD(LOG_TAG,format,msg1,msg2,msg3)
#endif

typedef int Degree; // [0,180] degrees

typedef int Len; // [0,250] mm? or cm

typedef int ServoSpeed; // msec/60 degre

typedef int Milliseconds;

#endif /* Types_h */
