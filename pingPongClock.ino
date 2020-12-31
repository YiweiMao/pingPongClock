#include "os.h"
#include <FastLED.h>
#include <RTClib.h>
RTC_Millis rtc;


#define LED_PIN     6
#define NUM_LEDS    128


CRGB leds[NUM_LEDS];
CHSV colour( 0, 255, 180);
int counter=0;
const int Digits[10][10] =
{
  {7,8,10,11,14,18,22,24},
  {14,16,18,22,24},
  {7,8,9,11,14,16,18,24},
  {7,9,11,14,16,18,22,24},
  {9,10,11,16,18,22,24},
  {7,9,10,11,14,16,18,22},
  {7,8,9,14,15,16,18,22},
  {7,11,14,16,17,24},
  {7,8,9,10,11,14,16,18,22,24},
  {7,9,10,11,14,16,17,24},
};
void setup()
{
  initOS();
  initBlink(); // toggle built in LED every 50 ms
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    //FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);
    // limit my draw to 1A at 5v of power draw
    FastLED.setMaxPowerInVoltsAndMilliamps(5,100); 

    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    rtc.begin(DateTime(F(__DATE__), F(__TIME__)));

    FastLED.clear();
    FastLED.show();
    
}

//function that uses the Digit aray to display numbers between 0 and 100
void displaynumber( int place , int number){
  for (int i = 0 ; i < 10 ; i++) {
    if (Digits[number/10][i] != 0) {
      leds[(Digits[number/10][i]+place)] = CRGB(255,255,255);;
    }
    if (Digits[number%10][i] != 0) {
      leds[(Digits[number%10][i]+28+place)] = CRGB(255,255,255);;
    }
  }
}

void loop(){
  DateTime now = rtc.now();
  
  
  //changes the colour of background every 10 cycles
  if (counter<10){
    counter++;
  }else{
    colour.hue = (colour.hue+1)%256;
    counter=0;
  }
  // sets background to rainbow colours
  for ( int i=0; i< 128;i++){
    colour.hue = (colour.hue+1)%256;
    leds[i]= colour;
  }
  colour.hue = (colour.hue+128)%256;
  
  displaynumber(0,now.hour());
  displaynumber(70,now.minute());  //display colons
  if ( now.second()%2 == 0 ){
    leds[64] = CRGB(255,255,255);
    leds[66] = CRGB(255,255,255);
  }
  FastLED.show();
  FastLED.delay(100);
}

/*

Serial.print(" seconds since 1970: ");
    Serial.println(now.unixtime());

    // calculate a date which is 7 days and 30 seconds into the future
    DateTime future (now.unixtime() + 7 * 86400L + 30);

    Serial.print(" now + 7d + 30s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();
*/