/*
 * Sample code for decoding signals to Hasbro Star Wars BB8 Droid Toy
 *   https://www.amazon.com/Star-Wars-Force-Awakens-BB-8/dp/B00WHX8Z1O/
 */
#include <IRLibDecodeBase.h> // First include the decode base
#include <IRLib_P90_bb8.h>   // BB8 specific code

IRdecodeBB8 myDecoder;

// Include a receiver either this or IRLibRecvPCI or IRLibRecvLoop
#include <IRLibRecv.h> 
IRrecv myReceiver(2);  //pin number for the receiver
uint16_t myBuffer[RECV_BUF_LENGTH];

void setup() {
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  myReceiver.enableAutoResume(myBuffer);
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive BB8 toy IR signals"));
}

void loop() {
  if (myReceiver.getResults()) { 
    if (myDecoder.decode()) {
      Serial.print("0x");Serial.print(myDecoder.value,HEX); Serial.print(":");
      //myDecoder.dumpResults(false);//change to true for verbose output
      if(myDecoder.forward) Serial.print("Forward "); 
      if(myDecoder.right)   Serial.print("Right "); 
      if(myDecoder.back)    Serial.print("Back "); 
      if(myDecoder.left)    Serial.print("Left "); 
      if(myDecoder.idle)    Serial.print("Idle ");
      Serial.println();
    }
    myReceiver.enableIRIn();      //Restart receiver
  }
}

