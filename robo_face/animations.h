#define TOTAL_CODES 21
#define MAX_ANIM_FRAMES 7

typedef struct animationFrame {
	int expression;
	int millis;
};
typedef struct codeMap {
	char code[9];
	int animation;
};
enum Button {
	BTN_POWER, 
	BTN_V_UP, 
	BTN_FUNC, 
	BTN_REWIND, 
	BTN_PLAY, 
	BTN_FWD, 
	BTN_DOWN, 
	BTN_V_DOWN, 
	BTN_UP, 
	BTN_0, 
	BTN_EQ, 
	BTN_REPT, 
	BTN_1, 
	BTN_2, 
	BTN_3, 
	BTN_4, 
	BTN_5, 
	BTN_6, 
	BTN_7, 
	BTN_8, 
	BTN_9, 
};
enum Expression {
	ANGRY, 
	BLANK, 
	BLINK1, 
	BLINK2, 
	BLINK3, 
	BLINK4, 
	BLINK5, 
	BLUSH, 
	BSOD1, 
	BSOD2, 
	BUFFERING1, 
	BUFFERING2, 
	BUFFERING3, 
	BUFFERING4, 
	CRYING, 
	HAPPY, 
	HEART1, 
	HEART2, 
	HEART3, 
	HMM, 
	HYPNO1, 
	HYPNO2, 
	HYPNO3, 
	IRRITATED, 
	NEUTRAL, 
	OWO, 
	RAINBOW, 
	SAD, 
	SCREENTEST, 
	SLEEPING, 
	SUNGLASSES, 
	SUS1, 
	SUS2, 
	SUS3, 
	VEXED, 
	EXP_NONE
};
enum Animation { 
	ANIM_DEFAULT, 
	ANIM_HAPPY, 
	ANIM_SUNGLASSES, 
	ANIM_CRYING, 
	ANIM_BUFFERING, 
	ANIM_OWO, 
	ANIM_SLEEPING, 
	ANIM_RAINBOW, 
	ANIM_SCREENTEST, 
	ANIM_BSOD1, 
	ANIM_BSOD2, 
	ANIM_UNUSED, 
	ANIM_HYPNO, 
	ANIM_BLUSH, 
	ANIM_HEART, 
	ANIM_SUS, 
	ANIM_SAD, 
	ANIM_HMM, 
	ANIM_VEXED, 
	ANIM_IRRITATED, 
	ANIM_ANGRY, 
	ANIM_NONE
}; 
const codeMap codeToAnim[21] = { 
	{"ba45ff00", ANIM_DEFAULT}, 
	{"b946ff00", ANIM_HAPPY}, 
	{"b847ff00", ANIM_SUNGLASSES}, 
	{"bb44ff00", ANIM_CRYING}, 
	{"bf40ff00", ANIM_BUFFERING}, 
	{"bc43ff00", ANIM_OWO}, 
	{"f807ff00", ANIM_SLEEPING}, 
	{"ea15ff00", ANIM_RAINBOW}, 
	{"f609ff00", ANIM_SCREENTEST}, 
	{"e916ff00", ANIM_BSOD1}, 
	{"e619ff00", ANIM_BSOD2}, 
	{"f20dff00", ANIM_UNUSED}, 
	{"f30cff00", ANIM_HYPNO}, 
	{"e718ff00", ANIM_BLUSH}, 
	{"a15eff00", ANIM_HEART}, 
	{"f708ff00", ANIM_SUS}, 
	{"e31cff00", ANIM_SAD}, 
	{"a55aff00", ANIM_HMM}, 
	{"bd42ff00", ANIM_VEXED}, 
	{"ad52ff00", ANIM_IRRITATED}, 
	{"b54aff00", ANIM_ANGRY}, 
}; 
PROGMEM const animationFrame animationData[21][7]= {
	{{NEUTRAL, 5000}, {BLINK1, 60}, {BLINK2, 60}, {BLANK, 60}, {BLINK3, 60}, {BLINK4, 60}, {BLINK5, 60}, },
	{{HAPPY, 5000}, },
	{{SUNGLASSES, 5000}, },
	{{CRYING, 5000}, },
	{{BUFFERING1, 100}, {BUFFERING2, 100}, {BUFFERING3, 100}, {BUFFERING4, 100}, },
	{{OWO, 5000}, },
	{{SLEEPING, 5000}, },
	{{RAINBOW, 5000}, },
	{{SCREENTEST, 5000}, },
	{{BSOD1, 5000}, },
	{{BSOD2, 5000}, },
	{{BLANK, 5000}, },
	{{HYPNO1, 100}, {HYPNO2, 100}, {HYPNO3, 100}, },
	{{BLUSH, 5000}, },
	{{HEART1, 200}, {HEART2, 200}, {HEART1, 200}, {HEART3, 200}, },
	{{SUS1, 500}, {SUS2, 500}, {SUS1, 500}, {SUS3, 500}, },
	{{SAD, 5000}, },
	{{HMM, 5000}, },
	{{VEXED, 5000}, },
	{{IRRITATED, 5000}, },
	{{ANGRY, 5000}, },
};

//find animation associated to IR hex code
int getAnimationFromCode(codeMap codeList[], char code[9]) {
  for (int i = 0; i < TOTAL_CODES; i++ ) {
    if(strcmp(codeList[i].code, code) == 0) {
      return codeList[i].animation;
    }
  }
  //if no match found, return a "no match" number
  return ANIM_NONE;
}
