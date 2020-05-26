#include "FirmwareCMD.h"

#include "Arduino.h"
#include "stdio.h"
#include <SoftwareSerial.h>
// #include "XBee.h"
#include "valveCommand.h"


FirmwareCMD::FirmwareCMD(SoftwareSerial * serial, valveCommand * vC) {
    vcmd = vC;
    xbee = serial;
    xbee -> begin(9600);

}

bool FirmwareCMD::_init(){
    startAT();

    devAddr64MSB = getCMD("ATSH");
    devAddr64LSB = getCMD("ATSL");
    devMY = getCMD("ATMY");
    setCMD("ATAP 1");
    // getCMD("ATAP");
    exitAT();
    Serial.println(devAddr64MSB + devAddr64LSB);

    return true;
}

void FirmwareCMD::startAT(){
  char inbuffer[3];
  delay(500);
  Serial.println("+++");
  xbee -> print("+++");

  while (xbee -> available() == 0){
    Serial.println("Waiting to enter AT mode...");
    delay(500);
  }
  int i = 0;
  while (xbee -> available() > 0){
    inbuffer[i] = char(xbee -> read());
    i++;
  }
  Serial.println(inbuffer);
}

void FirmwareCMD::exitAT(){

    // char inbuffer[3];

    // Serial.println("ATCN");
    // xbee -> print("ATCN");
    // while (xbee -> available() == 0){
    //     Serial.println("Waiting to exit AT mode ...");
    //     delay(500);
    // }

    // int i = 0;
    // while (xbee -> available() > 0){
    //     inbuffer[i] = char(xbee -> read());
    //     i++;
    // }

    // Serial.println(inbuffer);
    delay(500);
    getCMD("ATCN");
}

String FirmwareCMD::getCMD(String cmd){
  String str = "";
  Serial.println(cmd);
  delay(500);
  xbee -> print(cmd);
  xbee -> write('\r');

  int i = 0;
  while (xbee -> available() == 0){
    Serial.println("Waiting for command response...");
    delay(500);
  }
  
  xbee -> setTimeout(1500);
  
   while (xbee -> available() > 0){
     str += char(xbee -> read());
   }
   
  return str;

}

void FirmwareCMD::setCMD(String cmd){

    Serial.println(cmd);
    delay(500);
    xbee -> print(cmd);
    xbee -> write('\r');
    delay(500);
    xbee -> print("ATWR");
    xbee -> write('\r');


    while (xbee -> available() == 0){
       Serial.println("Waiting for response...");
        delay(500);
    }

    while (xbee -> available() > 0){
        Serial.print(char(xbee -> read()));
    }

    Serial.println();
  
}

int FirmwareCMD::sendData(String addr64, char data[], int datasize){
  
    byte apiCMD = 0x10;
    byte frameID = 0x01;

    uint8_t len = 1 + 1 + 8 + 2 + 1 + 1 + datasize; // apiCMD, frameID, add64 len, add16 len, option, BC radius, data length

    byte addr64bytes[addr64.length()/2];
    // Serial.println(addr64);
    convertToByteArray(addr64, addr64bytes); 
    
    uint8_t checksum = apiCMD + frameID + ADDR16_LSB + ADDR16_MSB + OPTION_DEFAULT + BROADCAST_DEFAULT;

    byte packet[len + 4];
    packet[0] = START;
    packet[1] = MSB_LEN;
    packet[2] = len;
    packet[3] = apiCMD;
    packet[4] = frameID;

    for (int i = 0; i < addr64.length()/2; i++){
        checksum += addr64bytes[i];
        packet[5 + i] = addr64bytes[i];
    }
    packet[13] = ADDR16_MSB;
    packet[14] = ADDR16_LSB;
    packet[15] = OPTION_DEFAULT;
    packet[16] = BROADCAST_DEFAULT;

    for (int j = 0; j < datasize; j++){
        checksum += charToHex(data[j]);
        packet[17 + j] = charToHex(data[j]);
    }
    checksum = CHECKSUM_DEFAULT - checksum;

    packet[len + 3] = checksum;

//    for (byte b: packet){
//      Serial.println(b);
//    }


     xbee -> write(packet, sizeof(packet)); 

     // delay the script to allow the response to be sent - i.e. response is not created immediately 
     delay(1000);

     // Handle API Response
     if (xbee -> available() > 0){
//        Serial.println("RECEIVED API INCOMING TRANSMISSION");

        int startPos;
        int statusPos;
        int addr16Pos;
        int incomingLen;

         xbee -> setTimeout(2000);
         int bufferLen = 255;
         byte buff[bufferLen];
         
         xbee -> readBytes(buff, bufferLen);

        for (int i = 0; i < bufferLen; ++i){
            if (buff[i] == START){
                startPos = i;
                incomingLen = buff[i + 2];
                addr16Pos = i+5;
                statusPos = i+8;
                break;
            }
        }

        if (buff[statusPos] == STATUS_SUCCESS){
//            Serial.println("Successful!");
            return 1;
        } else {
            return 0;
        }

     }

}

