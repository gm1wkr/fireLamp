/*
|-------------------------------------------------------------------------------|
| PHYSICAL PROJECT - Selanite Crustal Tower. PROTOTYPE
|
| Target: Espressif ESP32
|
| GPIO 12 - 24px WS2812B ring uplight for crystal
| GPIO 14 - 6px WS2812B top backlight
| GPIO 27 - 8px WS2812B bottom backlight
| GPIO 10 - RESERVED for soft run switch
|
|------------------------------------------------------------------------------ |
*/

#include<Arduino.h>
#define FASTLED_INTERNAL
#include<FastLED.h>

#define NUM_LEDS            24
#define LED_PIN             12

#define NUM_LEDS_TOP        8
#define LED_PIN_TOP         27

#define NUM_LEDS_BOTTOM     6
#define LED_PIN_BOTTOM      14

#define COLOR_ORDER         GRB
#define BRIGHTNESS          160

CRGB    leds[NUM_LEDS];
CRGB    ledsBottom[NUM_LEDS_TOP];
CRGB    ledsTop[NUM_LEDS_BOTTOM];

uint8_t data[NUM_LEDS];

uint8_t paletteIndex = 0;
uint8_t colourIndex[NUM_LEDS];

DEFINE_GRADIENT_PALETTE(pBlueIce){
    0, 0, 30, 127,
    16, 0, 90, 255,
    32, 0, 255, 255,
    64, 0, 255, 100,
    128, 0, 255, 176,
    122, 250, 236, 190,
    196, 237, 250, 190,
    255, 0, 200, 40
};

DEFINE_GRADIENT_PALETTE(pFire){
    0, 230, 60, 0,
    16, 255, 0, 0,
    48, 255, 70, 0,
    76, 255, 180, 0,
    128, 255, 60, 0,
    196, 122, 0, 0,
    220, 186, 64 , 0,
    255, 255, 20, 0,
};


void add_glitter();
void blueIce();
void iceLamp();
void fireLamp();
void sineTest();


void setup() {
        // Array of random colours from palette
    for(uint8_t i = 0; i < NUM_LEDS; i++){
        colourIndex[i] = random8();
    }

    FastLED.addLeds<WS2812B,LED_PIN, COLOR_ORDER>(leds,NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<WS2812B,LED_PIN_TOP, COLOR_ORDER>(ledsTop,NUM_LEDS_TOP);
    FastLED.addLeds<WS2812B,LED_PIN_BOTTOM, COLOR_ORDER>(ledsBottom,NUM_LEDS_BOTTOM);
    FastLED.setBrightness(BRIGHTNESS);

    // for (uint8_t i = 0; i < NUM_LEDS_TOP; i++){
    //     ledsTop[i] = CHSV(155, 255, 64);
    // }

    // for (uint8_t i = 0; i < NUM_LEDS_BOTTOM; i++){
    //     ledsBottom[i] = CHSV(20, 255, 64);
    // }
}


void loop() {
    // blueIce(); 
    fireLamp();
    // iceLamp();
    // add_glitter();

    FastLED.delay(20);
    FastLED.show();
}

void sineTest()
{
    uint8_t sineBeat = beatsin8(15, NUM_LEDS - 1, 0, 0);
    leds[sineBeat] = CRGB::Blue;
    fadeToBlackBy(leds, NUM_LEDS - 1, 2);

    // EVERY_N_MILLISECONDS(10){

    // }

    FastLED.show();
}

void blueIce() 
{
    CRGBPalette16 ice = pBlueIce;
    uint8_t brightness = beatsin8(6, 64, 160, 0, 0);

    // for(uint8_t i = 0; i < NUM_LEDS; i++){
    //     leds[i] = ColorFromPalette(ice, colourIndex[i]);
    // }

    // fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, ice, brightness, LINEARBLEND);

    // FastLED.show();

    for(uint8_t i = NUM_LEDS - 1; i < 0; i--){
        leds[i] = ColorFromPalette(ice, colourIndex[i]);
    }

    fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, ice, brightness, LINEARBLEND);

    EVERY_N_MILLISECONDS(20){
        fadeToBlackBy(leds, NUM_LEDS, 64);
    }

    EVERY_N_MILLISECONDS(120) {
        paletteIndex++;
    }
}


