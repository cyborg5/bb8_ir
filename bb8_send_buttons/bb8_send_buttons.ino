/*
 * Sample code for transmitting signals to Hasbro Star Wars BB8 Droid Toy
 *   https://www.amazon.com/Star-Wars-Force-Awakens-BB-8/dp/B00WHX8Z1O/
 */
#include <IRLibSendBase.h>    // First include the send base
#include <IRLib_P90_bb8.h>    // BB8 specific code

IRsendBB8 mySender;

//pin numbers for buttons
#define LEFT_BUTTON    A0
#define FORWARD_BUTTON A1
#define RIGHT_BUTTON   A2

void setup() {
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(FORWARD_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
}
void handlePress (uint8_t pin,uint16_t data) {
  mySender.send(BB8_IDLE);
  while (!digitalRead(pin)) {
    mySender.send(data);
  }
  mySender.send(BB8_IDLE);
  mySender.send(BB8_IDLE);
  mySender.send(BB8_IDLE);
  mySender.send(BB8_IDLE);
}
void loop() {
  if (!digitalRead(LEFT_BUTTON)){
    handlePress(LEFT_BUTTON,BB8_LEFT);
  }
  if (!digitalRead(FORWARD_BUTTON)){
    handlePress(FORWARD_BUTTON,BB8_FW);
  }
  if (!digitalRead(RIGHT_BUTTON)){
    handlePress(RIGHT_BUTTON,BB8_RIGHT);
  }
  delay (100);
}
