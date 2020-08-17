/*
   Midi pedals
*/
//display pins
#define TFT_SCLK 13  // SCLK can also use pin 14
#define TFT_MOSI 11  // MOSI can also use pin 7
#define TFT_CS   10  // CS & DC can use pins 2, 6, 9, 10, 15, 20, 21, 22, 23
#define TFT_DC    9  //  but certain pairs must NOT be used: 2+10, 6+9, 20+23, 21+22
#define TFT_RST   8  // RST can use any pin
#define SD_CS     4  // CS for SD card, can use any pin

#include <Adafruit_GFX.h>    // Core graphics library
#include <ST7735_t3.h> // Hardware-specific library
#include <SPI.h>
#include <Bounce2.h>
#include <ResponsiveAnalogRead.h>

ST7735_t3 tft = ST7735_t3(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

//mojologo
static const unsigned char PROGMEM mojo_logo[] =
{
  0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x7f, 0x20, 0x10, 0x0f, 0xe7, 0xf0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x01, 0xfe, 0xff, 0x7f, 0x20, 0x10, 0x0f, 0xf7, 0xf8, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x01, 0x80, 0x18, 0x60, 0x20, 0x10, 0x08, 0x36, 0x18, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x01, 0xfc, 0x18, 0x7e, 0x20, 0x10, 0x0f, 0xf7, 0xf8, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x06, 0x18, 0x60, 0x20, 0x10, 0x0f, 0xf7, 0xf8, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x01, 0x86, 0x18, 0x60, 0x30, 0x18, 0x08, 0x36, 0x30, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x01, 0xfe, 0x18, 0x7f, 0x3f, 0xdf, 0xe8, 0x36, 0x18, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x38, 0x00, 0x3f, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x01, 0xfc, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x07, 0xfe, 0x03, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0x80, 0x0f, 0xfe, 0x07, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0xe0, 0x1f, 0xfe, 0x1f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0xe0, 0x3f, 0xfe, 0x3f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0xf0, 0x7f, 0xfe, 0x3f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0xf8, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0x3f, 0xf7, 0xff, 0xff, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0x3f, 0xe7, 0xff, 0xff, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0x1f, 0xc7, 0xff, 0xff, 0x80, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0x0f, 0x87, 0xff, 0xff, 0x80, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x07, 0xfe, 0xff, 0xf8, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x07, 0xfe, 0x7f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x07, 0xfe, 0x7f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x07, 0xfe, 0x3f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x07, 0xfe, 0x1f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x07, 0xfe, 0x0f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x07, 0xfe, 0x03, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x03, 0xfc, 0x00, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x03, 0xf8, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x01, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x07, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x1f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x3f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0xff, 0xf0, 0x7f, 0xf8, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xc0, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x80, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x80, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x80, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfc, 0xff, 0x80, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfc, 0xff, 0xe0, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0xff, 0xf8, 0x7f, 0xf8, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0x80, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfe, 0x00, 0x03, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x00, 0x01, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00

};

/////////////////////////////////////////////
// potentiometers vars

const int NPots = 3; //*
int potPin[NPots] = {1, 2, 3}; //* Pin where the potentiometer is
int potCState[NPots] = {0}; // Current state of the pot
int potPState[NPots] = {0}; // Previous state of the pot

int midiCState[NPots] = {0}; // Current state of the midi value
int midiPState[NPots] = {0}; // Previous state of the midi value

//gotta have wawa
int wahValue = 0;
int wahLow = 15;
int wahHigh = 1022;
int LedPin = 1;

//Buttons!
#define NUM_BUTTONS 1
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {0};
const int but = 9;
boolean bstate[but] = {false};
unsigned long btime[but] = {0};
unsigned long btimer[but] = {0};
int bcolor = ST7735_BLUE;
uint16_t timeout = 1500;
boolean calwah = false;

//LEDS
const int leds = 4;
int ledPin[leds] = {20, 21, 22, 23};
int ledState[leds] = {0};
int ledPState[leds] = {0};
unsigned long previousMillis[leds] = {0};
long on[leds] = {20, 100, 120, 150};
long off[leds] = {120, 150, 200, 250};
int ledOn = 0;
/////////////////////////////////////////////
//Midi setup
int note[but] = {60, 61, 62, 63, 64, 65, 66, 67, 68};
int pnote[but] = {0};
int octave = 60;
int octlast = octave;
byte midiCh = 3; //* MIDI channel to be used
byte cc = 1; //* Lowest MIDI CC to be used
int counter = 1 ; // midi clock count
int play_flag = 0; // start stop var

//swirl vars
#define RGB(r,g,b) (b<<11|g<<6|r)
static int h = 0;
uint8_t r, g, b;

ResponsiveAnalogRead analog[NPots] = {
  ResponsiveAnalogRead(1, true, 0.1),
  ResponsiveAnalogRead(2, false),
  ResponsiveAnalogRead(3, true)
};
Bounce * pushbutton = new Bounce[NUM_BUTTONS];
#define DEBUG

void setup() {

  //  pinMode(SD_CS, INPUT_PULLUP);  // don't touch the SD card
  screensetup();
  switchsetup();
  ledsetup();
  ledSwitch(ledOn);
}

void loop() {

  buttonread();
  timer();
  octaveswitch ();
  colorswirl();
  screenDisplay();

} // END LOOP

void switchsetup() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pushbutton[i].attach( BUTTON_PINS[i] , INPUT_PULLUP  );
    pushbutton[i].interval(25);
  }

}
void octaveswitch () {
  if (counter > (leds - 1 )) {
    counter = 0;
  }
  for (int i = 0; i < NUM_BUTTONS; i++)  {
    pushbutton[i].update();
    if (pushbutton[i].rose()) {
      changeOctave(counter);
      ledSwitch(counter);
      counter++;
    }
  }
}

