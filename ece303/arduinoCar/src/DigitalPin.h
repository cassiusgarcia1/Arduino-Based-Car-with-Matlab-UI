#ifndef DigitalPin_h
#define DigitalPin_h

#include <Arduino.h>

class DigitalPin{
    public:
        DigitalPin(int pin);
        void set_pin();
        void set_input_mode();
        void set_output_mode();
        void on();
        void off();
        void invert_pin();
        void set_TCCRA(volatile uint8_t* reg, uint8_t value);
        void set_TCCRB(volatile uint8_t* reg, uint8_t value);
        void set_TCNT(volatile uint16_t* reg, uint16_t value);
        void set_OCR(volatile uint16_t* reg, uint16_t value);
        void set_ICR(volatile uint16_t* reg, uint16_t value);
        void set_duty_cycle(volatile uint16_t* ocr,volatile uint16_t* icr, float dutyCycle);
        volatile void factor_OCR(volatile uint16_t* reg, double factor);
        void set_TIMSK(volatile uint8_t* reg, uint8_t value);
        int getPin();
        int digitalRead();
        void printPos();
    private:
        int _pin;
        int _pinBit;
        uint8_t _pinBitPosition;
        volatile uint8_t *_tccrA;
        volatile uint8_t *_tccrB;
        volatile uint8_t *_portX;
        volatile uint8_t *_ddrX;
        volatile uint8_t *_pinX;
};

#endif