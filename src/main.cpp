#include <Arduino.h>
#include "Lpf2Hub.h"

Lpf2Hub myHub;



void setup() {
  // put your setup code here, to run once:
    myHub.init();
}

void loop() {

  if (myHub.isConnecting()) {
    myHub.connectHub();
    if (myHub.isConnected()) {
      Serial.println("We are now connected to the HUB");
    } else {
      Serial.println("We have failed to connect to the HUB");
    }
  }  

}