int changeOctave(int a) {
  for (int i = 4; i < but; i++) {
    switch (a) {
      case 0:
        note[i] = constrain((pnote[i] + 10), (60 + i), (60 + i));
        break;
      case 1:
        note[i] = constrain((pnote[i] - 10), (70 + i), 78);
        break;
      case 2:
        note[i] = constrain((pnote[i] + 10), 40, (40 + i));
        break;
      case 3:
        note[i] = constrain((pnote[i] - 10), (50 + i), (50 + i));
        break;
      default:
        Serial.print("default");
        Serial.print('\n');
    }
  }
}
/////led setup

void ledsetup() {
  for (int i = 0; i < leds; i++) {
    pinMode(ledPin[i], OUTPUT);
    ledState[i] = HIGH;
    digitalWrite(ledPin[i], ledState[i]);
    delay(100);
    ledState[i] = LOW;
    digitalWrite(ledPin[i], ledState[i]);
  }
}

void ledLoop() {
  unsigned long currentMillis = millis();
  for (int i = 0; i < leds; i++) {
    if (ledState[i] == HIGH && currentMillis - previousMillis[i] >= on[i]) {
      ledState[i] = LOW;
      previousMillis[i] = currentMillis;
      digitalWrite(ledPin[i], ledState[i]);
    } else if (ledState[i] == LOW && currentMillis - previousMillis[i] >= off[i]) {
      ledState[i] = HIGH;
      previousMillis[i] = currentMillis;
      digitalWrite(ledPin[i], ledState[i]);
    }
  }
}

void ledReset() {
  for (int x = 0; x < leds; x++) {
    ledState[x] = 0;
    digitalWrite(ledPin[x], ledState[x]);
  }
  ledSwitch(ledOn);
  tft.fillCircle(15, 60, 10, ST7735_WHITE);
  tft.fillCircle(114, 60, 10, ST7735_WHITE);
}

void ledSwitch(int led) {
  for (int i = 0; i < leds; i++) {
    switch (led) {
      case 0 :
        ledState[i] = LOW;
        digitalWrite(ledPin[i], ledState[i]);
        ledState[0] = HIGH;
        ledOn = 0;
        digitalWrite(ledPin[0], ledState[0]);
        bcolor = ST7735_BLUE;
        tft.drawBitmap(2, 10, mojo_logo, 128, 64, bcolor);
        break;
      case 1 :
        ledState[i] = LOW;
        digitalWrite(ledPin[i], ledState[i]);
        ledState[1] = HIGH;
        ledOn = 1;
        digitalWrite(ledPin[1], ledState[1]);
        bcolor = ST7735_RED;
        tft.drawBitmap(2, 10, mojo_logo, 128, 64, bcolor);
        break;
      case 2 :
        ledState[i] = LOW;
        digitalWrite(ledPin[i], ledState[i]);
        ledState[2] = HIGH;
        ledOn = 2;
        digitalWrite(ledPin[2], ledState[2]);
        bcolor = ST7735_CYAN;
        tft.drawBitmap(2, 10, mojo_logo, 128, 64, bcolor);
        break;
      case 3 :
        ledState[i] = LOW;
        digitalWrite(ledPin[i], ledState[i]);
        ledState[3] = HIGH;
        ledOn = 3;
        digitalWrite(ledPin[3], ledState[3]);
        bcolor = ST7735_MAGENTA;
        tft.drawBitmap(2, 10, mojo_logo, 128, 64, bcolor);
        break;
      default:
        Serial.print("default");
        Serial.print('\n');
    }
  }
}

