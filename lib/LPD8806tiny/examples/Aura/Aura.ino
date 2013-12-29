#include "Time.h"
#include "LPD8806tiny.h"

/*****************************************************************************/

const uint8_t numPixels = 10, dataPin = 0, clockPin = 1, ambLightPin = 3;

const float red = 1, green = .33, blue = 0;
uint8_t wait = 40;
uint8_t pixelValues[numPixels] = {0};

LPD8806 strip = LPD8806(numPixels, dataPin, clockPin);

void setup() { 
    strip.begin();
    strip.show();
    randomSeed(analogRead(ambLightPin));
}

void loop() {
	
}

void adjustHyperbola() {
	static uint8_t ceiling = getCeiling();
        static uint8_t base = ceiling - 20;
	uint8_t centerPixel = random(0, numPixels - 1);

	for(static int i = base; i <= ceiling; i++) {
    	for(static int j = centerPixel - 5; j <= centerPixel + 5; j++) {
            if(j < 0 || j > numPixels - 1) continue;
            pixelValues[j] = round(-(j - centerPixel)^2 + i);
        }
        assignPixelValues(); 
	}

	for(int k = ceiling; k >= base; k--) {
		// TODO: Adjust each pixelValue
		assignPixelValues();
	}
}

void drawBase(uint8_t base) {
    for(static int i = 0; i < numPixels; i++) {
        pixelValues[i] = base;
    }
    assignPixelValues();
}

void assignPixelValues() {
    for( static int i = 0; i < numPixels; i++ ) {
	strip.setPixelColor( i, strip.Color( red * pixelValues[i], green * pixelValues[i], blue * pixelValues[i] ));
    }
    strip.show();
    delay(wait);
}

int getCeiling() {
    return abs(round(analogRead(ambLightPin)/8));
}

