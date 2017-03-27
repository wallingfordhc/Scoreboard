// MAIN CODE FOR WALLINGFORD HC SCOREBOARD
// MARCH 2017 VERSION 1.2
// DAVID SHANNON

// INCLUDE REQUIRED HEADER FILES

#include <RTClib.h> //REAL TIME CLOCK FILE
#include <Adafruit_NeoPixel.h> // LED STRIP FILE ? CHANGE FOR FASTLED.IO?
#include <Wire.h> // NOT SURE WHAT
#include <arduino.h>

// PIN DEFINITIONS

#define LEDPIN      8 // data pin for LED strip
#define GSMTXPIN    9 // DATA PIN FOR TRANSMITTING MESSAGES TO GSM SIM
#define GSMRXPIN    10 // DATA PIN FOR RECEIVING MESSAGES FROM GSM SIM
#define TEMPPIN     11 // DATA PIN FOR TEMPERATURE SENSOR
#define LIGHTPIN    12 // DATA PIN FOR LIGHT SENSOR
#define HOMEPIN     13 // PIN FOR HOME GOAL BUTTON
#define AWAYPIN     14 // PIN FOR AWAY GOAL BUTTON
#define MODEPIN     15 // PIN FOR MODE BUTTON
#define SETPIN      16 // PIN FOR SET BUTTON

// Define constants

#define N_LEDS 180

#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF
#define OFF    0X000000
// define global variables


int DefaultBrightness = 64;
String TimerStatus = "Unset";
long TimerStartTime = 0;
long TimerDuration = 0;
long TimeNow = 0;
long Timer1 = 0;
long SwitchOnTime;

String DisplayMode = "Clock";
uint32_t DisplayColor = RED;
RTC_DS1307 RTC;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, LEDPIN, NEO_GRB + NEO_KHZ800);


// Font definitions ////////////////////////////
// 7 segments set up as

//  33333
//  4   2
//  11111
//  5   7
//  66666

int COLONLED = 81;

int Font[11][7] =  {
        {0,1,1,1,1,1,1}, //value 0
        {0,1,0,0,0,0,1}, //value 1
        {1,1,1,0,1,1,0}, //value 2
        {1,1,1,0,0,1,1}, //value 3
        {1,1,0,1,0,0,1}, //value 4
        {1,0,1,1,0,1,1}, //value 5
        {1,0,0,1,1,1,1}, //value 6
        {0,1,1,0,0,0,1}, //value 7
        {1,1,1,1,1,1,1}, //value 8
        {1,1,1,1,0,0,1},  //value 9
        {0,0,0,0,0,0,0}  //value 10
};

int LedSegmentMap[4][7][8] =  {
        {//first digit
                {0,1,2,3,4,4,4,4}, // segment 0
                {5,6,7,8,9,10,10,10}, // segment 1
                {11,12,13,14,15,15,15,15}, // segment 2
                {16,17,18,19,20,21,21,21},// segment 3
                {22,23,24,25,26,27,27,27}, // segment 4
                {28,29,30,31,32,32,32,32}, // segment 5
                {33,34,35,36,37,38,38,38} // segment 6
        },

        {//second digit
                {41,42,43,44,45,45,45,45}, // segment 0
                {46,47,48,49,50,51,51,51}, // segment 1
                {52,53,54,55,56,56,56,56}, // segment 2
                {57,58,59,60,61,62,62,62}, // segment 3
                {63,64,65,66,67,68,68,68}, // segment 4
                {69,70,71,72,73,73,73,73}, // segment 5
                {74,75,76,77,78,79,79,79} // segment 6
        },
        {//third digit
                {83,84,85,86,87,87,87,87}, // segment 0
                {88,89,90,91,92,93,93,93}, // segment 1
                {94,95,96,97,98,98,98,98}, // segment 2
                {99,100,101,102,103,104,104,104}, // segment 3
                {105,106,107,108,109,110,110,110}, // segment 4
                {111,112,113,114,115,115,115,115}, // segment 5
                {116,117,118,119,120,121,121,121} // segment 6
        },

        {//forth digit
                {124,125,126,127,128,128,128,128}, // segment 0
                {129,130,131,132,133,134,134,134}, // segment 1
                {135,136,137,138,139,139,139,139}, // segment 2
                {140,141,142,143,144,145,145,145}, // segment 3
                {146,147,148,149,150,151,151,151}, // segment 4
                {152,153,154,155,156,156,156,156}, // segment 5
                {157,158,159,160,161,162,172,172}  // segment 6

        }
};

