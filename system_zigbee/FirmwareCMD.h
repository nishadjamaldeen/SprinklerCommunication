#ifndef FirmwareCMD_h
#define FirmwareCMD_h

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "valveCommand.h"

#define START 0x7E
#define TX_DEFAULT_LEN 5
#define CHECKSUM_DEFAULT 0xFF
#define MSB_LEN 0x00
#define OPTION_DEFAULT 0x00
#define BROADCAST_DEFAULT 0x00
#define ADDR16_MSB 0xFF
#define ADDR16_LSB 0xFE
#define STATUS_SUCCESS 0x00

struct Device{
    String MY;
    String SH;
    String SL;
    String NI;
    String PNA; // parent network address
    String type;
    String status;
    String profile_id;
    String manufacturer_id;
    Device * next;

};

class FirmwareCMD {
    
    public:

    FirmwareCMD(SoftwareSerial * serial, valveCommand * vC);
    bool _init();
    void startAT();
    void exitAT(); 
    String getCMD(String cmd);
    void setCMD(String cmd);
    void discoverNetwork(int numDev);
    int sendData(String addr64, char data[], int datasize);
    Device * getNetworkDevices();
    void receiveData();
    int setValveDelay(String addr64, int del);
    int openValve(String addr64);
    int closeValve(String addr64);
    String networkScanDiscovery();
    String scanNetwork();


    private:

    String devAddr64MSB;
    String devAddr64LSB;
    String devMY;
    SoftwareSerial * xbee;
    valveCommand * vcmd;
    Device * head = new Device();
    void structGenerator(String datastream);
    
    void convertToByteArray(String hex, byte * bytes);
    uint8_t charToHexAddress(char c);
    uint8_t charToHex(char c);

};

#endif
