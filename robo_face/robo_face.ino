//#include <FastLED.h>
#include <IRremote.hpp>
#include "expressions.h"
#include "animations.h"
#include <Hashtable.h>
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
  //Hashtable<String, Animation> codeToAnim;
  //createCodeToAnimTable(codeToAnim);
  //Serial.println(codeToAnim["BF40FF00"]);
  //Serial.println(expressionData[ANGRY][0].b);
  //Serial.println(expressionData[SCREENTEST][50].b);
  Serial.println(codeToAnim["E916FF00"]);
  // range based access
  //codeToAnim["F30CFF00"] = ANIM_HYPNO;
  
  
  
  
}

void loop() {
  if (IrReceiver.decode()) {
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); // Print "old" raw data
      
      auto recvData = IrReceiver.decodedIRData.decodedRawData;
      char hexCode [8];
      ltoa(recvData, hexCode, 16);
      String hexString = String(hexCode);
      hexString.toUpperCase();
      if(!hexString.equals("0")){
        Serial.println(hexString);
        Serial.println(codeToAnim[hexString]);
        Serial.println(animationData[codeToAnim[hexString]][1].millis);
        for (const auto& m : codeToAnim)
          {
            Serial.print("{");
            Serial.print(m.first); 
            Serial.println("}");
          }
      }
      IrReceiver.resume(); // Enable receiving of the next value
  }
}
