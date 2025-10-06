//#include <FastLED.h>
#include <IRremote.hpp>
#include "expressions.h"
#include <Hashtable.h>

/**
20 expressions/animations, 21 buttons
POWER 	BA45	FF00  NEUTRAL - Blinking
VOL UP	B946	FF00  HAPPY
FUNC 	  B847	FF00  SUNGLASSES
REWIND 	BB44	FF00  CRYING
PLAY 	  BF40	FF00  PAUSE(?)
FORWARD	BC43	FF00  CRYING
DOWN	  F807	FF00  SLEEPING
VOLDOWN	EA15 	FF00  RAINBOW
UP	    F609	FF00  SCREENTEST
0	      E916	FF00  BSOD1
EQ	    E619	FF00  BSOD2
REPT	  F20D	FF00  HYPNO - animated
1	      F30C	FF00  NAUSEA
2	      E718	FF00  BLUSH
3	      A15E	FF00  HEART - animated
4	      F708	FF00  SUS - animated
5	      E31C	FF00  SAD
6	      A55A	FF00  HMM
7	      BD42	FF00  VEXED
8	      AD52	FF00  IRRITATED
9	      B54A	FF00  ANGRY
*/

//button -> animation
//animation -> list of expressions & timings
//some are one-to-one

#define LED_Pin 7
#define NUM_LEDS 140

#define DECODE_NEC

String POWER = String("BA45FF00");
String ZERO_BUTTON = String("E916FF00");

int IR_RECEIVE_PIN = 9;

//CRGB leds[NUM_LEDS];



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("Started up");  
  Hashtable<String, Expression> dict;
  dict.put(String("BA45FF00"), NEUTRAL); // power
  dict.put(String("E916FF00"), SCREENTEST);


}

void loop() {
  // put your main code here, to run repeatedly:
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
