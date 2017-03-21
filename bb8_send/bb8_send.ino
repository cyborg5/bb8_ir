/*
 * Sample code for transmitting signals to Hasbro Star Wars BB8 Droid Toy
 *   https://www.amazon.com/Star-Wars-Force-Awakens-BB-8/dp/B00WHX8Z1O/
 */
#include <IRLibSendBase.h>    // First include the send base
#include <IRLib_P90_bb8.h>    // BB8 specific code

IRsendBB8 mySender;

void setup() {
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  Serial.println(F("Type Direction number:0-9"));
}

void loop() {
  if (Serial.available () > 0) {
    uint8_t i = Serial.parseInt();
    Serial.print("Sending:"); Serial.println(i,DEC);
    switch (i) {  
      case 0: mySender.send(BB8_IDLE); break;
      case 1: mySender.send(BB8_FW); break;
      case 2: mySender.send(BB8_FW_RT); break;
      case 3: mySender.send(BB8_RIGHT); break;
      case 4: mySender.send(BB8_BK_RT); break;
      case 5: mySender.send(BB8_BACK); break;
      case 6: mySender.send(BB8_BK_LT); break;
      case 7: mySender.send(BB8_LEFT); break;
      case 8: mySender.send(BB8_FW_LT); break;
    };
  }
}