void FirmwareCMD::receiveData(){
//    Serial.println("BEGIN INCOMING TRANSMISSION");
    int bufferLen = 40;
    int startBytePos;
    int msgLen;
    int addr64Pos;
    int receiveOpPos;
    int dataPos;
    int datalen;
    byte buff[bufferLen];
    xbee -> setTimeout(2000);
    xbee -> readBytes(buff, bufferLen);

    for (int i = 0; i < bufferLen; ++i){
        if (buff[i] == START){
            startBytePos = i;
            break;  
        } 
    }
//    Serial.println(startBytePos);
//    Serial.println();

    msgLen = buff[startBytePos + 2];
    addr64Pos = startBytePos + 4;
    receiveOpPos = startBytePos + 14;
    dataPos = startBytePos + 15;
    datalen = msgLen - 12;

    for (byte b: buff){
//        Serial.println(b);
    }
//    Serial.println(char(buff[dataPos]));
//    Serial.println(char(buff[dataPos + 1]));

    // for (int i = 0; i < 8; i++){
    //     Serial.println(buff[addr64Pos + i]);
    // }

    // for (int i = 0; i < datalen; i++){
    //     Serial.print(char(buff[dataPos + i]));
    // }

    // 10 - open valve; 12 TTTT - set delay for TTTT millis; 11 - close valve
    if (msgLen > 2 && char(buff[dataPos]) == '1' && char(buff[dataPos + 1]) == '2'){
//        Serial.println("Set delay and open valve!");
        int del =  1000*(int(char(buff[dataPos + 3])) - 48) + 100*(int(char(buff[dataPos + 4])) - 48) + 10*(int(char(buff[dataPos + 5])) - 48) + int(char(buff[dataPos + 3])) - 48;
        vcmd -> setDelay(del);
    }
    else if (char(buff[dataPos]) == '1' && char(buff[dataPos + 1]) == '0'){
//        Serial.println("Open valve!");
        vcmd -> openValve();
        vcmd -> closeValve();
    }

    else if (char(buff[dataPos]) == '1' && char(buff[dataPos + 1]) == '1'){
        vcmd -> closeValve();
    }

//    Serial.println();
//    Serial.println("END INCOMING TRANSMISSION");



}

void FirmwareCMD::convertToByteArray(String hex, byte * bytes){
    int len = hex.length();
    for (int i = 0; i < len; i+=2){
        uint8_t msb = charToHexAddress(hex.charAt(i));
        uint8_t lsb = charToHexAddress(hex.charAt(i+1));
        bytes[i/2] = 16*msb + lsb;
    }
}

uint8_t FirmwareCMD::charToHexAddress(char c){
    byte b = byte(c);
    if (b >= 48 && b < 58){
        return b - 48;
    } else if ( b >= 65 && b < 71){
        return b - 55;
    } else if (b >= 97 && b < 103){
        return b - 87;
    }

}

uint8_t FirmwareCMD::charToHex(char c){
    return byte(c);
}

int FirmwareCMD::setValveDelay(String addr64, int del){

    char str[4];
    itoa(del, str, 10);
    char data[6];
    data[0] = 1;
    data[1] = 2;

    for (int i = 2; i < 6; i++){
        data[i] = str[i-2];
    }

    return sendData(addr64, data, 6);

}

int FirmwareCMD::openValve(String addr64LSB){
    char data[] = {'1', '0'};
    String addr64MSB = "0013A200";
    return sendData(addr64MSB + addr64LSB, data, 2);

}