//button timers
void timer() {
  for (int i = 0; i < but; i++) {
    btime[i] = millis();
    if ((btime[4] - btimer[4] > timeout) && bstate[4] == true) { // mxr0 state
      //      bcolor = ST7735_GREEN;
      tft.drawBitmap(2, 10, mojo_logo, 128, 64, ST7735_GREEN);
      calibrate();
      ledLoop();
    } else if ((btime[5] - btimer[5] > timeout) && bstate[5] == true) { // mxr1 state
      //ledLoop();
      //      tft.fillCircle(15, 60, 10, bcolor);

    } else if ((btime[6] - btimer[6] > timeout) && bstate[6] == true) { // mxr2 state
      //ledLoop();

      //      tft.fillCircle(114, 60, 10, bcolor);
      //      tft.drawBitmap(2, 10, mojo_logo, 128, 64, bcolor);
    } else if ((btime[7] - btimer[7] > timeout) && bstate[7]) { // mxr4 state
      //ledLoop();

      //      tft.fillCircle(114, 30, 10, bcolor);
      //      tft.drawBitmap(2, 10, mojo_logo, 128, 64, bcolor);
    } else if ((btime[8] - btimer[8] > timeout) && bstate[8]) { // mxr3 state
      //ledLoop();

      //      tft.fillCircle(15, 30, 10, bcolor);
      //      tft.drawBitmap(2, 10, mojo_logo, 128, 64, bcolor);
    }
  }
}
/////////////////////////////////////////////////////////////
// POTENTIOMETERS(buttons/wah)
void buttonread() {
  for ( int i = 0; i < NPots; i++) {
    analog[i].update();
    if (analog[i].hasChanged()) {
#ifdef DEBUG
      Serial.print(i);
      Serial.print(':');
      Serial.print(potCState[i]);
      Serial.print(':');
      Serial.print(wahLow);
      Serial.print(':');
      Serial.print(wahHigh);
      Serial.print('\n');
#endif
      potCState[i] = analog[i].getValue(); // Reads the pot and stores it in the potCState variable
      if ( i == 1) {
        midiCState[i] = constrain(map(potCState[i], wahLow, wahHigh, 127, 0), 0, 127);  //maths for expression pedal
      } else {
        midiCState[i] = map(potCState[i], 0, 1023, 0, 127);
      }
      if (i == 1 && midiPState[1] != midiCState[1]) { //WAWA
        usbMIDI.sendControlChange(cc, midiCState[1], midiCh);
      }
      potPState[i] = potCState[i]; // Stores the current reading of the potentiometer to compare with the next
      midiPState[i] = midiCState[i];

      for ( int j = 0; j < but; j++) {
#ifdef DEBUG
        Serial.print(j);
        Serial.print(':');
        Serial.print(bstate[j]);
        Serial.print(':');
        Serial.print(note[j]);
        Serial.print(':');
        Serial.print(pnote[j]);
        Serial.print(':');
        Serial.print(btime[j]);
        Serial.print(':');
        Serial.print(btimer[j]);
        Serial.print('\n');
#endif
        if (pnote[j] <= 0) {
          pnote[j] = note[j];
        }

        if (i == 0 && potCState[0] > 1000 && bstate[0] == false) { // START BUTTON
          usbMIDI.sendNoteOn(note[0], 99, midiCh);
          bstate[0] = true;
          tft.fillRoundRect(15, 80, 50, 38, 8, ST7735_BLACK);
          tft.fillTriangle(30, 82, 30, 112, 55, 97, ST7735_RED);
        } else if (i == 0 && midiCState[0] < 40 && bstate[0] == true ) {
          usbMIDI.sendNoteOff(note[0], 0, midiCh);
          bstate[0] = false;
          tft.fillRoundRect(15, 80, 50, 38, 8, ST7735_WHITE);
          tft.fillTriangle(30, 82, 30, 112, 55, 97, ST7735_GREEN);
        } else if (i == 0 && potCState[0] > 920 && potCState[0] < 950 && bstate[1] == false) { // STOP
          usbMIDI.sendNoteOn(note[1], 127, midiCh);
          bstate[1] = true;
          tft.fillRoundRect(67, 80, 50, 38, 8, ST7735_BLACK);
          tft.fillRoundRect(77, 88, 31, 22, 2, ST7735_CYAN);
        }  else if (i == 0 && midiCState[0] < 40 && bstate[1] == true ) {
          usbMIDI.sendNoteOff(note[1], 0, midiCh);
          bstate[1] = false;
          tft.fillRoundRect(67, 80, 50, 38, 8, ST7735_WHITE);
          tft.fillRoundRect(77, 88, 31, 22, 2, ST7735_RED);
        } else if (i == 0 && potCState[0] > 845 && potCState[0] < 855 && bstate[2] == false) {  // UP
          usbMIDI.sendNoteOn(note[2], 127, midiCh);
          bstate[2] = true;
          tft.fillCircle(80, 30, 4, bcolor);
        } else if (i == 0 && midiCState[0] < 40 && bstate[2] == true ) {
          usbMIDI.sendNoteOff(note[2], 0, midiCh);
          bstate[2] = false;
          tft.fillCircle(80, 30, 4, ST7735_WHITE);
        } else if (i == 0 && midiCState[0] > 95 && midiCState[0] < 99 && bstate[3] == false) { //DOWN
          usbMIDI.sendNoteOn(note[3], 127, midiCh);
          bstate[3] = true;
          tft.fillCircle(80, 60, 4, bcolor);
        } else if (i == 0 && midiCState[0] < 40 && bstate[3] == true ) {
          usbMIDI.sendNoteOff(note[3], 0, midiCh);
          bstate[3] = false;
          tft.fillCircle(80, 60, 4, ST7735_WHITE);
        } else if (i == 2 && midiCState[2] > 80 && midiCState[2] < 90 && bstate[4] == false) { //mxr0
          usbMIDI.sendNoteOn(note[4], 127, midiCh);
          bstate[4] = true;
          tft.drawBitmap(2, 10, mojo_logo, 128, 64, ST7735_GREENISH);
          btimer[4] = btime[4];
        }  else if (i == 2 && midiCState[2] < 40 && bstate[4] == true ) {
          usbMIDI.sendNoteOff(note[4], 0, midiCh);
          bstate[4] = false;
          tft.drawBitmap(2, 10, mojo_logo, 128, 64, bcolor);
          btimer[4] = 0;
          ledReset();
        } else if (i == 2 && midiCState[2] > 89 && midiCState[2] < 95 && bstate[5] == false) {  // mxr1
          usbMIDI.sendNoteOn(note[5], 127, midiCh);
          bstate[5] = true;
          tft.fillCircle(15, 60, 10, bcolor);
          btimer[5] = btime[5];
          pnote[5] = note[5];
        } else if (i == 2 && midiCState[2] < 40 && bstate[5] == true ) {
          if (pnote[5] != note[5]) {
            usbMIDI.sendNoteOff(pnote[5], 0, midiCh);
          }
          usbMIDI.sendNoteOff(note[5], 0, midiCh);
          bstate[5] = false;
          tft.fillCircle(15, 60, 10, ST7735_WHITE);
          btimer[5] = 0;
        } else if (i == 2 && midiCState[2] > 96 && midiCState[2] < 100 && bstate[6] == false) { //mxr2
          usbMIDI.sendNoteOn(note[6], 127, midiCh);
          bstate[6] = true;
          tft.fillCircle(114, 60, 10, bcolor);
          btimer[6] = btime[6];
          pnote[6] = note[6];
        } else if (i == 2 && midiCState[2] < 40 && bstate[6] == true ) {
          if (pnote[6] != note[6]) {
            usbMIDI.sendNoteOff(pnote[6], 0, midiCh);
          }
          usbMIDI.sendNoteOff(note[6], 0, midiCh);
          bstate[6] = false;
          tft.fillCircle(114, 60, 10, ST7735_WHITE);
          btimer[6] = 0;
        } else if (i == 2 && midiCState[2] > 104 && midiCState[2] < 116 && bstate[7] == false) { //mxr4
          usbMIDI.sendNoteOn(note[8], 127, midiCh);
          bstate[7] = true;
          tft.fillCircle(114, 30, 10, bcolor);
          btimer[7] = btime[7];
          pnote[8] = note[8];
        } else if (i == 2 && midiCState[2] < 100 && bstate[7] == true ) {
          if (pnote[8] != note[8]) {
            usbMIDI.sendNoteOff(pnote[8], 0, midiCh);
          }
          usbMIDI.sendNoteOff(note[8], 0, midiCh);
          bstate[7] = false;
          tft.fillCircle(114, 30, 10, ST7735_WHITE);
          btimer[7] = 0;
        } else if (i == 2 && midiCState[2] > 115 && bstate[8] == false) { //mxr3 (wired last in chain
          usbMIDI.sendNoteOn(note[7], 127, midiCh);
          bstate[8] = true;
          tft.fillCircle(15, 30, 10, bcolor);
          btimer[8] = btime[8];
          pnote[7] = note[7];
        } else if (i == 2 && midiCState[2] < 110 && bstate[8] == true ) {
          if (pnote[7] != note[7]) {
            usbMIDI.sendNoteOff(pnote[7], 0, midiCh);
          }
          usbMIDI.sendNoteOff(note[7], 0, midiCh);
          bstate[8] = false;
          tft.fillCircle(15, 30, 10, ST7735_WHITE);
          btimer[8] = 0;
        }
      }

    }
  }
}

