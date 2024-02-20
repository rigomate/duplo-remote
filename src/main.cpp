#include <Arduino.h>
#include "Lpf2Hub.h"
#include <sstream>
#include <cstring>
#include <HardwareSerial.h>

Lpf2Hub myHubDuplo;
Lpf2Hub myHubLego;

std::string DuploAddress("6c:b2:fd:6a:c8:6e");
std::string LegoAddress("90:84:2b:d0:9a:6d");


struct Command{
  std::string command;
  int value;
};

//Define two Serial devices mapped to the two internal UARTs
HardwareSerial MySerial0(0);

const size_t buflen{100};
char bufferstream[buflen];
size_t bufpos{0};

void readSerial(Stream &stream)
{
  if (stream.readBytes(&bufferstream[bufpos], 1))
  {
    stream.print(bufferstream[bufpos]);
    if (bufferstream[bufpos] == '\r')
    {
      stream.print('\n');
    }
    bufpos++;
  }

  if (bufpos > buflen)
  {
    bufferstream[buflen - 1] = '\r';
  }
}

/*return commands in the format: "$M1 100\r"
First argument is command second argument i value
*/
bool getCommand(Command &command)
{
  bool retval {false};
  bool containsEnd = (std::strchr(bufferstream, '\r') != nullptr);

  if (containsEnd)
  {
    if (bufferstream[0] == '$')
    {
      std::string input(bufferstream);
      std::string strcommand, strvalue;
      std::istringstream iss(input);
      iss >> strcommand >> strvalue;

      int intValue = 0;
      try {
          // Convert the modified string to an integer
          intValue = std::stoi(strvalue);
          retval = true;
          command.command = strcommand;
          command.value = intValue;

          // Output the result
      } catch (const std::invalid_argument& e) {
          std::string errorMessage = e.what();
          Serial.printf("Invalid argument: %s\n", errorMessage.c_str());
      } catch (const std::out_of_range& e) {
        std::string errorMessage = e.what();
        Serial.printf("Out of range: %s\n", errorMessage.c_str());
      }

      
      memset(bufferstream, 0, buflen);
      bufpos = 0;
    }
    memset(bufferstream, 0, buflen);
    bufpos = 0;
  }
  
  return retval;
}



void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("Hello World");
    
    // Configure MySerial0 on pins TX=D6 and RX=D7 (-1, -1 means use the default)
    MySerial0.begin(9600, SERIAL_8N1, -1, -1);
    MySerial0.print("MySerial0");
    Serial.setTimeout(100);
    //myHubDuplo.init(DuploAddress, 100);
    //myHubLego.init(LegoAddress, 100);
}

bool written = false;
bool written2 = false;
char buffer[100];

unsigned long previousMillis = 0;  // will store last time an alarm was triggered
const long interval = 2000;       // interval for the alarm in milliseconds (5 seconds)

int connectCounter = 0;
void loop() {
  readSerial(MySerial0);

  unsigned long currentMillis = millis();  // get the current time

  // Check if the specified interval has passed since the last alarm
  if (currentMillis - previousMillis >= interval) {
        if (connectCounter == 0)
        {
          if (!myHubDuplo.isConnected())
          {
            Serial.println("Initing duplo");
            myHubDuplo.init(DuploAddress, 1);
          }
          connectCounter++;
        }
        else if(connectCounter == 1)
        {
          if (!myHubLego.isConnected())
          {
            Serial.println("Initing lego");
            myHubLego.init(LegoAddress, 1);
          }
          connectCounter = 0;
        }

      // Update the last triggered time
      previousMillis = currentMillis;
  }



  if (myHubDuplo.isConnecting()) {
    if (!written)
    {
      Serial.println("Duplo train connecting");
      written = true;
    }
    myHubDuplo.connectHub();
    if (myHubDuplo.isConnected()) {
      Serial.println("We are now connected to the Duplo train");
      std::string address = myHubDuplo.getHubAddress();
      Serial.println(address.c_str());
    } else {
      Serial.println("We have failed to connect to the Duplo train");
    }
  }
  if (myHubLego.isConnecting()) {
    if (!written2)
    {
      Serial.println("Lego train connecting");
      written2 = true;
    }
    myHubLego.connectHub();
    if (myHubLego.isConnected()) {      
      Serial.println("We are now connected to the Lego train");
      std::string address = myHubLego.getHubAddress();
      Serial.println(address.c_str());
    } else {
      Serial.println("We have failed to connect to the Lego train");
    }
  }

  Command command;
  if (getCommand(command))
  {
    if (myHubLego.isConnected())
    {
      if (command.command == "$P0")
      {
        myHubLego.setBasicMotorSpeed(static_cast<byte>(PoweredUpHubPort::A), command.value);
      }
      if (command.command == "$P1")
      {
        myHubLego.setBasicMotorSpeed(static_cast<byte>(PoweredUpHubPort::B), command.value);
      }
    }
    if (myHubDuplo.isConnected())
    {
      if (command.command == "$P2")
      {
        myHubDuplo.setBasicMotorSpeed(static_cast<byte>(DuploTrainHubPort::MOTOR), command.value);
      }
    }
  }

}

