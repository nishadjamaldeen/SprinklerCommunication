#include "Arduino.h"
#include "stdio.h"
#include "LoraController.h"
#include <LoRa.h>
#include <SPI.h>

#include "cmdMessenger.h"
#include "enum.h"

#define SS 10
#define RST 8
#define DIO0 2

LoraController lc(SS, RST, DIO0);
CmdMessenger cmdMessenger(Serial);
volatile bool messageAvailable = false;

void setup(){
    Serial.begin(9600);
    Serial.println("Start");
    lc._init();
    Serial.println("Started!");
    pinMode(LED_BUILTIN, OUTPUT);

    LoRa.onReceive(onReceive);

}

void loop(){
    // ISR Interrupt triggers this 
    if (messageAvailable){
        lc.receiveMessage();
        messageAvailable = false;
    }

    // // Continuously loop to determine if the CMD messenger has new information
    cmdMessenger.feedinSerialData();
}

void onReceive(int packetSize){
    if (packetSize == 0) return;
    messageAvailable = true;
}
