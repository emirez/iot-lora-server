#include <Arduino.h>
#include <RadioHead.h>
#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <hexdump.h>
#include <action.h>
#include <device.h>

#define LED_PORT 4
#define POTI_PORT 0

#define OTHER_ADDRESS 1
#define MY_ADDRESS 2

const bool serialdebug = false;

void lora_listen_for_action();

RH_RF95 driver;
RHReliableDatagram manager(driver, MY_ADDRESS);
TUT__action action;
TUT__device device;

void setup(){
  Serial.begin(9600);
  pinMode(LED_PORT, OUTPUT);
  pinMode(POTI_PORT, INPUT);


  if (manager.init()) {
          Serial.println("RadioHead Datagram Manager initialized");
  } else {
          Serial.println("ERROR initializing RadioHead Datagram Manager");
  }

}

uint8_t inbuf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t outbuf[RH_RF95_MAX_MESSAGE_LEN];

void loop(){
  lora_listen_for_action();
}

void process(struct TUT__action *action, struct TUT__device *device){
  Serial.println(action->read);
  Serial.println(action->write);

  if(action->read == 1) {
    device->led = digitalRead(LED_PORT);
    device->poti = analogRead(POTI_PORT);
  } else {
    digitalWrite(LED_PORT, action->write);
  }
}

void lora_listen_for_action(){
  TUT__action_init(&action);
  TUT__device_init(&device);

  if(manager.available()){
      uint8_t len = sizeof(inbuf);
      uint8_t from;

      if(manager.recvfromAck(inbuf, &len, &from)){
        if(serialdebug){
          Serial.println("Received!");
          hexDump("hexDump", inbuf, len);
        }
        action.TUT__action_parse(&action, inbuf, len);
        process(&action, &device);
        len = device.TUT__device_serialize(&device, outbuf, sizeof(outbuf));
        if(manager.sendtoWait(outbuf, len, from)){
          if(serialdebug){
            Serial.println("Successfully answered!");
          }
        }
        else {
          if(serialdebug){
          Serial.println("Error during answering");
          }
        }
      }
        else {
          if(serialdebug){
          Serial.println("ERROR during receive.");
          }
        }
    }
}
