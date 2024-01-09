#include <Arduino.h>
#include "Lpf2Hub.h"

#include "M5StickCPlus.h"

Lpf2Hub myHub;
int speed = 0;
extern M5StickCPlus M5;
char charbuffer[200];

float getBatteryLevel(void)
{
    auto vbatData = M5.Axp.GetBatVoltage();
    return 100.0f * ((vbatData - 3.0f) / (4.15f - 3.0f));
}

void setup() {
  // put your setup code here, to run once:
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Lcd.setTextSize(3);
    myHub.init();
}

void loop() {
  M5.update();

  if (myHub.isConnecting()) {
    myHub.connectHub();
    if (myHub.isConnected()) {
      Serial.println("We are now connected to the HUB");
    } else {
      Serial.println("We have failed to connect to the HUB");
    }
  }  

  if (M5.BtnA.wasReleased())
  {
    speed += 25;
    if (speed < 0) speed = 0;
    if (speed > 100) speed = 100;
    myHub.setBasicMotorSpeed(static_cast<byte>(DuploTrainHubPort::MOTOR), speed);
    snprintf(charbuffer, sizeof(charbuffer), "speed: %d", speed);
    charbuffer[sizeof(charbuffer) - 1] = '\0';
    M5.Lcd.fillScreen(0);
    M5.Lcd.drawString(charbuffer, 0, 30);
  }
  if (M5.BtnB.wasReleased())
  {
    speed -= 25;
    if (speed > 0) speed = 0;
    if (speed < -100) speed = -100; 
    myHub.setBasicMotorSpeed(static_cast<byte>(DuploTrainHubPort::MOTOR), speed);
    snprintf(charbuffer, sizeof(charbuffer), "speed: %d", speed);
    charbuffer[sizeof(charbuffer) -1] = '\0';
    M5.Lcd.fillScreen(0);
    M5.Lcd.drawString(charbuffer, 0, 30);
  }

  
  snprintf(charbuffer, sizeof(charbuffer), "bat: %.2f", getBatteryLevel());
  charbuffer[sizeof(charbuffer) -1] = '\0';
  M5.Lcd.drawString(charbuffer, 0, 0);

}

