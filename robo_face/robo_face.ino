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

cppQueue animationQueue(4, MAX_ANIM_FRAMES, FIFO);

int IR_RECEIVE_PIN = 9;

//CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("Started up"); 
  int currentAnimation = ANIM_DEFAULT;
  unsigned long animFrameStart = millis();
  Serial.println(animFrameStart);
  unsigned long animFrameEnd = millis() + animationData[ANIM_DEFAULT][0].millis;
  Serial.println(animFrameEnd);
  //start animation is default w/ blinking
  for(int i = 0; i < MAX_ANIM_FRAMES; i++){
    animationQueue.push(&animationData[ANIM_DEFAULT][i]);
  }
}

void loop() {
  if (IrReceiver.decode()) {    

    //convert received data to 8 character hex code
    auto recvData = IrReceiver.decodedIRData.decodedRawData;
    char hexCode [8];
    ltoa(recvData, hexCode, 16);
    String hexString = String(hexCode);
    hexString.toUpperCase();

    //ignore repeated/burst data (cluster of the same code is decoded as "0")
    if(!hexString.equals("0")){
      Serial.println(hexString);
      int animNumber = getAnimationFromCode(codeToAnim, hexString);
      //a corresponding animation was found for the received code
      if (animNumber != ANIM_NONE) {

        //clear old animation from queue
        animationQueue.flush();

        //add all frames of the selected animation to the queue
        for(int i = 0; i < MAX_ANIM_FRAMES; i++){
          //if the frame has 0 ms, it can be ignored
          if(animationData[animNumber][i].millis != 0){
            animationQueue.push(&animationData[animNumber][i]);
          }
        }
      }
    }
    IrReceiver.resume(); // Enable receiving of the next value
  }
}
