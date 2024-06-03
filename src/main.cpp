#include <Arduino.h>
#define IR_USE_AVR_TIMER 1
#include <IRremote.hpp>

#define IR_MODULE_PIN 12 // This is the port your connect your IR reciever OUTPUT
#define BTN_RED 3091726080 // Get the raw code from your controller and set your own code for the red button
#define BTN_GREEN 3158572800 // Do the same thing as the red button, but for the green button
#define BTN_BLUE 4127850240 // -||-
#define RESET 4161273600 // Set your code for a reset button on your controller

int rgbs[] = {6, 5, 3};  // Red D3, Green D5, Blue D6
const int count = 3;
int values[count];

unsigned long codes[] = {
    3910598400,  // Zero
    4077715200,  // One
    3877175040,  // Two
    2707357440,  // Three
    4144561920,  // Four
    3810328320,  // Five
    2774204160,  // Six
    3175284480,  // Seven
    2907897600,  // Eight
    3041591040   // Nine
};

String displayed_number = "";

void setup() {
    for (int i = 0; i < count; i++) {
        pinMode(rgbs[i], OUTPUT);
    }
    pinMode(IR_MODULE_PIN, INPUT);
    IrReceiver.begin(IR_MODULE_PIN, ENABLE_LED_FEEDBACK);
    Serial.begin(9600);
}

void set_color(int color) {
    int value = displayed_number.toInt();
    if (value <= 255 && value >= 0) {
        switch (color) {
            case 0:
                Serial.print("red ");
                break;
            case 1:
                Serial.print("green ");
                break;
            case 2:
                Serial.print("blue ");
                break;
            default:
                break;
        }
        Serial.print("led got value: ");
        Serial.println(value);
        values[color] = value;
        analogWrite(rgbs[color], values[color]);
        displayed_number = "";
    } else {
        Serial.println("Value is out of range.");
        displayed_number = "";
    }
}

void reset() {
    for (int i = 0; i < count; i++) {
        analogWrite(rgbs[i], 0);
        values[i] = 0;
    }
    displayed_number = "";
}

void display_number(unsigned long code) {
    for (int i = 0; i < 10; i++) {
        if (code == codes[i]) {
            displayed_number += String(i);
            Serial.println(displayed_number);
        }
    }
}

void loop() {
    if (IrReceiver.decode()) {
        unsigned long recieved_code = IrReceiver.decodedIRData.decodedRawData;
        Serial.print("Number: ");
        display_number(recieved_code);
        Serial.println("");
        Serial.print("Code: ");
        Serial.println(recieved_code);
        switch (recieved_code) {
            case BTN_RED:
                set_color(0);
                break;
            case BTN_GREEN:
                set_color(1);
                break;
            case BTN_BLUE:
                set_color(2);
                break;
            case RESET:
                reset();
                Serial.println("Resetting...");
                break;
            default:
                break;
        }
        delay(100);
        IrReceiver.resume();
    }
}