void iceLamp()
{
    CRGBPalette16 ice = pBlueIce;

    uint8_t brightnessSine1 = beatsin8(1, 32, 96, 0, 0);
    uint8_t brightnessSine2 = beatsin8(2, 0, 255, 0, 0);
    uint8_t brightnessSine3 = beatsin8(7, 128, 230, 0, 0);
    uint8_t brightness = (brightnessSine1 + brightnessSine2 + brightnessSine3) / 3;
    uint8_t hueStart = 60;
    fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, ice, brightness, LINEARBLEND); 

    for(int i = 0; i < NUM_LEDS - 1; i++){
        // if(i >= 0 && i <= NUM_LEDS / 4){
        //     leds[i] = CRGB(255, 0, 0);
        // }
        // if(i > NUM_LEDS / 4 && i <= (NUM_LEDS / 2)){
        //     leds[i] = CRGB(0, 255, 0);
        // }
        if(i > (NUM_LEDS / 2) && i <= (NUM_LEDS / 4) + (NUM_LEDS / 2)){
            uint8_t lum1 = beatsin8(1, 127, 255);
            uint8_t lum2 = beatsin8(7, 148, 255);
            uint8_t lum3 = beatsin8(12, 16, 255);
            uint8_t lum = (lum1 + lum2 + lum3) / 3;
            leds[i] = CHSV(hueStart + 10, 255, lum);
            hueStart+=20;
        }
    }

    EVERY_N_MILLISECONDS(50){
        paletteIndex++;
    }

}


void fireLamp() 
{
    CRGBPalette16 firePallete = pFire;
    bool flipDirection = true;

    uint8_t brightnessSine1 = beatsin8(1, 64, 180, 0, 0);
    uint8_t brightnessSine2 = beatsin8(3, 64, 180, 0, 0);
    uint8_t brightnessSine3 = beatsin8(11, 32, 200, 0, 0);
    uint8_t brightness = (brightnessSine1 + brightnessSine2 + brightnessSine3) / 3;

    fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, firePallete, brightness, LINEARBLEND);    
    // add_glitter();

    EVERY_N_MILLISECONDS(200){
        paletteIndex++;
    }

    uint8_t lum = beatsin8(8, 128, 255);
    uint8_t lum2 = beatsin8(3, 220, 255);
    uint8_t lum3 = beatsin8(1, 64, 255);
    uint8_t hue = sin8((millis() / 333) - 20);
    uint8_t hue2 = cos8(millis() / 1000);

    leds[5] = CHSV(hue / 2, 255, lum); 
    leds[6] = CHSV(hue2 / 2, 255, (lum + lum2 + lum3) / 3);
    leds[14] = CHSV(hue / 2, 255, lum);
    leds[15] = CHSV(hue2 / 2, 255, (lum + lum2 + lum3) / 3);

    EVERY_N_MILLISECONDS(40){
        for (uint8_t i = 0; i < NUM_LEDS_TOP; i++){
            ledsTop[i] = CHSV(hue, 255, 64);
        }

        for (uint8_t i = 0; i < NUM_LEDS_BOTTOM; i++){
            ledsBottom[i] = CHSV(hue2, 255, 64);
        }
    }
}


void add_glitter()
{
  int chance_of_glitter =  1;
  int number_of_glitters =  4;


  int r = random8(100);
  if( r < chance_of_glitter ) {
    for( int j = 0; j < number_of_glitters; j++) {
      int pos = random16( NUM_LEDS);
      leds[pos] = CRGB::White;
      pos = random16( NUM_LEDS);
      leds[pos] = CRGB::Red;
    }
  }
}