// Button timing variables
int debounce = 20;          // ms debounce period to prevent flickering when pressing or releasing the button
int DCgap = 250;            // max ms between clicks for a double click event
int holdTime = 1000;        // ms hold period: how long to wait for press+hold event
int longHoldTime = 3000;    // ms long hold period: how long to wait for press+hold event

// Button variables
boolean buttonVal = HIGH;   // value read from button
boolean buttonLast = HIGH;  // buffered value of the button's previous state
boolean DCwaiting = false;  // whether we're waiting for a double click (down)
boolean DConUp = false;     // whether to register a double click on next release, or whether to wait and click
boolean singleOK = true;    // whether it's OK to do a single click
long downTime = -1;         // time the button was pressed down
long upTime = -1;           // time the button was released
boolean ignoreUp = false;   // whether to ignore the button release because the click+hold was triggered
boolean waitForUp = false;        // when held, whether to wait for the up event
boolean holdEventPast = false;    // whether or not the hold event happened already
boolean longHoldEventPast = false;// whether or not the long hold event happened already




// MAIN SETUP FUNCTION
void setup() {

// Set-up mode button pin
   pinMode(MODEBUTTONPIN, INPUT);
   digitalWrite(MODEBUTTONPIN, HIGH );


// SETUP REFRESH SPEED - HOW OFTEN SHOULD SMS ETC BE CHECKED

int SMSCheckRate = 10; // CHECK FOR SMS EVERY 10 SECS
int TEMPCheckRate = 600; // CHECK TEMPERATURE EVERY 10 MINS

// SETUP SERIAL CONNECTION
   Serial.begin(9600);

// SETUP REAL TIME CLOCK

   setupRTC ();

// SETUP TIMER

int TimerLowerLimit = 120; // TIMER STOP POINT - the lower limit for the timer. e.g. stop at 2 mins to give the umpire the final say

SetTimer(1,100); // Set timer 1 to 100 seconds
StartTimer(1); // Start timer 1

uint32_t TimerColor = 0xFF0000;


// SETUP GSM SIM

// SETUP TEMP SENSOR

// SETUP LIGHT SENSOR

// SETUP DISPLAY

    strip.begin();
    SetBrightness();


// Serial.println("---------------");
//  Serial.println(DisplayMode);



}



void loop() {

//EVERY CYCLE CHECK FOR UPDATES TO SCORE, TIME OR MODE

//CHECK FOR SMS

//CHECK FOR BUTTON PUSH

int b=ButtonPress();

if (b==1) ShowTimer(1);
if (b==2) ShowClock();
if (b==3) CycleBrightness();

  //BUILD IN DEBOUCED ASSESSMENT OF BUTTON STATUS

  // PRESSED, LONG PRESS, ETC

// CHECK AMBIENT LIGHT


// EVERY CYCLE CHECK FOR CURRENT MODE AND DISPLAY APPROPRIATELY

  Display(); // Show Stuff on the Digits
}
// end of Loop

void Display() {

  //Depending on the DisplayMode show appropriately on the Clock and Score Digits

  //Test Mode
  if (DisplayMode == "Test") {
    ShowTest();
  }
  //Countdown timer mode
  if (DisplayMode == "Timer") {
    ShowTimer(1);
  }
  //Clock Mode
  if (DisplayMode == "Clock") {
    ShowClock();
  }
}

