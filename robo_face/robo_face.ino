//#include <FastLED.h>
#include <IRremote.hpp>
#include "expressions.h"
#include "animations.h"
#include <cppQueue.h>


#define LED_Pin 7
#define NUM_LEDS 140
#define IR_RECEIVE_PIN 9

#define DECODE_NEC

cppQueue animationQueue(4, MAX_ANIM_FRAMES, FIFO);
unsigned long animFrameStart;
unsigned long animFrameEnd;
int currentAnimation;
int currentExpression;
//CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("Started up"); 
  
  
  //start animation is default w/ blinking
  currentAnimation = ANIM_DEFAULT;
  for(int i = 0; i < MAX_ANIM_FRAMES; i++){
    animationQueue.push(&animationData[currentAnimation][i]);
  }

  animationFrame frame;
  animationQueue.pop(&frame);
  currentExpression = frame.expression;

  animFrameStart = millis();
  animFrameEnd = millis() + frame.millis;
  Serial.print("Current Expression: ");
  Serial.println(currentExpression);

}

void loop() {

  

  //if an IR data packet has been received
  if (IrReceiver.decode()) {   
    Serial.println("receiving..."); 

    //convert received data to 8 character hex code
    auto recvData = IrReceiver.decodedIRData.decodedRawData;
    char hexCode [9];
    ltoa(recvData, hexCode, 16);
    //String hexString = String(hexCode);
    //hexString.toUpperCase();

    //ignore repeated/burst data (cluster of the same code is decoded as "0")
    if(strcmp("0", hexCode) != 0){
      Serial.println(hexCode);
      int animNumber = getAnimationFromCode(codeToAnim, hexCode);
      //a corresponding animation was found for the received code
      if (animNumber != ANIM_NONE) {
        //Serial.println(animNumber);

        //clear old animation from queue
        animationQueue.flush();
        currentAnimation=animNumber;

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

  //if current animation frame has been displayed for the necessary duration, display next or loop
  if (millis() > animFrameEnd){
    
    //if at end of animation, loop current animation
    if(animationQueue.isEmpty()){
      for(int i = 0; i < MAX_ANIM_FRAMES; i++){
        //if the frame has 0 ms, it can be ignored
          if(animationData[currentAnimation][i].millis != 0){
            animationQueue.push(&animationData[currentAnimation][i]);
          }
      }
    }

    animationFrame frame;
    animationQueue.pop(&frame);
    currentExpression = frame.expression;

    animFrameStart = millis();
    animFrameEnd = millis() + frame.millis;
    Serial.print("Current Expression: ");
    Serial.println(currentExpression);
  }
}
