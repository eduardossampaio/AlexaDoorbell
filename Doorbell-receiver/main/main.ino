/*
 * Example
 *
 * If you encounter any issues:
 * - check the readme.md at https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md
 * - ensure all dependent libraries are installed
 * - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#arduinoide
 * - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#dependencies
 * - open serial monitor and check whats happening
 * - check full user documentation at https://sinricpro.github.io/esp8266-esp32-sdk
 * - visit https://github.com/sinricpro/esp8266-esp32-sdk/issues and check for existing issues or open a new one
 */

// Custom devices requires SinricPro ESP8266/ESP32 SDK 2.9.6 or later

// Uncomment the following line to enable serial debug output
// #define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
#define DEBUG_ESP_PORT Serial
#define NODEBUG_WEBSOCKETS
#define NDEBUG
#endif

#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

#include <SinricPro.h>
#include "HomeBroker.h"

#define APP_KEY "<APP_KEY>"
#define APP_SECRET "<APP_SECRET>"
#define DEVICE_ID "<DEVICE_ID>"

#define SSID "<SSID>"
#define PASS "<PASSWORD>"

#define CONNECTED_LED_PIN 2
#define BAUD_RATE 115200

HomeBroker &homeBroker = SinricPro[DEVICE_ID];
RCSwitch mySwitch = RCSwitch();
bool isConnectedToSinric = false;
/*************
 * Variables *
 ***********************************************
 * Global variables to store the device states *
 ***********************************************/

/*************
 * Callbacks *
 *************/

/**********
 * Events *
 *************************************************
 * Examples how to update the server status when *
 * you physically interact with your device or a *
 * sensor reading changes.                       *
 *************************************************/

// Doorbell
void ringDoorbell()
{
  static unsigned long lastBtnPress;
  unsigned long actualMillis = millis();
  if (actualMillis - lastBtnPress > 500)
  {
    homeBroker.sendDoorbellEvent();
    lastBtnPress = actualMillis;
  }
}

// PushNotificationController
// void sendPushNotification(String notification) {
//  homeBroker.sendPushNotification(notification);
//}

/*********
 * Setup *
 *********/

void setupSinricPro()
{

  SinricPro.onConnected([]
                        { 
    Serial.printf("[SinricPro]: Connected\r\n"); 
    isConnectedToSinric = true; });
  SinricPro.onDisconnected([]
                           { Serial.printf("[SinricPro]: Disconnected\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
};

void setupWiFi()
{
  WiFi.begin(SSID, PASS);
  Serial.printf("[WiFi]: Connecting to %s", SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.printf(".");
    delay(250);
  }
  Serial.printf("connected\r\n");
}

void setup()
{
  Serial.begin(BAUD_RATE);
  pinMode(LED_BUILTIN, OUTPUT);
  setupWiFi();
  setupSinricPro();
  mySwitch.enableReceive(0);
}

/********
 * Loop *
 ********/
void handleRfSignals()
{

  if (mySwitch.available())
  {
    int received = mySwitch.getReceivedValue();

    Serial.printf("received :%d\r\n", received);
    if (received == 6577456)
    {
      received = 0;
      Serial.printf("ding dong\r\n");
      ringDoorbell();
    }

    mySwitch.resetAvailable();
  }
}
void loop()
{
  SinricPro.handle();
  handleRfSignals();
  if (isConnectedToSinric)
  {
    digitalWrite(CONNECTED_LED_PIN, HIGH);
  }
  Serial.flush();
}
