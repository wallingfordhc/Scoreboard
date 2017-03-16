#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2017-03-16 00:01:07

#include "Arduino.h"
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <arduino.h>
void setup() ;
void loop() ;
void SetDigits(int digit0,int digit1, int digit2, int digit4);
void setDigit(int digit, int value, uint32_t color) ;
void SetTimer(int TimerNumber,int secs, uint32_t color) ;
void StartTimer(int TimerNumber) ;
void resetTimer(int timer);
void TimerValue(int timer);
void ShowTimer(int TimerNumber) ;
void ShowClock();
void setupRTC () ;
void loopRTC() ;
void TestRun() ;

#include "Scoreboard12.ino"


#endif
