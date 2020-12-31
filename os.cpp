/*
 * File:   os.cpp
 * Author: TristanH
 *
 * Created 8th March 2019
*/

#include <Arduino.h>
#include "os.hpp"

#define _TASK_LENGTH 32 // power of 2 <= 64
#define _SYSTEM_TIME() micros()
#define MAX_SCHEDULE 1000000000U // 100 s
#define _INCREMENT(var) var = (var + 1) & (_TASK_LENGTH - 1)

void toggleBlink();

typedef unsigned long ostime_t;
typedef struct {
    void (*f)();
    ostime_t t;
} Task;

struct {
    Task tasks[_TASK_LENGTH];
    void (*put)(Task);
    char (*isEmpty)();
    Task (*get)();
    char (*isFull)();
    int _putInd = 0;
    int _getInd = 0;
    char size = 0;
} OS;

void OS_put(Task elem) {
    OS.tasks[OS._putInd] = elem;
    _INCREMENT(OS._putInd);
    OS.size += 1;
}

Task OS_get() {
    Task val;
    val = OS.tasks[OS._getInd];
    _INCREMENT(OS._getInd);
    OS.size -= 1;
    return val;
}

char OS_isFull() { return OS.size == _TASK_LENGTH; }

char OS_isEmpty() { return OS._getInd == OS._putInd; }

void initOS() {
    OS.put = OS_put;
    OS.get = OS_get;
    OS.isEmpty = OS_isEmpty;
    OS.isFull = OS_isFull;
    OS._putInd = 0;
    OS._getInd = 0;
    OS.size = 0;
}

void run_later(void (*func)(), ostime_t delay) {
    // micros() is microseconds since program start
    // microsecond resolution (on 16MHz, resolution is 4 us) 
    // will overflow after about 70 mins
    // millis() is milliseconds and overflows after about 50 days
    ostime_t taskTime = _SYSTEM_TIME() + delay*1000;
    Task task = {func, taskTime};
    OS.put(task);
}

void run() {
    Task task, next;
    ostime_t dif;

    while (1) {
        while (OS.isEmpty());
        next = OS.get(); //task on the buffer that should be executed soonest
        OS.put(next);
        while (1) {
            task = OS.get(); //get task
            if (task.f == next.f && task.t == next.t) break;
            
            //check if task should occur sooner than next
            //task.t < next.t, allowing for overflow
            dif = next.t - task.t;
            if (
                (dif > 0 && dif < MAX_SCHEDULE) ||  //next > task, but not by too much
                (dif < -MAX_SCHEDULE)
            ) { next = task; }
            OS.put(task);
        }

        dif = _SYSTEM_TIME() - next.t;
        //if task.t is scheduled before current time
        if (
            (dif > 0 && dif < MAX_SCHEDULE) || //sysTime > task, but not by too much
            (dif < -MAX_SCHEDULE)
        ) { next.f(); }
        else {
            OS.put(next);
        }
    }
}

void dream(short hrs, short mins, short secs) {
    #if defined(TEENSY)
    //Set RTC alarm wake up in (hours, minutes, seconds)
    alarm.setRtcTimer(hrs, mins, secs);
    Snooze.hibernate( config_teensy36 );
    
    #else // Arduino
    unsigned int seconds = hrs*3600 + mins*60 + secs;
    delay(seconds*1000);
    #endif 
}

void initBlink(){
    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN,HIGH);
    run_later(toggleBlink,0);
}

void toggleBlink() {
    short toggle_delay = 250; // ms
    run_later(toggleBlink,toggle_delay);
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
}