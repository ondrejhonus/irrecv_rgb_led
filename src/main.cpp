#include <Arduino.h>
#include <IRremote.hpp>

#define IR_MODULE_PIN 12 // This is the port your connect your IR reciever OUTPUT
#define BTN_RED 19365 // Get the raw code from your controller and set your own code for the red button
#define BTN_GREEN 19366 // Do the same thing as the red button, but for the green button
#define BTN_BLUE 19364 // -||-
#define RESET 149 // Set your code for a reset button on your controller

int rgbs[] = {6, 5, 3};  // Red D3, Green D5, Blue D6
const int count = 3;
int values[count];

unsigned long codes[] = {
    137,  // Zero
    128,  // One
    129,  // Two
    130,  // Three
    131,  // Four
    132,  // Five
    133,  // Six
    134,  // Seven
    135,  // Eight
    136   // Nine
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
        display_number(recieved_code);
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
        delay(200);
        IrReceiver.resume();
    }
}