void calibrate() {
#ifdef DEBUG
  Serial.print("calibrating");
  Serial.print('\n');
#endif
  if (bstate[8] == true) {
    //    Serial.print(" LOW RESET");
    //    Serial.print('\n');
    wahHigh = 600;
  } else if (bstate[7] == true) {
    //    Serial.print(" HIGH RESET");
    //    Serial.print('\n');
    wahLow = 700;
  }
  // turn on the indicator LED to indicate that calibration is happening:
  //  digitalWrite(LedPin, HIGH);
  // read the sensor:
  wahValue = potCState[1];

  // record the maximum sensor value
  if (wahValue > wahHigh) {
    wahHigh = wahValue - 100 ;
    //    Serial.print("LOW SET");
    //    Serial.print('\n');
    tft.fillCircle(15, 60, 10, bcolor);
  }

  // record the minimum sensor value
  if (wahValue < wahLow) {
    wahLow = wahValue + 10;
    //    Serial.print("HIGH SET");
    //    Serial.print('\n');
    tft.fillCircle(114, 60, 10, bcolor);
  }

}
void colorswirl() {
  //  static int h = 0;
  //
  //  uint8_t r, g, b;
  if (h < 32) {
    r = 31; g = h; b = 0;
  } else if (h < 64) {
    r = 63 - h; g = 31; b = 0;
  } else if (h < 96) {
    r = 0; g = 31; b = h - 64;
  } else if (h < 128) {
    r = 0; g = 127 - h; b = 31;
  } else if (h < 160) {
    r = h - 128; g = 0; b = 31;
  } else if (h < 192) {
    r = 31; g = 0; b = 191 - h;
  } else {
    r = 31; g = 0; b = 0;
    h = 0;
  }

  h++;
}
void screensetup() {
  tft.initR(INITR_144GREENTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE, ST7735_BLUE);
  tft.setTextSize(1);
  tft.setTextWrap(false);
  tft.fillRoundRect(0, 0, 128, 8, 0, ST7735_BLUE);
  tft.drawBitmap(2, 10, mojo_logo, 128, 64, ST7735_BLUE);
  tft.fillCircle(15, 30, 10, ST7735_WHITE);
  tft.fillCircle(15, 60, 10, ST7735_WHITE);
  tft.fillCircle(80, 30, 4, ST7735_WHITE);
  tft.fillCircle(80, 60, 4, ST7735_WHITE);
  tft.fillCircle(114, 30, 10, ST7735_WHITE);
  tft.fillCircle(114, 60, 10, ST7735_WHITE);
  tft.fillRoundRect(15, 80, 50, 38, 8, ST7735_WHITE);
  tft.fillTriangle(30, 82, 30, 112, 55, 97, ST7735_GREEN);
  tft.fillRoundRect(67, 80, 50, 38, 8, ST7735_WHITE);
  tft.fillRoundRect(77, 88, 31, 22, 2, ST7735_RED);
}

void screenDisplay() {
  tft.setCursor(0, 4);
  tft.print(Format(potCState[0], 4, 0));
  tft.setCursor(45, 4);
  tft.print(Format(midiCState[1], 4, 0)); //wawa
  tft.setCursor(80, 4);
  tft.print(Format(midiCState[2], 4, 0));

  int bar = midiCState[1];
  tft.fillRect(bar, 120, 128 - bar, 8, ST7735_GREY);
  tft.fillRect(3, 120, bar , 8, RGB(r, g, bar));
  tft.fillRect(bar, 120, 3, 8, ST7735_BLACK);
}
String Format(double val, int dec, int dig ) {

  // this is my simple way of formatting a number
  // data = Format(number, digits, decimals) when needed

  int addpad = 0;
  char sbuf[20];
  String fdata = (dtostrf(val, dec, dig, sbuf));
  int slen = fdata.length();
  for ( addpad = 1; addpad <= dec + dig - slen; addpad++) {
    fdata = " " + fdata;
  }
  return (fdata);

}

