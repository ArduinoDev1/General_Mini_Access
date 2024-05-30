#include "Arduino.h"
#include "RFID.h"
#include <Adafruit_NeoPixel.h>
#include <Keyboard.h>
#define RFID_PIN_SDA  10
#define RFID_PIN_RST  2
#define PIN           4
#define PIN1          5
#define PIN2          6
#define PIN3          7
#define PIN4          8
#define PIN5          9
#define NUMPIXELS  4
#define NUMPIXELS1 4
#define NUMPIXELS2 4
#define NUMPIXELS3 4
#define NUMPIXELS4 4
#define NUMPIXELS5 4
RFID rfid(RFID_PIN_SDA, RFID_PIN_RST);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels1(NUMPIXELS1, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2(NUMPIXELS2, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels3(NUMPIXELS3, PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels4(NUMPIXELS4, PIN4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels5(NUMPIXELS5, PIN5, NEO_GRB + NEO_KHZ800);
bool check = false;
unsigned long fadePreviousMillis = 0;
const long fadeInterval = 30; 
int brightness = 0; 
int brightness1 = 0; 
int fadeDirection = 1;
bool pixelsFadingIn = false;
bool pixelsFadingOut = false;
void setup() {
  Keyboard.begin();
  pixels.begin();
  pixels1.begin();
  pixels2.begin();
  pixels3.begin();
  pixels4.begin();
  pixels5.begin();
  Serial.begin(9600);
  rfid.init();
}
void updatePixels(Adafruit_NeoPixel &pixels, uint32_t color) {
  for (int i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, color);
  }
  pixels.show();
}
uint32_t fadeColor(uint32_t color, int brightness) {
  uint8_t r = (uint8_t)(brightness * ((color >> 16) & 0xFF) / 255);
  uint8_t g = (uint8_t)(brightness * ((color >> 8) & 0xFF) / 255);
  uint8_t b = (uint8_t)(brightness * ((color) & 0xFF) / 255);
  return pixels.Color(r, g, b);
}
void loop() {
  rfid.init();
  String rfidtag = rfid.readTag();
  rfid.printTag(rfidtag);

  if (check == false && rfidtag != "None") {
    if (rfidtag.length() < 10) {
      rfidtag = "0" + rfidtag;
    }
    if (rfidtag.length() == 10) {
      Keyboard.print(rfidtag);
      check = true;
      //updatePixels(pixels, pixels.Color(0, 200, 0));
      //updatePixels(pixels1, pixels1.Color(0, 200, 0));
      updatePixels(pixels2, pixels2.Color(0, 200, 0));
      updatePixels(pixels3, pixels3.Color(0, 200, 0));
      // updatePixels(pixels4, pixels4.Color(0, 200, 0));
      // updatePixels(pixels5, pixels5.Color(0, 200, 0));
    }
  }
  if (rfidtag == "None") {
    check = false;
    unsigned long currentMillis = millis();
    updatePixels(pixels2, pixels2.Color(0, 200, 100));
    updatePixels(pixels3, pixels3.Color(0, 200, 100));
    if (currentMillis - fadePreviousMillis >= fadeInterval) {
      fadePreviousMillis = currentMillis;
      if (!pixelsFadingOut && brightness < 200) {
        brightness += fadeDirection * 5;
      }
      if (!pixelsFadingOut && brightness >= 100 && brightness1 < 200) {
        brightness1 += fadeDirection * 5;
      }

      if (brightness1 >= 200) {
        pixelsFadingOut = true;
      }

      if (pixelsFadingOut && brightness1 > 0) {
        brightness1 -= fadeDirection * 5;
      }

      if (pixelsFadingOut && brightness1 <= 100 && brightness > 0) {
        brightness -= fadeDirection * 5;
      }
      if (pixelsFadingOut && brightness == 0 && brightness1 == 0) {
        pixelsFadingOut = false;
      }
    }
  }
  uint32_t fadedColor = fadeColor(pixels.Color(0, 200, 100), brightness);
  updatePixels(pixels, fadedColor);
  updatePixels(pixels5, fadedColor);
  uint32_t fadedColor1 = fadeColor(pixels.Color(0, 200, 100), brightness1);
  updatePixels(pixels1, fadedColor1);
  updatePixels(pixels4, fadedColor1);
}


