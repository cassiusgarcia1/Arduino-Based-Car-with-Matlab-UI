#ifndef AnalogPin_h
#define AnalogPin_h

#include <Arduino.h>

class AnalogPin{
    public:
        AnalogPin(int pin);
        void adcInit();
        uint16_t readADC();
    private:
        int _pin;
        uint8_t _channel;
};

#endif