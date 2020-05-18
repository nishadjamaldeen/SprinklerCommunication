#include <SoftwareSerial.h>

#include "FirmwareCMD.h"
#include "valveCommand.h"
#include "Arduino.h"
#include "stdio.h"

#include "cmdMessenger.h"
#include "enum.h"
// Build cmdMessenger object
CmdMessenger cmdMessenger = CmdMessenger(Serial);
String outputStr;

byte packet[] = {0x7E, 0x00, 0x15, 0x10, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x41, 0x84, 0xE9, 0xBD, 0xFF, 0xFE, 0x00, 0x00, 0x74, 0x65, 0x73, 0x74, 0x69, 0x6E, 0x67, 0xD3};
byte pkt[] = {0x7E, 0x00, 0x10, 0x10, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x41, 0x84, 0xE9, 0xBD, 0xFF, 0xFE, 0x00, 0x00, 0x31, 0x30, 0x70};

SoftwareSerial xbee(2, 3); // (RX, TX)
valveCommand vC(4,5); // 1A, 2A --> + side of solenoid into pin 1Y, - side into Pin 2Y
FirmwareCMD fc(&xbee, &vC);

String _MSB = "0013A200";
String RO11 = "4184EF73";  
String RO10 = "41851FDE";
String ROU9 = "418516DF"; 
String ROU8 = "41851687"; 
String ROU6 = "41851686";
String ROU5 = "418517B1";
String ROU4 = "4184EED4";
String ROU3 = "41851821";
String ROU2 = "4185166D";
String ROU1 = "4184E9BD";
String CORD = "418521E9";

void setup(){

  Serial.begin(9600);
  fc._init();
  
  // L27 - 31 for actuating devices
  // vC.init();
  // vC.setDelay(1000);

//Serial.println(netscan);

  // L34 - for non-actuating coordinator
  // String network = fc.scanNetwork();
  // Serial.println(network);
  //  fc.openValve(ROU4);

  // Data does not get sent if we use cmdMessenger
  outputStr.reserve(128);
  cmdMessenger.printLfCr();
  attachCommandCallbacks();
}


void loop(){

  if (xbee.available() > 0){
    fc.receiveData();
  }
//    fc.openValve(ROU3); // segfault when I use put this here and also have cmdMessenger below 
   cmdMessenger.feedinSerialData();


}

void attachCommandCallbacks() {
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(ACTUATE, onActuate);       // S,sucesss;
//  cmdMessenger.attach(PULSE, onPulse);          // S,sucesss;
//  cmdMessenger.attach(OPEN, onOpenValve);       // S,sucesss;
//  cmdMessenger.attach(CLOSE, onCloseValve);      // S,success;
//  cmdMessenger.attach(SCAN, onScan);          // S,success;
}


// =============================================================================================
// Format : outputStr = "e, unknown command;"
void OnUnknownCommand() {
  outputStr.remove(0);
  outputStr.concat(F("e, unknown command;"));
  Serial.println(outputStr);
}

void onActuate() {
//  String address = cmdMessenger.readStringArg();
//  int open_time = cmdMessenger.readInt16Arg();
  outputStr.remove(0);
  outputStr.concat(F("actuating device;"));
  Serial.println(outputStr);
  // set delay first 
//  fc.openValve(address);
  fc.openValve(ROU3); 
}
