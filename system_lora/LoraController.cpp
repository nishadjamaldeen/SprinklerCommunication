#include "LoraController.h"

#include "Arduino.h"
#include "stdio.h"
#include <SPI.h>
#include <LoRa.h>

LoraController::LoraController(int ss, int rst, int dio0){
    _ss = ss;
    _rst = rst;
    _dio0 = dio0;
//    _init();
}

void LoraController::_init(){
//    Serial.begin(115200);
    while (!Serial);
    LoRa.setPins(_ss, _rst, _dio0);

    while (!LoRa.begin(915E6)){
        Serial.print(".");
        delay(500);
    }

    LoRa.setSyncWord(0xF3);

}

bool LoraController::sendMessage(String msg){
    // Serial.print("Sending version: ");
    LoRa.beginPacket();
    LoRa.print(msg);
    LoRa.endPacket();
}

bool LoraController::receiveMessage(){

    int sz = LoRa.parsePacket();
    if(sz){
        Serial.print("received '");

        while (LoRa.available()){
            String inputData = LoRa.readString();
            Serial.print(inputData);
        }

        Serial.print("' with RSSI ");
        Serial.println(LoRa.packetRssi());
    }
}
