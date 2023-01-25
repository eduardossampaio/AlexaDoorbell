/*
 * 
 *
*/

#include <RCSwitch.h>
#define LED_PIN 2
#define TRANSMIT_PIN 1
RCSwitch mySwitch = RCSwitch();

void setup() {
  pinMode(LED_PIN, OUTPUT);
  mySwitch.enableTransmit(TRANSMIT_PIN);
  mySwitch.setRepeatTransmit(15);
}

void loop() {
  digitalWrite(LED_PIN,HIGH);
  mySwitch.send(6577456, 24);
  delay(1000); 
  digitalWrite(LED_PIN,LOW);
  delay(1000);
}
