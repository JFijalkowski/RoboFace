//#include <FastLED.h>
#include <IRremote.hpp>
#include "expressions.h"
#include "animations.h"
#include <cppQueue.h>

#define LED_Pin 7
#define NUM_LEDS 140

#define DECODE_NEC

String POWER = String("BA45FF00");
String ZERO_BUTTON = String("E916FF00");

int IR_RECEIVE_PIN = 9;

//CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("Started up"); 
}

void loop() {
  if (IrReceiver.decode()) {
      //Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); // Print "old" raw data
      
      auto recvData = IrReceiver.decodedIRData.decodedRawData;
      char hexCode [8];
      ltoa(recvData, hexCode, 16);
      String hexString = String(hexCode);
      hexString.toUpperCase();
      if(!hexString.equals("0")){
        Serial.println(hexString);
        int animNumber = getAnimationFromCode(codeToAnim, hexString);
        if (animNumber != ANIM_NONE) {
          animationFrame frame = animationData[animNumber][0];
          Serial.println(frame.expression);
          Serial.println(frame.millis);
        }
      }
      IrReceiver.resume(); // Enable receiving of the next value
  }
}
