#include <Arduino.h>
#include "AnalogPin.h"

// put function declarations here:
AnalogPin::AnalogPin(int pin){
  _pin = pin;
  _channel = _pin;
  adcInit();
}

void AnalogPin::adcInit(){
  ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
  ADMUX |= (1 << REFS0);
  ADCSRA |= (1 << ADEN);
  ADCSRA |= (1 << ADSC);
}

uint16_t AnalogPin::readADC(){
  ADMUX &= 0xE0;
  ADMUX |= _channel & 0x07;
  ADCSRB = _channel & (1 << 3);
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
  return ADCW;
}