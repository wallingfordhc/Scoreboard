#include <Adafruit_NeoPixel.h>

// creat 7 segment display using addressable LED strips
// with thanks to tetsuo in pjrc.com

const int ledsPerStrip = 56;

#define PIN      8
#define N_LEDS 56

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);



int SegmentsActive[10][7] =  {
        {1,1,1,0,1,1,1}, //value 0
        {0,0,1,0,0,1,0}, //value 1
        {1,0,1,1,1,0,1}, //value 2
        {1,0,1,1,0,1,1}, //value 3
        {0,1,1,1,0,1,0}, //value 4
        {1,1,0,1,0,1,1}, //value 5
        {1,1,0,1,1,1,1}, //value 6
        {1,0,1,0,0,1,0}, //value 7
        {1,1,1,1,1,1,1}, //value 8
        {1,1,1,1,0,1,0}  //value 9
};

int LedSegmentMap[1][7][8] =  { 
        {//first digit
                {0,1,2,3,4,5,6,7}, // segment 0
                {8,9,10,11,12,13,14,15}, // segment 1
                {16,17,18,19,20,21,22,23}, // segment 2
                {24,25,26,27,28,29,30,31}, // segment 3
                {32,33,34,35,36,37,38,39}, // segment 4
                {40,41,42,43,44,45,46,47}, // segment 5 
                {48,49,50,51,52,53,54,55} // segment 6
        }
};                          

#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF
#define OFF    0X000000

void setup() {
        strip.begin();
        //leds.show(); 
}

void loop() {

        //Countdown timer
        for(int x = 0; x < 10; x++){
                setDigit(0, x, RED);
                delay(1000);
        }
        delay(4000);
  
}

void setDigit(int digit, int value, uint32_t color) {
        int ledsPerSeg = 8;

        for (int seg = 0; seg < 7; seg++) {
                for (int led = 0; led < ledsPerSeg; led++) {
                        strip.setPixelColor(LedSegmentMap[digit][seg][led], SegmentsActive[value][seg] ? color : OFF);
                }
        }
        strip.show();
}
