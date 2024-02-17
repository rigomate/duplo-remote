#include <Arduino.h>
#include "Lpf2Hub.h"

Lpf2Hub myHub;
Lpf2Hub myHubLego;
byte motorPort = (byte)DuploTrainHubPort::MOTOR;
byte legoport = (byte)PoweredUpHubPort::A;

std::string DuploAddress("6c:b2:fd:6a:c8:6e");
std::string LegoAddress("90:84:2b:d0:9a:6d");



void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("Hello World");
    Serial.setTimeout(100);
    myHub.init(DuploAddress);
    myHubLego.init(LegoAddress);
}

bool written = false;
bool written2 = false;
char buffer[100];
void loop() {
  if (myHub.isConnecting()) {
    if (!written)
    {
      Serial.println("duplo connecting");
      written = true;
    }
    myHub.connectHub();
    if (myHub.isConnected()) {
      Serial.println("We are now connected to the HUB");
      std::string address = myHub.getHubAddress();
      Serial.println(address.c_str());
    } else {
      Serial.println("We have failed to connect to the HUB");
    }
  }
  if (myHubLego.isConnecting()) {
    if (!written)
    {
      Serial.println("duplo connecting");
      written = true;
    }
    myHubLego.connectHub();
    if (myHubLego.isConnected()) {
      Serial.println("We are now connected to the HUB");
      std::string address = myHubLego.getHubAddress();
      Serial.println(address.c_str());
    } else {
      Serial.println("We have failed to connect to the HUB");
    }
  }

  if (myHub.isConnected())
  {
    if (Serial.readBytes(buffer, 1))
    {
      Serial.print(buffer[0]);
      std::string command{buffer};

      if (command == "w")
      {
        myHub.setBasicMotorSpeed(motorPort, 50);
        Serial.println("speed 50");
      }
      else if (command == "s")
      {
        myHub.setBasicMotorSpeed(motorPort, -50);
        Serial.println("speed -50");
      }
      else if (command == "e")
      {
        myHub.stopBasicMotor(motorPort);
        Serial.println("stop");
      }
    }
  }

  if (myHubLego.isConnected())
  {
    if (Serial.readBytes(buffer, 1))
    {
      Serial.print(buffer[0]);
      std::string command{buffer};

      if (command == "t")
      {
        myHubLego.setBasicMotorSpeed(legoport, 50);
        Serial.println("speed 50");
      }
      else if (command == "g")
      {
        myHubLego.setBasicMotorSpeed(legoport, -50);
        Serial.println("speed -50");
      }
      else if (command == "z")
      {
        myHubLego.stopBasicMotor(legoport);
        Serial.println("stop");
      }
    }
  }

}

