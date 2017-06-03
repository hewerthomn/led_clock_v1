#include <FastLED.h>
#include <virtuabotixRTC.h>

#define PIN_RTC_CLK   6
#define PIN_RTC_DATA  7
#define PIN_RTC_RST   8

#define PIN_LED_DATA 3
#define NUM_LEDS     24
#define BRIGHTNESS   50
#define LED_FPS      1

CRGB leds[NUM_LEDS];

int _day        = 3,
    _month      = 6,
    _year       = 2017,
    _dayofweek  = 7,
    _hours      = 7,
    _minutes    = 51,
    _seconds    = 14,
    actualHours,
    actualMinutes,
    actualSeconds
;



//             myRTC(clock, data, rst)
virtuabotixRTC myRTC(PIN_RTC_CLK, PIN_RTC_DATA, PIN_RTC_RST);

void setup() {
  Serial.begin(9600);

  //    setDS1302Time(seconds, minutes, hours, dayofweek, day, month, year)
//  myRTC.setDS1302Time(_seconds, _minutes, _hours, _dayofweek, _day, _month, _year);

  FastLED.addLeds<WS2811, PIN_LED_DATA, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  printClock();  
  showLedClock();
  
  delay(1000);
//  turnOffLeds();
}

void showLedClock()
{
  int ledHours = 0;
  int ledMinutes = 0;

  myRTC.updateTime();
  
  actualSeconds = myRTC.seconds;  

  if (actualHours != myRTC.hours) {
    actualHours = myRTC.hours;
    
    ledHours = (actualHours > 12) ? (actualHours - 12) : actualHours;  
    ledHours = ledHours - 1;
    leds[ledHours] = CRGB::Blue;
    changeTime(true);
    leds[ledHours] = CRGB::Blue;

    Serial.print("ledHours: ");Serial.println(ledHours);    
  } 

  if (actualMinutes != myRTC.minutes) {
    actualMinutes = myRTC.minutes;    
    ledMinutes = map(actualMinutes, 0, 55, 12, 23);
    leds[ledMinutes] = CRGB::Red;
    changeTime(false);
    leds[ledMinutes] = CRGB::Red;
    Serial.print("ledMinutes: ");Serial.println(ledMinutes);    
  }

  FastLED.show();
//  FastLED.delay(1000 / LED_FPS);
}

void changeTime(bool isHour)
{
  FastLED.show();
  delay(1000);  
  if (isHour) {
    turnOffLedsHour();
  } else {
    turnOffLedsMinutes();
  }
}

void turnOffLedsHour()
{
  for (int i = 0; i <= 11; i++) {
    leds[i] = CRGB::Black;
  }  
  FastLED.show();
}

void turnOffLedsMinutes()
{
  for (int i = 12; i <= 23; i++) {
    leds[i] = CRGB::Black;
  }  
  FastLED.show();  
}

void printClock()
{
  Serial.print(actualHours);Serial.print(":");
  Serial.print(actualMinutes);Serial.print(":");
  Serial.println(actualSeconds);
  Serial.println("---------");
}
