#include <Arduino.h>
#include <DHT.h>
#include <SPI.h>
#include <MFRC522.h>
#include <stdbool.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <liquidCrystal.h>
#include <Wire.h>
#include <Arduino.h>
#include "DigitalPin.h"
#include "AnalogPin.h"

#define SS_PIN 53
#define RST_PIN 6
#define DHT_PIN 8
#define WATER_POWER_PIN 22
#define WATER_SIGNAL_PIN A0 //analog pin
#define IR_IN_PIN 9
#define trigPin 46     // TRIG pin
#define echoPin 47   // ECHO pin
#define forward 4   //forward pin for motor
#define backward 5  //backward pin for motor
#define RED 44
#define YELLOW 43
#define GREEN 42
#define HEADLIGHTS 11
#define TEMP_MAX 90
#define WATER_LVL_MAX 250
#define VOLUME_UP 70 //Define the volume key
#define PAUSE_PLAY 64 //Define the pause/play key

DigitalPin red(RED);
DigitalPin yellow(YELLOW);
DigitalPin green(GREEN);
DigitalPin triggerPin(trigPin);
DigitalPin echoPinO(echoPin);
DigitalPin waterPower(WATER_POWER_PIN); 
AnalogPin waterData(WATER_SIGNAL_PIN);


int outVal = 0;
int inVal = 0;
int coolantLevel = 0;
int coolantLevelAlarm = 0;
int temperatureAlarm = 0;
int headlightValue = 1023;

uint16_t command;
float distance, temperatureC, temperatureF, motorSpeed;
bool locked = true;
char val;
char temp;

LiquidCrystal lcd(2,3,12,38,36,34);

DHT dht;
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  //motor pins
  pinMode(forward, OUTPUT);
  pinMode(backward, OUTPUT);
  //led pins
  red.set_output_mode();
  yellow.set_output_mode();
  green.set_output_mode();
  pinMode(HEADLIGHTS, OUTPUT);
  //ultrasonic sensor pins
  triggerPin.set_output_mode();
  pinMode(echoPin, INPUT);
  //temperature pin
  dht.setup(DHT_PIN);
  //initialize to LOW
  digitalWrite(forward,LOW);
  digitalWrite(backward, LOW);
  red.off();
  yellow.off();
  green.off();
  analogWrite(HEADLIGHTS,0);
  //begin lcd
  lcd.begin(16,2);
}

float ultrasonicMeasure() {
  // generate 10-microsecond pulse to TRIG pin
  triggerPin.on();
  delayMicroseconds(10);
  triggerPin.off();
  // measure duration of pulse from ECHO pin
  float duration_us = pulseIn(echoPin, HIGH);
  // calculate the distance
  float distance_cm = 0.017 * duration_us;
  return distance_cm;
}

int coolantLevelMeasure() {
  delay(10);                      // wait 10 milliseconds
  //int value = analogRead(WATER_SIGNAL_PIN); // read the analog value from sensor
  waterPower.on();
  int value = waterData.readADC();
  waterPower.off();
  return value;
}

boolean checkCardReader()
{
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) 
  {
    return false;
  }
  // Select one of the cards
  if (mfrc522.PICC_ReadCardSerial()) 
  {
    return false;
  }
  //Show UID on serial monitor
  //Serial.print("UID tag :");
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  content.toUpperCase();
  if(content.substring(1) == "CD 3B 7C 82") //change here the UID of the card/cards that you want to give access
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WELCOME");
    tone(41, 500, 750);
    delay(1100);
    lcd.clear();
    return true;
  }
 
  else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WRONG KEY");
    tone(41, 494, 750);
    delay(1100);
    lcd.clear();
  }
  return false;
} 

void updateCar(){
  distance = ultrasonicMeasure();
  outVal = map(inVal,0,1023,0,255);
  motorSpeed = outVal / 255.0 *100.0;
  
  analogWrite(forward, outVal);
  analogWrite(HEADLIGHTS,headlightValue);
  
  if(distance > 25.0){
    red.off();
    yellow.off();
    green.off();
   }
  else if(distance > 20.0){
    red.off();
    yellow.off();
    green.on();
  }
  else if(distance > 15.0){
    red.off();
    yellow.on();
    green.on();
  }
  else{
    red.on();
    yellow.on();
    green.on();
  }

}

void displayLCD(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Spd:");
  lcd.setCursor(4,0);
  lcd.print((int) motorSpeed);
  lcd.setCursor(7,0);
  lcd.print("Tmp:");
  lcd.setCursor(11,0);
  lcd.print((int) temperatureF);
  lcd.setCursor(0,1);
  lcd.print("Coolant:");
  lcd.print((int) coolantLevel);
}

void printData(){
  Serial.print(distance/100.0,2);Serial.print(",");

  Serial.print(digitalRead(RED));Serial.print(",");
  Serial.print(digitalRead(YELLOW));Serial.print(",");
  Serial.print(digitalRead(GREEN));Serial.print(",");

  if(motorSpeed < 10) Serial.print("00");
  else if(motorSpeed < 100) Serial.print("0");
  Serial.print((int)motorSpeed);Serial.print(",");

  if(temperatureF < 10) Serial.print("00");
  else if(temperatureF < 100) Serial.print("0");
  else if(temperatureF == NAN) Serial.print("00000");
  Serial.print(temperatureF);Serial.print(",");

  Serial.print(temperatureAlarm);Serial.print(",");

  if(coolantLevel < 10) Serial.print("00");
  else if(coolantLevel < 100) Serial.print("0");

  Serial.print(coolantLevel);Serial.print(",");
  Serial.print(coolantLevelAlarm);Serial.print(",");
  Serial.println(headlightValue/1023.0,1);
}

void checkForSerialInput(){
  if(Serial.available()>0){
      val = Serial.read();
      temp = Serial.read();
      printData();
    }
}

void loop() {

  checkForSerialInput();

  if(locked){
    while(!checkCardReader()) checkForSerialInput();
    locked = false;
    //initialize IR receiver for the remote
    IrReceiver.begin(IR_IN_PIN, ENABLE_LED_FEEDBACK);
    delay(500);
  }
 
  if(IrReceiver.decode()){
    command = IrReceiver.decodedIRData.command;
    if (command == VOLUME_UP) { //Check if the key 1 is pressed
      if(inVal==1000){
        inVal = 0;
      }
      else{
        inVal = inVal + 100; //increment the motor by 10%
      }
    }
    else if(command == PAUSE_PLAY) { //Check if the key 2 is pressed
      if(headlightValue==0){
        headlightValue = 550;
      }
      else if(headlightValue==550){
        headlightValue = 1023;
      }
      else if(headlightValue==1023){
        headlightValue = 0;
      }
    }
    delay(150);
    IrReceiver.resume();
  }
  
  
  //gather info from sensors that lead to interlocking
  temperatureC = dht.getTemperature();
  temperatureF = dht.toFahrenheit(temperatureC);
  coolantLevel = coolantLevelMeasure();

  displayLCD();
  
  if(temperatureF > TEMP_MAX) temperatureAlarm = 1; 
  else temperatureAlarm = 0; 
  // water level sensor is broken, do not raise alarm
  if(coolantLevel < WATER_LVL_MAX) coolantLevelAlarm = 0;  
  else coolantLevelAlarm = 0; 
  // if there is an alarm, do not let car run
  
  
  if(coolantLevelAlarm || temperatureAlarm){
    analogWrite(forward, 0);
  }
  else{
    updateCar();
  }
}