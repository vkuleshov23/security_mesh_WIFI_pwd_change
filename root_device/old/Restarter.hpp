#ifndef _Restarter_
#define _Restarter_

#include <Arduino.h>
#include "Timer.h"

class Restarter{
protected:
    bool reboot = false;
    Timer timer;

    bool isCanReboot() {
        if(timer == 0)
            return reboot;
        return false;
    }

public:
    Restarter(){};
    void needReboot(unsigned long millis = 0) {
        this->timer.start(millis);
        this->reboot = true;
    }
    void update() {
        if(isCanReboot()) {
            rebootDevice();
        }
    }
    void rebootDevice() {
        ESP.restart();
    }
};

#endif