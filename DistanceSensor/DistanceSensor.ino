
// Include Libraries
#include "Arduino.h"
#include "NewPing.h"
#include "Adafruit_NeoPixel.h"
#include "SevSeg.h"


// Pin Definitions
#define HCSR04_PIN_TRIG	3
#define HCSR04_PIN_ECHO	2
#define LEDRGB_PIN_DIN	4
#define S7SEG_PIN_DIG1	10
#define S7SEG_PIN_DIG2	11
#define S7SEG_PIN_DIG3	12
#define S7SEG_PIN_DIG4	13
#define S7SEG_PIN_A	5
#define S7SEG_PIN_B	6
#define S7SEG_PIN_C	7
#define S7SEG_PIN_D	8
#define S7SEG_PIN_E	14
#define S7SEG_PIN_F	15
#define S7SEG_PIN_G	16
#define S7SEG_PIN_DECIMAL	9



// Global variables and defines
#define LedRGB_NUMOFLEDS 1
int s7segCounter = 0; 
unsigned long s7segTimer = millis();
byte s7segDigitPins[] = { S7SEG_PIN_DIG1, S7SEG_PIN_DIG2, S7SEG_PIN_DIG3, S7SEG_PIN_DIG4 };
byte s7segSegmentPins[] = { S7SEG_PIN_A, S7SEG_PIN_B, S7SEG_PIN_C, S7SEG_PIN_D, S7SEG_PIN_E, S7SEG_PIN_F ,S7SEG_PIN_G, S7SEG_PIN_DECIMAL };
// object initialization
NewPing hcsr04(HCSR04_PIN_TRIG,HCSR04_PIN_ECHO);
Adafruit_NeoPixel LedRGB(LEDRGB_PIN_DIN);
SevSeg s7seg(s7segDigitPins, s7segSegmentPins);


// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    LedRGB.begin(); // This initializes the NeoPixel library.
    LedRGB.show(); // Initialize all leds to 'off'
    s7seg.setBrightness(90);      //Seven-Segment LED brightness 0 - 100
    menuOption = menu();
    
}

void loop() {
    int distance_cm = hcsr04.ping_cm();

    if (distance_cm > 0) {
        float distance_in = distance_cm * 0.393701;
        float limited_dist = constrain(distance_in, 2.0, 40.0);

        s7seg.setNumber(limited_dist, 1); // update number

        float t = (limited_dist - 2.0) / (40.0 - 2.0);
        t = constrain(t, 0.0, 1.0);
        int red   = (1.0 - t) * 255;
        int green = t * 255;
        LedRGB.setPixelColor(0, LedRGB.Color(red, green, 0));
        LedRGB.show();

        Serial.print("Distance: ");
        Serial.print(distance_cm);
        Serial.print(" cm / ");
        Serial.print(limited_dist, 2);
        Serial.println(" in");
    } else {
        LedRGB.setPixelColor(0, LedRGB.Color(0, 0, 0));
        LedRGB.show();
        s7seg.setChars("----");
        Serial.println("No object detected.");
    }

    // 👇 This must always be outside the conditionals
    s7seg.refreshDisplay();

    delay(10);  // reduce delay to improve refresh rate
}
