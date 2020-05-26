#ifndef valveCommand_h
#define valveCommand_h

#include "Arduino.h"
#include <SoftwareSerial.h>

class valveCommand {
    public:

    valveCommand(int cmdPin1, int cmdPin2);
    void init();
    void setDelay(int delayMillis);
    int getResponse();
    int actuate();
    int openValve();
    int closeValve();
    void stop_command();

    private:
    int cp1;
    int cp2;
    int _delay{1000};
    char _in;
    char  _actuateCommand;
    char _stopCommand;
//    void init();
};

#endif

