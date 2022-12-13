#include <Arduino.h>
#include "Timer.h"
#include <Adafruit_NeoPixel.h>

#define PIN_LED 5    // Control signal, connect to DI of the LED
#define NUM_LED 1     // Number of LEDs in a strip

bool btn_Bleue;
bool btn_Jaune;
bool btn_Rouge;

// Custom colour1: ROUGE
#define RED_VAL_1       255
#define GREEN_VAL_1     0
#define BLUE_VAL_1      0

// Custom colour2: VERT
#define RED_VAL_2       0
#define GREEN_VAL_2     255
#define BLUE_VAL_2      0


#include "WIFI_NINA_Connector.h"
#include "MQTTConnector.h"

const int pin_Bleue    = 2;
const int pin_Jaune    = 3;
const int pin_Rouge    = 4;

//const int LED_RGB      = 5;

Adafruit_NeoPixel RGB_Strip = Adafruit_NeoPixel(NUM_LED, PIN_LED, NEO_GRB + NEO_KHZ800);

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint16_t wait) {
  for (uint16_t i = 0; i < RGB_Strip.numPixels(); i++) {
    RGB_Strip.setPixelColor(i, c);
    RGB_Strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return RGB_Strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return RGB_Strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return RGB_Strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < RGB_Strip.numPixels(); i++) {
      RGB_Strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    RGB_Strip.show();
    delay(wait);
  }
}

void setup() {

  Serial.print(9600);
  ClientMQTT.loop();

  pinMode(pin_Bleue, INPUT); 
  pinMode(pin_Jaune, INPUT); 
  pinMode(pin_Rouge, INPUT); 
  //pinMode(LED_RGB, OUTPUT);
  RGB_Strip.begin();
  RGB_Strip.show();
  RGB_Strip.setBrightness(128);    // Set brightness, 0-255 (darkest - brightest)

  wifiConnect(); //  Branchement au réseau WIFI
  MQTTConnect();
}

void loop() {

  ClientMQTT.loop();

  btn_Bleue = digitalRead(pin_Bleue);
  btn_Jaune = digitalRead(pin_Jaune);
  btn_Rouge = digitalRead(pin_Rouge);
  
  rainbow(20);  // Rainbow
  colorWipe(RGB_Strip.Color(0, 255, 0), 1000);  // Green
  colorWipe(RGB_Strip.Color(255, 0, 0), 1000);  // Red
  colorWipe(RGB_Strip.Color(0, 0, 255), 1000);  // BLUE
  
   if (runEveryShort(1000)){
    appendPayload("SB_1", btn_Jaune);
    appendPayload("SB_2", btn_Rouge);
    appendPayload("SB_3", btn_Rouge);
    sendPayload();
   }
}
 