// Include Libraries
#include "Arduino.h"
#include "NewPing.h"
#include "Adafruit_NeoPixel.h"
#include "SevSeg.h"

// Pin Definitions
#define HCSR04_PIN_TRIG     3
#define HCSR04_PIN_ECHO     2
#define LEDRGB_PIN_DIN      4
#define S7SEG_PIN_DIG1      10
#define S7SEG_PIN_DIG2      11
#define S7SEG_PIN_DIG3      12
#define S7SEG_PIN_DIG4      13
#define S7SEG_PIN_A         5
#define S7SEG_PIN_B         6
#define S7SEG_PIN_C         7
#define S7SEG_PIN_D         8
#define S7SEG_PIN_E         14
#define S7SEG_PIN_F         15
#define S7SEG_PIN_G         16
#define S7SEG_PIN_DECIMAL   9

// Global variables and defines
#define LedRGB_NUMOFLEDS    1

byte s7segDigitPins[]   = { S7SEG_PIN_DIG1, S7SEG_PIN_DIG2, S7SEG_PIN_DIG3, S7SEG_PIN_DIG4 };
byte s7segSegmentPins[] = { S7SEG_PIN_A, S7SEG_PIN_B, S7SEG_PIN_C, S7SEG_PIN_D, S7SEG_PIN_E, S7SEG_PIN_F, S7SEG_PIN_G, S7SEG_PIN_DECIMAL };

// Object initialization
NewPing hcsr04(HCSR04_PIN_TRIG, HCSR04_PIN_ECHO);
Adafruit_NeoPixel LedRGB(LedRGB_NUMOFLEDS, LEDRGB_PIN_DIN, NEO_GRB + NEO_KHZ800);
SevSeg s7seg(s7segSegmentPins, s7segDigitPins, 4, COMMON_CATHODE, false, false, false);

// Setup
void setup() {
    Serial.begin(9600);
    while (!Serial);

    Serial.println("System Initialized");

    LedRGB.begin();
    LedRGB.setBrightness(100);
    LedRGB.show();  // Ensure the LED is initially off

    s7seg.setBrightness(90);
}

// Main loop
void loop() {
    // Read distance (in cm)
    int distance_cm = hcsr04.ping_cm();

    if (distance_cm > 0) {
        // Convert cm to inches
        float distance_in = distance_cm * 0.393701;
        float limited_dist = constrain(distance_in, 2.0, 40.0);

        // Display on 7-segment
        s7seg.setNumber(limited_dist, 1); // 1 decimal place
        s7seg.refreshDisplay();

        // Linear interpolate color from red to green
        float t = (limited_dist - 2.0) / (40.0 - 2.0);
        t = constrain(t, 0.0, 1.0);
        int red   = (1.0 - t) * 255;
        int green = t * 255;

        LedRGB.setPixelColor(0, LedRGB.Color(red, green, 0));
        LedRGB.show();

        // Debug output
        Serial.print("Distance: ");
        Serial.print(distance_cm);
        Serial.print(" cm / ");
        Serial.print(limited_dist, 2);
        Serial.println(" in");
    } else {
        // No object detected: turn off LED and display dashes
        LedRGB.setPixelColor(0, LedRGB.Color(0, 0, 0));
        LedRGB.show();
        s7seg.setChars("----");
        s7seg.refreshDisplay();
        Serial.println("No object detected.");
    }

    delay(100);  // Limit update rate
}