// FUNCTION TO SET THE CLOCK DIGITS
void SetDigits(int digit0,int digit1, int digit2, int digit4){
  setDigit( 0, digit0, DisplayColor);
  setDigit( 1, digit1, DisplayColor);
  setDigit( 2, digit2, DisplayColor);
  setDigit( 3, digit4, DisplayColor);
}

// FUNCTION TO SET AND SHOW ANY SINGLE DIGIT
void setDigit(int digit, int value, uint32_t color) {
        int ledsPerSeg = 8;

        for (int seg = 0; seg < 7; seg++) {
                for (int led = 0; led < ledsPerSeg; led++) {
                        strip.setPixelColor(LedSegmentMap[digit][seg][led], Font[value][seg] ? color : OFF);
                }
        }
        strip.show();
}

// FUNCTION TO SET AND SHOW THE COLON

void setColon (int vlaue, uint32_t color) {


  strip.setPixelColor(COLONLED,color);
  strip.show();
}

// SET A TIMER IN MEMORY TO A GIVEN NUMBER OF SECONDS
void SetTimer(int TimerNumber,int secs) {

TimerDuration = secs;
TimerStatus = "Set";

}

// FUNCTION TO START A PARTICULAR TIMER
void StartTimer(int TimerNumber) {

DateTime now=RTC.now(); // dont use RtC IF ITS NOT CONNECTED
long TimeNow = now.unixtime();
//long TimeNow = millis()/1000; dont use if RTC is connected
TimerStartTime = TimeNow;
 TimerStatus = "Running";

}

// FUNCTION TO RESET TIMER
void resetTimer(int timer){

}

// FUNCTION TO SET VALUE OF TIMER
long TimerValue(int timer){
DateTime now=RTC.now();
long TimeNow = now.unixtime();
long TimerShow=0;

if (TimerStatus == "Running"){
// TimeNow = millis()/1000;
long TimerDisplayTime =  TimerStartTime - TimeNow + TimerDuration;
Serial.print(TimerDisplayTime);
TimerShow = TimerDisplayTime;
}
else {
// don't update the Timer Display Time
}

return TimerShow;
}

// FUNCTION TO SHOW A PARTICULAR TIMER ON THE CLOCK DISPLAY
void ShowTimer(int TimerNumber) {
 DisplayColor = GREEN;

DisplayMode = "Timer";  // set the display mode in case it isnt yet set

long TimerDisplayTime = TimerValue(1); //get the value to display
int TimerDisplayTenMins = TimerDisplayTime/600;
if (TimerDisplayTenMins==0) TimerDisplayTenMins = 10; // if zero set the Tens digit to OFF
int TimerDisplayMins = (TimerDisplayTime/60)%10;
int TimerDisplayTenSecs = ((TimerDisplayTime%60)/10)%10;
int TimerDisplaySecs = TimerDisplayTime%10;

if(TimerDisplayTime <= 0){
  SetDigits(10,0,0,0);
}
else
{
  SetDigits (TimerDisplayTenMins,TimerDisplayMins,TimerDisplayTenSecs,TimerDisplaySecs);
}
}

// FUNCTION TO SHOW CURRENT TIME ON CLOCK DIGITS
void ShowClock(){
DisplayColor = RED;
  DisplayMode = "Clock";
  // Serial.println("Show Clock");
  DateTime now=RTC.now();

  int Digit1 = (now.hour())/10;
  int Digit2 = now.hour()%10;
  int Digit3 = now.minute()/10;
  int Digit4 = now.minute()%10;

  // Flash the COLON

  if (millis()%1000 < 100) {
    setColon(1,RED);
  }
  else
  {
    setColon  (0,OFF);
  }

 // Serial.println(Digit1);
 // Serial.println(now.hour());
 // Serial.println("---------------");
 // Serial.println(Digit2);

  SetDigits(Digit1,Digit2,Digit3,Digit4);
}

