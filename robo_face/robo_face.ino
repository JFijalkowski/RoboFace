#include <FastLED.h>
#include <IRremote.hpp>
#include "expressions.h"
#include "animations.h"
#include <cppQueue.h>


#define LED_PIN 7
#define LED_DATA_PIN 11
#define IR_RECEIVE_PIN 9

#define DECODE_NEC

//TODO
//Friendship ended with queue data type, just make list of animation frames instead
//global variable to track position in animation
//if reaching the end (and index is 8, or 
//cppQueue animationQueue(4, MAX_ANIM_FRAMES, FIFO);

unsigned long animFrameEnd;
int currentAnimation;
int currentAnimationFrame;
int currentExpression;
animationFrame animationQueue[MAX_ANIM_FRAMES];
CRGB leds[TOTAL_PIXELS];


void addAnimationToQueue(animationFrame queue[], animationFrame frames[]){
  
  //end current animation frame if haven't already
  animFrameEnd = millis();
	for(int i = 0; i < MAX_ANIM_FRAMES; i++){
    /*
    Serial.print("Adding expression to queue: ");
    Serial.println(pgm_read_word(&(frames[i].expression)));
    */
		queue[i] = {pgm_read_word(&(frames[i].expression)), pgm_read_word(&(frames[i].millis))};
  }
}

//increment animation frame counter and set new expression + timer
void advanceAnimation(){
	currentAnimationFrame++;
  
	currentExpression = animationQueue[currentAnimationFrame].expression;
  animFrameEnd = millis() + animationQueue[currentAnimationFrame].millis;
  /*
  Serial.print("Current Frame: ");
  Serial.print(currentAnimationFrame);
  Serial.print(", Displaying ");
  Serial.print(currentExpression);
  Serial.print(" for ");
  Serial.println(animationQueue[currentAnimationFrame].millis);
  */
}

void showExpression(){
  //get pixel pixel data for expression and apply to LEDs
  for(int i = 0; i < TOTAL_PIXELS; i++) {
    const int r = pgm_read_word(&expressionData[currentExpression][i].r);
    const int g = pgm_read_word(&expressionData[currentExpression][i].g);
    const int b = pgm_read_word(&expressionData[currentExpression][i].b);
    leds[i].red = r;
    leds[i].green = g;
    leds[i].blue = b;
  }
  FastLED.show();
}

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("Started up"); 

  FastLED.addLeds<WS2812, LED_DATA_PIN, GRB>(leds, TOTAL_PIXELS);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(32);
  
  
  //start animation is default w/ blinking
  currentAnimation = ANIM_DEFAULT;
  
  addAnimationToQueue(animationQueue, animationData[currentAnimation]);
  currentAnimationFrame = -1;
  advanceAnimation();

  for(int i = 0; i < MAX_ANIM_FRAMES; i++){
    Serial.print("queue position ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(animationQueue[i].expression);
  }

  Serial.print("Current Expression: ");
  Serial.println(currentExpression);
  Serial.println("Until: ");
  Serial.println(animFrameEnd);

  showExpression();
}

void loop() {
  //if an IR data packet has been received
  if (IrReceiver.decode()) {

    //convert received data to 8 character hex code
    auto recvData = IrReceiver.decodedIRData.decodedRawData;
    char hexCode [9];
    ltoa(recvData, hexCode, 16);
    //Serial.println(hexCode);

    //ignore repeated/burst data (cluster of the same code is decoded as "0")
    if(strcmp("0", hexCode) != 0){
      int animNumber = getAnimationFromCode(codeToAnim, hexCode);
      //a corresponding animation was found for the received code
      if (animNumber != ANIM_NONE) {
        //update current animation to selected
        currentAnimation=animNumber;

        addAnimationToQueue(animationQueue, animationData[currentAnimation]);

      }
    }
    IrReceiver.resume(); // Enable receiving of the next value
  }

  //if current animation frame has been displayed for the necessary duration, display next or loop
  if (millis() > animFrameEnd){
    advanceAnimation();

    //if at end of animation, loop current animation
    if(currentAnimationFrame == MAX_ANIM_FRAMES || animationQueue[currentAnimationFrame].millis == 0){
      addAnimationToQueue(animationQueue, animationData[currentAnimation]);
      currentAnimationFrame = -1;
      advanceAnimation();
    }
    
    showExpression();
  }
}
