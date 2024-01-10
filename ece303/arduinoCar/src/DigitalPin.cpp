#include <Arduino.h>
#include "DigitalPin.h"

// put function declarations here:
DigitalPin::DigitalPin(int pin){
  _pin = pin;
  this->set_pin();
}

void DigitalPin::set_pin(){

  uint8_t pinPositionMap[54] = 
  {0, 1, 4, 5, 5, 3, 3, 4,
  5, 6, 4, 5, 6, 7, 1, 0,
  1, 0, 3, 2, 1, 0, 0, 1,
  2, 3, 4, 5, 6, 7, 7, 6,
  5, 4, 3, 2, 1, 0, 7, 2,
  1, 0, 7, 6, 5, 4, 3, 2,
  1, 0, 3, 2, 1, 0};

  volatile uint8_t *portRegisterMap[54] = 
  {&PORTE, &PORTE, &PORTE, &PORTE, &PORTG, &PORTE, &PORTH, &PORTH,
  &PORTH, &PORTH, &PORTB, &PORTB, &PORTB, &PORTB, &PORTJ, &PORTJ,
  &PORTH, &PORTH, &PORTD, &PORTD, &PORTD, &PORTD, &PORTA, &PORTA,
  &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTC, &PORTC,
  &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, &PORTD, &PORTG,
  &PORTG, &PORTG, &PORTL, &PORTL, &PORTL, &PORTL, &PORTL, &PORTL,
  &PORTL, &PORTL, &PORTB, &PORTB, &PORTB, &PORTB};

  volatile uint8_t *ddrRegisterMap[54] = 
  {&DDRE, &DDRE, &DDRE, &DDRE, &DDRG, &DDRE, &DDRH, &DDRH,
  &DDRH, &DDRH, &DDRB, &DDRB, &DDRB, &DDRB, &DDRJ, &DDRJ,
  &DDRH, &DDRH, &DDRD, &DDRD, &DDRD, &DDRD, &DDRA, &DDRA,
  &DDRA, &DDRA, &DDRA, &DDRA, &DDRA, &DDRA, &DDRC, &DDRC,
  &DDRC, &DDRC, &DDRC, &DDRC, &DDRC, &DDRC, &DDRD, &DDRG,
  &DDRG, &DDRG, &DDRL, &DDRL, &DDRL, &DDRL, &DDRL, &DDRL,
  &DDRL, &DDRL, &DDRB, &DDRB, &DDRB, &DDRB};

  volatile uint8_t *pinRegisterMap[54] = 
  {&PINE, &PINE, &PINE, &PINE, &PING, &PINE, &PINH, &PINH,
  &PINH, &PINH, &PINB, &PINB, &PINB, &PINB, &PINJ, &PINJ,
  &PINH, &PINH, &PIND, &PIND, &PIND, &PIND, &PINA, &PINA,
  &PINA, &PINA, &PINA, &PINA, &PINA, &PINA, &PINC, &PINC,
  &PINC, &PINC, &PINC, &PINC, &PINC, &PINC, &PIND, &PING,
  &PING, &PING, &PINL, &PINL, &PINL, &PINL, &PINL, &PINL,
  &PINL, &PINL, &PINB, &PINB, &PINB, &PINB};

  _portX = portRegisterMap[_pin];
  _ddrX = ddrRegisterMap[_pin];
  _pinBitPosition = pinPositionMap[_pin];
  _pinX = pinRegisterMap[_pin];
  _pinBit = (uint8_t) pow(2,_pinBitPosition)+1;
}

void DigitalPin::set_input_mode(){
  *_ddrX &= !_pinBit;
}

void DigitalPin::set_output_mode(){
  *_ddrX |= _pinBit;
}

void DigitalPin::on(){
  *_portX |= _pinBit;
}

void DigitalPin::off(){
  *_portX &= !_pinBit;
}

void DigitalPin::invert_pin(){
  if(this->digitalRead() == 1)  this->off();
  else                          this->on();
}

void DigitalPin::set_TCCRA(volatile uint8_t* reg, uint8_t value) {
  *reg = value;
}

    // Set TCCRB register based on the associated pin
void DigitalPin::set_TCCRB(volatile uint8_t* reg, uint8_t value) {
  *reg = value;
}

    // Set TCNT register based on the associated pin
void DigitalPin::set_TCNT(volatile uint16_t* reg, uint16_t value) {
  *reg = value;
}

    // Set OCR register based on the associated pin
void DigitalPin::set_OCR(volatile uint16_t* reg, uint16_t value) {
  *reg = value;
}

void DigitalPin::set_ICR(volatile uint16_t* reg, uint16_t value) {
  *reg = value;
}

void DigitalPin::set_duty_cycle(volatile uint16_t* ocr,volatile uint16_t* icr, float dutyCycle) {
  *ocr = *icr * dutyCycle;
}

    // Factor OCR register based on the associated pin
volatile void DigitalPin::factor_OCR(volatile uint16_t* reg, double factor) {
  *reg = *reg * factor;
}

    // Set TIMSK register based on the associated pin
void DigitalPin::set_TIMSK(volatile uint8_t* reg, uint8_t value) {
  *reg = value;
}

int DigitalPin::digitalRead(){
  int read;
  read = (*_pinX >> _pinBitPosition & _pinBit >> _pinBitPosition);
  return read;
}

void DigitalPin::printPos(){
  Serial.println(_pinBitPosition);
  Serial.println(_pinBit);
}

int DigitalPin::getPin(){
  return _pin;
}