// FUNCTION TO INITIAILISE AND SET THE REAL TIME CLOCK
void setupRTC () {
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
   RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  if(RTC.isrunning()){
   Serial.println("RTC is running!");
  }
  loopRTC();
  DateTime now=RTC.now();
  long TimeNow = now.unixtime();
  SwitchOnTime = TimeNow;
}

// FUNCTION TO EXTRACT YEAR/month/day/hour/minute components from RTC FEED
void loopRTC() {

    DateTime now = RTC.now();
    String timeStr = String(now.year()) + ", ";
    timeStr = timeStr + String(now.month()) + ", ";
    timeStr = timeStr + String(now.day()) + ", ";
    timeStr = timeStr + String(now.hour()) + ", ";
    timeStr = timeStr + String(now.minute()) + ", ";
    timeStr = timeStr + String(now.second());
    Serial.println(timeStr);

}

// FUNCTION TO RUN THROUGH THE DIGITS TESTING THAT THEY FUNCTION CORRECTLY
void ShowTest() {
for (int digit=0; digit<4;digit ++){
 for (int i=0; i<11; i++){
    setDigit(digit,i,RED);
    delay(500);
    }
  }
}
// function to return the state of a button
int ButtonPress(int ButtonPIN)
{
   int event = 0;
   buttonVal = digitalRead(ButtonPIN);
   // Button pressed down
   if (buttonVal == LOW && buttonLast == HIGH && (millis() - upTime) > debounce)
   {
       downTime = millis();
       ignoreUp = false;
       waitForUp = false;
       singleOK = true;
       holdEventPast = false;
       longHoldEventPast = false;
       if ((millis()-upTime) < DCgap && DConUp == false && DCwaiting == true)  DConUp = true;
       else  DConUp = false;
       DCwaiting = false;
   }
   // Button released
   else if (buttonVal == HIGH && buttonLast == LOW && (millis() - downTime) > debounce)
   {
       if (not ignoreUp)
       {
           upTime = millis();
           if (DConUp == false) DCwaiting = true;
           else
           {
               event = 2;
               DConUp = false;
               DCwaiting = false;
               singleOK = false;
           }
       }
   }
   // Test for normal click event: DCgap expired
   if ( buttonVal == HIGH && (millis()-upTime) >= DCgap && DCwaiting == true && DConUp == false && singleOK == true && event != 2)
   {
       event = 1;
       DCwaiting = false;
   }
   // Test for hold
   if (buttonVal == LOW && (millis() - downTime) >= holdTime) {
       // Trigger "normal" hold
       if (not holdEventPast)
       {
           event = 3;
           waitForUp = true;
           ignoreUp = true;
           DConUp = false;
           DCwaiting = false;
           //downTime = millis();
           holdEventPast = true;
       }
       // Trigger "long" hold
       if ((millis() - downTime) >= longHoldTime)
       {
           if (not longHoldEventPast)
           {
               event = 4;
               longHoldEventPast = true;
           }
       }
   }
   buttonLast = buttonVal;
   return event;

}

void SetBrightness(){

   int Brightness == DefaultBrightness; // set brightness to default level
// IF DISPLY MODE IS 'FLASH' 1THEN WORK OUT IF THE LIGHTS ARE ON OR OFF
long flashFrequency = 200 // milliseconds between flashes
        long flashLength = 100 // milliseconds the flash lasts
        
        if (DisplayFlash){ // if the display should be flashing
                if (millis()%flashfrequency>flashLength){ // and if the flash should be off
                        Brightness == 0; // set brightness to zero
                }
        } 
    
        strip.setBrightness(Brightness);
}

void CycleBrightness(){
// get the current brightness and increase it to max then start at low
Brightness = Brightness + 10;
if (Brightness > 255) Brightness = 20;
SetBrightness();
  
}
