#include         <string.h>
#include         <SoftwareSerial.h>

// ------- Adafruit Thermal to serial port ------- //
#include         <Adafruit_Thermal.h>
SoftwareSerial   printerSerial(5, 6);
Adafruit_Thermal printer(&printerSerial);
#include        "russian_decode.h"
// ------- Adafruit Thermal to serial port ------- //



void PrintSms(String str) {
  printerSerial.begin(9600);
  printer.begin();
  printer.setCodePage(CODEPAGE_CP866);
  printer.justify('L');
  printer.println(str);
  printer.feed(3);
}

