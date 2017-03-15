// MAIN CODE FOR WALLINGFORD HC SCOREBOARD
// MARCH 2017 VERSION 1.1
// DAVID SHANNON

// INCLUDE REQUIRED HEADER FILES

#include <RTClib.h> //REAL TIME CLOCK FILE
#include <Adafruit_NeoPixel.h> // LED STRIP FILE ? CHANGE FOR FASTLED.IO?
#include <Wire.h> // NOT SURE WHAT

// PIN DEFINITIONS

#define LEDPIN      8 // data pin for LED strip
#define GSMTXPIN    9 // DATA PIN FOR TRANSMITTING MESSAGES TO GSM SIM
#define GSMRXPIN    10 // DATA PIN FOR RECEIVING MESSAGES FROM GSM SIM
#define TEMPPIN     11 // DATA PIN FOR TEMPERATURE SENSOR
#define LIGHTPIN    12 // DATA PIN FOR LIGHT SENSOR

// MAIN SETUP FUNCTION
void setup() {

// SETUP REFRESH SPEED - HOW OFTEN SHOULD SMS ETC BE CHECKED

int SMSCheckRate = 10 // CHECK FOR SMS EVERY 10 SECS
int TEMPCheckRate = 600 // CHECK TEMPERATURE EVERY 10 MINS

// SETUP SERIAL CONNECTION
   Serial.begin(9600);

// SETUP REAL TIME CLOCK
   RTC_DS1307 RTC;
   setupRTC ();

// SETUP TIMER

int TimerLowerLimit = 120 // TIMER STOP POINT - the lower limit for the timer. e.g. stop at 2 mins to give the umpire the final say

String TimerStatus = "Unset";

long TimerStartTime = 0;
long TimerDuration = 0;
long TimeNow = 0;
long TimerDisplayTime = 0;
long TimerDisplayMins = 0;
long TimerDisplaySecs = 0;
long TimerDisplayTenSecs = 0;

uint32_t TimerColor = 0xFF0000;


// SETUP GSM SIM

// SETUP TEMP SENSOR

// SETUP LIGHT SENSOR

// SETUP DISPLAY

string DisplayMode = "Clock";

#define N_LEDS 180

#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF
#define OFF    0X000000

int Brightness = 64;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, LEDPIN, NEO_GRB NO+ NEO_KHZ800);


// 7 segments set up as

//  33333
//  4   2
//  11111
//  5   7
//  66666

int SegmentsActive[11][7] =  {
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

        strip.begin();
        strip.setBrightness(Brightness);

}

void loop() {

//EVERY CYCLE CHECK FOR UPDATES TO SCORE, TIME OR MODE

//CHECK FOR SMS

//CHECK FOR BUTTON PUSH

  //BUILD IN DEBOUCED ASSESSMENT OF BUTTON STATUS

  // PRESSED, LONG PRESS, ETC

// CHECK AMBIENT LIGHT


// EVERY CYCLE CHECK FOR CURRENT MODE AND DISPLAY APPROPRIATELY

if (DisplayMode == "Test"){
TestRun();

}
        //Countdown timer
if (ClockMode == "Timer"){
  Serial.println("hello");
        ShowTimer(1);
}
if (ClockMode == "Clock"){
  ShowClock();
}

delay(500); // delay to make sure everythign responds
}

// FUNCTION TO SET THE CLOCK DIGITS
void SetDigits(int digit0,int digit1, int digit2, int digit4){
  setDigit( 0, digit0, TimerColor);
  setDigit( 1, digit1, TimerColor);
  setDigit( 2, digit2, TimerColor);
  setDigit( 3, digit4, TimerColor);
}

// FUNCTION TO SET AND SHOW ANY SINGLE DIGIT
void setDigit(int digit, int value, uint32_t color) {
        int ledsPerSeg = 8;

        for (int seg = 0; seg < 7; seg++) {
                for (int led = 0; led < ledsPerSeg; led++) {
                        strip.setPixelColor(LedSegmentMap[digit][seg][led], SegmentsActive[value][seg] ? color : OFF);
                }
        }
        strip.show();
}

// SET A TIMER IN MEMORY TO A GIVEN NUMBER OF SECONDS
void SetTimer(int TimerNumber,int secs, uint32_t color) {

TimerDuration = secs;
TimerStatus = "Set";

}

// FUNCTION TO START A PARTICULAR TIMER
void StartTimer(int TimerNumber) {

//DateTime now=RTC.now(); // dont use RtC IF ITS NOT CONNECTED
//TimeNow = now.unixtime();
TimeNow = millis()/1000;
TimerStartTime = TimeNow;
 TimerStatus = "Running";

}

// FUNCTION TO RESET TIMER
void resetTimer(int timer){

}

// FUNCTION TO SET VALUE OF TIMER
void TimerValue(int timer){
// DateTime now=RTC.now();
// TimeNow = now.unixtime();

if (timerstatus == "Running"){
TimeNow = millis()/1000;
TimerDisplayTime =  TimerStartTime - TimeNow + TimerDuration;
}
else {
TimerDisplayTime = TimerDISPLAYTIME;
}
}

// FUNCTION TO SHOW A PARTICULAR TIMER ON THE CLOCK DISPLAY
void ShowTimer(int TimerNumber) {


int TimerDisplayTenMins = TimerDisplayTime/600;
TimerDisplayMins = (TimerDisplayTime/60)%10;
TimerDisplayTenSecs = ((TimerDisplayTime%60)/10)%10;
TimerDisplaySecs = TimerDisplayTime%10;

if(TimerDisplayTime <= 0){
  SetDigits(0,0,0,0);
}
else
{
  SetDigits (TimerDisplayTenMins,TimerDisplayMins,TimerDisplayTenSecs,TimerDisplaySecs);
}
}

// FUNCTION TO SHOW CURRENT TIME ON CLOCK DIGITS
void ShowClock(){
  DateTime now=RTC.now();

  int Digit1 = (now.hour())/10;
  int Digit2 = now.hour()%10;
  int Digit3 = now.minute()/10;
  int Digit4 = now.minute()%10;

  Serial.println(Digit1);
  Serial.println(now.hour());
  Serial.println("---------------");
  Serial.println(Digit2);

  SetDigits(Digit1,Digit2,Digit3,Digit4);
}

// FUNCTION TO INITIAILISE AND SET THE REAL TIME CLOCK
void setupRTC () {
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
   // loopRTC();
    // following line sets the RTC to the date & time this sketch was compiled
   // RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  if(RTC.isrunning()){
   //loopRTC();
  }
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
void TestRun() {
for (int digit=0; digit<4;digit ++){
 for (int i=0; i<11; i++){
    setDigit(digit,i,RED);
    delay(500);
    }
  }
}
