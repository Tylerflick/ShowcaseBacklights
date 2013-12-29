#include <Arduino.h>
#include "LPD8806tiny.h"
#include <avr/power.h>
void setup();
void loop();
uint8_t readAmbientLight();
void adjustPixelValue();
bool ambLightOutOfRange();
void drawStaticPixels();
void drawDynamicPixels();
void twinkle();
#line 1 "src/sketch.ino"
//#include "LPD8806tiny.h"
//#include <avr/power.h>


#define DATA_PIN 0
#define CLOCK_PIN 1
#define RED 1
#define GREEN 0.33
#define BLUE 0
#define WAIT 50
#define DEBUG false

typedef enum {
  increasing, decreasing
} Adjustment;
typedef struct {
  Adjustment adjustment;
  short adjustValue;
} LEDPixel;

static const uint16_t numPixels = 10, 
                      photoPin = 1,
                      value_range = 10; 
uint8_t curBaseLight = 0,
        nextBaseLight, 
        minValue, 
        maxValue;
LPD8806 strip = LPD8806(numPixels, DATA_PIN, CLOCK_PIN);
LEDPixel pixelValues[numPixels];


void setup() {
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  strip.begin();
  strip.show();
  randomSeed(analogRead(2));
  curBaseLight = readAmbientLight();

  // Calculate the min & max value for the ambient light
  minValue = (uint8_t)ceil(1/min(min(GREEN, BLUE), RED)) * value_range;
  maxValue = (uint8_t)127 - minValue;

  for(int i = 0; i < numPixels; ++i) {
    pixelValues[i].adjustment = random(0,1) ? increasing : decreasing;
    pixelValues[i].adjustValue = random(-value_range, value_range);
  }
  if(DEBUG) pinMode(0, OUTPUT);
}

void loop() {
  nextBaseLight = readAmbientLight();
  if(DEBUG) {
    // Write the current ambient light to the onboard LED
    analogWrite(0, nextBaseLight);
    delay(100);
  } else {
    while(ambLightOutOfRange()) {
      adjustPixelValue();
      drawStaticPixels();
      strip.show();
      delay(WAIT);
  }
  //drawDynamicPixels();
  twinkle();
  strip.show();
  delay(WAIT);
  }
}

uint8_t readAmbientLight() {
  // Convert range from 1024 to 128
  uint8_t returnVal = (uint8_t)(analogRead(photoPin)/8);
  returnVal = returnVal <  minValue ? minValue : returnVal;
  returnVal = returnVal > maxValue ? maxValue : returnVal;
  return returnVal;
}

void adjustPixelValue() {
  if(curBaseLight > nextBaseLight)
    --curBaseLight;
  else if(curBaseLight < nextBaseLight)
    ++curBaseLight;
}

bool ambLightOutOfRange() {
  return abs(curBaseLight - nextBaseLight) > value_range;
}

void drawStaticPixels() {
  for(uint8_t pixel = 0; pixel < numPixels; ++pixel) {
    strip.setPixelColor(pixel, strip.Color(
                        (uint8_t)(curBaseLight * RED), 
                        (uint8_t)(curBaseLight * GREEN),
                        (uint8_t)(curBaseLight * BLUE))); 
  }
}

void drawDynamicPixels() {
  for(uint8_t i = 0; i < numPixels; ++i) {
    
    if (pixelValues[i].adjustment == increasing) {
      if (pixelValues[i].adjustValue >= value_range) {
        pixelValues[i].adjustment = decreasing;
        --pixelValues[i].adjustValue;        
      } else {
        ++pixelValues[i].adjustValue;
      }
    } else {
      if (pixelValues[i].adjustValue <= -value_range) {
        pixelValues[i].adjustment = increasing;
        ++pixelValues[i].adjustValue;
      } else {
        --pixelValues[i].adjustValue;
      }
    }
    strip.setPixelColor(i,
                        (uint8_t)((curBaseLight + pixelValues[i].adjustValue) * RED),
                        (uint8_t)((curBaseLight + pixelValues[i].adjustValue) * GREEN),
                        (uint8_t)((curBaseLight + pixelValues[i].adjustValue) * BLUE));
  }
}

void twinkle() {
  for(uint8_t i = 0; i < numPixels; ++i) {
    pixelValues[i].adjustValue = random(-value_range, value_range);
    strip.setPixelColor(i,
                        (uint8_t)((curBaseLight + pixelValues[i].adjustValue) * RED),
                        (uint8_t)((curBaseLight + pixelValues[i].adjustValue) * GREEN),
                        (uint8_t)((curBaseLight + pixelValues[i].adjustValue) * BLUE));

  }
}
