#include "valveCommand.h"

#include "Arduino.h"
#include "stdio.h"
#include <SoftwareSerial.h>

#define PRINT false

// SoftwareSerial XBee(2, 3);

valveCommand::valveCommand(int cmdPin1, int cmdPin2) : cp1(cmdPin1), cp2(cmdPin2) {
    if (PRINT == true) {
        printf("Operation failed with code %d", _in);
    }
    cp1 = cmdPin1;
    cp2 = cmdPin2;
}

void valveCommand::init(){
  if (PRINT == true) {
    printf("Operation failed with code %d", _in);
    }
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(cp1, OUTPUT);
    pinMode(cp2, OUTPUT);

    _actuateCommand = '1';
    _stopCommand = '0';
}

void valveCommand::setDelay(int delayMillis){
    _delay = delayMillis;
}

int valveCommand::getResponse(){
//    int status = -1;
//    if(XBee -> available() > 0){
//        _in = XBee -> read();
//        status = int(_in);
//        if(_in == '1'){
//            if (PRINT == true) {
//                printf("Success");
//            }
//        } else {
//            if (PRINT == true) {
//                printf("Operation failed with code %d", _in);
//            }
//        }
//    } 
//    return status;
return 0;
}

int valveCommand::openValve(){
    digitalWrite(cp1, HIGH);
    digitalWrite(cp2, LOW);
    delay(_delay);
}

int valveCommand::closeValve(){
    digitalWrite(cp2, HIGH);
    digitalWrite(cp1, LOW);
    delay(_delay);
}





