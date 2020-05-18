#ifndef FirmwareCMD_h
#define FirmwareCMD_h

#include "Arduino.h"
#include <SoftwareSerial.h>

class LoraController{
    public: 
        LoraController(int ss, int rst, int dio0);
        void _init();
        bool sendMessage(String msg);
        // TODO: change the return of the receiver to string eventually to get the output - currently just writes to serial
        bool receiveMessage(); 
    private:
        SoftwareSerial * lora;
        int _rst;
        int _ss;
        int _dio0;


};

#endif
