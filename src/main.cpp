// fuse settings for attiny85 8mhz internal clock:
// avrdude -c dragon_hvsp -p attiny85 -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

// USING forked library to fix setBrightness bug https://github.com/adafruit/Adafruit_NeoPixel/pull/63
#include <Adafruit_NeoPixel.h> // beware, I wasted some time with an older version that didn't have getBrightness()

#define PIN 0
Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, PIN, NEO_RGB + NEO_KHZ800);

//uint8_t eyeColor = 0; // green
//uint8_t eyeColor = 40; // yellow
//uint8_t eyeColor = 80; // red
//uint8_t eyeColor = 130; // purple ?
//uint8_t eyeColor = 180; // blue
//uint8_t eyeColor = 210; // aqua

uint8_t eyeColor = 50;


long randDelay;
uint8_t maxBrightness = 254;

void eyesOn();
void eyesOff();
void rampUp(uint32_t color, uint8_t wait);
void rampDown(uint8_t wait);
void colorWipe(uint32_t c, uint8_t wait);
uint32_t Wheel(byte WheelPos);




void setup() {
  pinMode(PIN, OUTPUT);
  strip.begin();
  strip.setBrightness(254);
  strip.show(); // Initialize all pixels to 'off'
  randomSeed(analogRead(0));
}

void loop() {
  // sometimes the eyes should just go away for 2-10 seconds...
  if(random(6) == 1) {
    randDelay = random(8000);
    delay(randDelay);
    delay(2000);
  }
  else {
    randDelay = random(5000);
    // turn the eyes on for .5 - 5.5 seconds...
    eyesOn();
    delay(500);
    delay(randDelay);
    // sometimes the eyes should do a quick double blink...
    if(random(4) == 1) {
      // turn the eye off two tenths of a second...
      eyesOff();
      delay(200);
      // turn the eyes on for .5 - 5.5 seconds...
      eyesOn();
      delay(500);
      delay(randDelay);
    }
    // turn the eyes off for .5 - 5.5 seconds...
    eyesOff();
    delay(500);
    delay(randDelay);
  }
}

void eyesOn() {
  rampUp(Wheel(eyeColor), 5);
  // colorWipe(Wheel(eyeColor), 0);
}

void eyesOff() {
  rampDown(1);
  // colorWipe(strip.Color(0,0,0), 0);
}

// ramp down brightness
void rampDown(uint8_t wait) {
  uint8_t curBrightness = strip.getBrightness();    // grab the current brightness leel
  for(uint8_t i=curBrightness; i>0; i--) {          // repeat as many steps as it is bright
    strip.setBrightness(i);                         // set the brightness
    strip.show();                                   // show the change
    delay(wait);                                    // and delay...
  }
  colorWipe(strip.Color(0,0,0), 0);
  strip.show();
}

// ramp up brightness with color value
void rampUp(uint32_t color, uint8_t wait) {
  strip.setBrightness(1);                       // set to 1 -- BUG ALERT, Adafruit library shutsdown strip if brightness goes to 0
  colorWipe(color, 0);                          // and set all to a color (should be off now)
  for(uint8_t i=1; i<maxBrightness; i++) {    // repeat as many times as max brightness
    strip.setBrightness(i);                     // set the brightness
    strip.show();                               // show the change
    delay(wait);                                // and delay...
  }
}

// Fill the dots one after the other with a color
// Courtesy Adafruit example... 
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// Courtesy Adafruit example...
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}