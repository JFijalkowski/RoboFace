//#include <FastLED.h>
#include <IRremote.hpp>
#include "expressions.h"
#include <Hashtable.h>

#define LED_Pin 7
#define NUM_LEDS 140

#define DECODE_NEC

//button -> animation
//animation -> list of expressions & timings
//expression -> pixel data
//some are one-to-one



String POWER = String("BA45FF00");
String ZERO_BUTTON = String("E916FF00");

int IR_RECEIVE_PIN = 9;

//CRGB leds[NUM_LEDS];

typedef struct animationFrame {
  int expression;
  int millis;
};



void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("Started up");  
  Hashtable<String, Expression> buttonToAnim;
  buttonToAnim.put("BA45FF00", NEUTRAL); // power
  buttonToAnim.put("E916FF00", SCREENTEST);
  buttonToAnim.put("B946FF00", HAPPY);
  Hashtable<Expression, animationFrame[]>;
  animationFrame neutralStatic[1] = {{NEUTRAL, 5000}};
  animationFrame neutralBlink[7] = {{NEUTRAL, 5000}, {BLINK1, 100}, {BLINK2, 100}, {BLINK3, 100}, {BLINK4, 100}, {BLINK5, 100}, {BLINK6, 100}};

}

void loop() {
  if (IrReceiver.decode()) {
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); // Print "old" raw data
      
      auto recvData = IrReceiver.decodedIRData.decodedRawData;
      char hexCode [8];
      ltoa(recvData, hexCode, 16);
      Serial.println(hexCode);
      String hexString = String(hexCode);
      hexString.toUpperCase();
      if (hexString.equals(POWER)){
        Serial.println("Power button pressed");
      }
      else if (hexString.equals(ZERO_BUTTON)){
        Serial.println("Zero Button pressed");
      }
      IrReceiver.resume(); // Enable receiving of the next value
  }
}
