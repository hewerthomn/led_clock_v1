#include <FastLED.h>
#include <virtuabotixRTC.h>

#define PIN_RTC_CLK   6
#define PIN_RTC_DATA  7
#define PIN_RTC_RST   8

#define PIN_LED_HOURS   3
#define PIN_LED_MINUTES 4
#define PIN_LED_SECONDS 5
#define BRIGHTNESS_MIN  50
#define BRIGHTNESS_MAX  255
#define LED_FPS      1

CRGB ledsHours[12];
CRGB ledsMinutes[12];
CRGB ledsSeconds[24];

int _day        = 3,
    _month      = 6,
    _year       = 2017,
    _dayofweek  = 7,
    _hours      = 11,
    _minutes    = 25,
    _seconds    = 00,
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

  FastLED.addLeds<WS2811, PIN_LED_HOURS, RGB>(ledsHours, 12);
  FastLED.setBrightness(BRIGHTNESS_MAX);
  
  FastLED.addLeds<WS2811, PIN_LED_MINUTES, RGB>(ledsMinutes, 12);
  FastLED.setBrightness(BRIGHTNESS_MIN);

  FastLED.addLeds<WS2811, PIN_LED_SECONDS, RGB>(ledsSeconds, 24);
  FastLED.setBrightness(BRIGHTNESS_MAX);
}

void loop() {
  printClock();  
  showLedClock();
}

void showLedClock()
{
  int ledHours = 0;
  int ledMinutes = 0;
  int ledSeconds = 0;

  myRTC.updateTime();

  if (actualHours != myRTC.hours) {
    actualHours = myRTC.hours;
    
    ledHours = (actualHours > 12) ? (actualHours - 12) : actualHours;  
    ledHours = ledHours - 1;
    ledsHours[ledHours] = CRGB::Red;
    changeTime(1);
    ledsHours[ledHours] = CRGB::Red;

    Serial.print("ledHours: ");Serial.println(ledHours);    
  } 

  if (actualMinutes != myRTC.minutes) {
    actualMinutes = myRTC.minutes;    
    ledMinutes = map(actualMinutes, 0, 55, 0, 11);
    ledsMinutes[ledMinutes] = CRGB::Blue;
    changeTime(2);
    ledsMinutes[ledMinutes] = CRGB::Blue;
    Serial.print("ledMinutes: ");Serial.println(ledMinutes);
  }

  if (actualSeconds != myRTC.seconds) {
    actualSeconds = myRTC.seconds;

    ledSeconds = map(actualSeconds, 0, 59, 0, 23);
    ledsSeconds[ledSeconds] = CRGB::White;
    changeTime(3);
    ledsSeconds[ledSeconds] = CRGB::White;
    Serial.print("ledSeconds: ");Serial.println(ledSeconds);
  }

  FastLED.show();
//  FastLED.delay(1000 / LED_FPS);
}

void changeTime(int t)
{
  FastLED.show();  
  delay(500);
  switch (t)
  {
    case 1:
      turnOffLedsHour();      
      break;
    case 2:
      turnOffLedsMinutes();
      break;
    case 3:
      turnOffLedsSeconds();
      break;
  }  
}

void turnOffLedsHour()
{
  for (int i = 0; i <= 11; i++) {
    ledsHours[i] = CRGB::Black;
  }  
  FastLED.show();
}

void turnOffLedsMinutes()
{
  for (int i = 0; i <= 11; i++) {
    ledsMinutes[i] = CRGB::Black;
  }  
  FastLED.show();  
}

void turnOffLedsSeconds()
{
  for (int i = 0; i <= 23; i++) {
    ledsSeconds[i] = CRGB::Black;
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