int FirmwareCMD::closeValve(String addr64LSB){
    char data[] = {'1', '1'};
    String addr64MSB = "0013A200";
    return sendData(addr64MSB + addr64LSB, data, 2);
}

void FirmwareCMD::structGenerator(String datastream){
    Device * curr = head;
    int start = 0;
    int len = datastream.length();
    int ndx = len - 1;
    Serial.println("Start generator");
    Serial.println(len);
    
    while (ndx > 0){
        ndx = datastream.indexOf("  ") == -1 ? len : datastream.indexOf("  ");
        String device = datastream.substring(start, ndx);
        // Serial.println(device);


        Device * d = new Device();
        curr -> next = d;
        curr = curr -> next;

        String MY = device.substring(0, 3);
        String SH = device.substring(5, 13);
        String SL = device.substring(14, 22);
        String NI = device.substring(23, 27);
        String PNA = device.substring(28, 32); // parent network address
        String type = device.substring(33, 35);
        String status = device.substring(36, 38);
        String profile_id = device.substring(39, 43);
        String manufacturer_id = device.substring(44, 48);
        
        curr -> MY = MY;
        curr -> SH = SH;
        curr -> SL = SL;
        curr -> NI = NI;
        curr -> PNA = PNA;
        curr -> type = type;
        curr -> status = status;
        curr -> profile_id = profile_id;
        curr -> manufacturer_id = manufacturer_id;

        
        datastream = datastream.substring(ndx+2, len);
        len = datastream.length();
        ndx = len - 1;
    }

    curr -> next = nullptr;
    Serial.println("Done!");
}

Device * FirmwareCMD::getNetworkDevices(){
    return head;
}


// The following 3 functions need to be reimplemented
String FirmwareCMD::scanNetwork(){
    discoverNetwork(3);
    Device * curr = getNetworkDevices();
    curr = curr -> next;
    String output = "";

    while (curr != nullptr){
        output += curr -> SL;
        output += " ";
    }
    Serial.println(output.length());
    Serial.println("Done!");

    return output;
}

void FirmwareCMD::discoverNetwork(int numDev){
    
    const String delimiter = " ";
    String deviceID = ""; 

    delay(1000);
    startAT();

    delay(500);
    xbee -> print ("ATNT F1");
    xbee -> write('\r');
    delay(500);
    xbee -> print("ATWR");
    xbee -> write('\r');
 
    delay(1000);
    startAT(); 
    
    for (int i = 0; i < numDev; i++){

        String str = "";  
        delay(1000);
        Serial.println("ATND");
        xbee -> print("ATND");
        delay(50);
        xbee -> write('\r'); 
        delay(500);
        xbee -> setTimeout(2500);

        while (xbee -> available() == 0){
            Serial.println("Searching for devices in network...");
            delay(1000);
        }

        while(xbee -> available() > 0){
            char r = char(xbee-> read());
            if (r == '\r')
            {
                str+=" ";
            } else
            {
                str += r;

            }
            delay(50);
        }


        int start = 14;
        int jump = 59;
        int total = str.length()/jump;
    
        for (int i = start; i < str.length(); i = i + jump){
            String addrSL = str.substring(i, i+7);
            Serial.println(deviceID.indexOf(addrSL));
            if (deviceID.indexOf(addrSL) > 0){
                Serial.println(addrSL);
                deviceID += addrSL;
            }
        }
        delay(2000); //adding delay to allow XBee to switch commands

        
        Serial.println(str);
        
        // structGenerator(str);
    }

    Serial.println(deviceID);
    exitAT();
}

String FirmwareCMD::networkScanDiscovery(){
    String output = "";
    const String delimiter = " ";
    Serial.println("ATND");
    // delay(1000);
    // xbee -> setTimeout(5000);
    for (int i = 0; i < 6; ++i){
        
        startAT();
        xbee -> print ("ATNT 8C");
        xbee -> write('\r');
        xbee -> print("ATWR");
        xbee -> write('\r');

        
        xbee -> print("ATND");
        xbee -> write('\r'); 
        Serial.println();

        while (xbee -> available() == 0){
            Serial.println("Searching for devices in network...");
            delay(1000);
        }

        String inputStream;
        while (xbee -> available() > 0){
            inputStream += char(xbee -> read());
            delay(150);
        }
        Serial.println(inputStream);


    }

    return output;
}
