#include         <string.h>
#include         <SoftwareSerial.h>

// ------- Adafruit Thermal to serial port ------- //
#include         <Adafruit_Thermal.h>
SoftwareSerial   printerSerial(5, 6);
Adafruit_Thermal printer(&printerSerial);
#include        "russian_decode.h"
// ------- Adafruit Thermal to serial port ------- //


// ------------- GPRS init variables ------------ //
SoftwareSerial gsm(7, 8); // RX, TX
String Arsp, Grsp, Helper;
// ------------- GPRS init variables ------------ //

void setup() {
  Serial.begin(9600);
  Serial.println("SETUP");
  gsm.begin(9600);
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.println("AT+CNMI=1, 2, 0, 0, 0");
}

void loop() {
  if (gsm.available()) {
    Grsp = gsm.readString();
    
    Serial.println(Grsp);
    
    if(Grsp.substring(1,6) == "+CMT") {
      Serial.println("Hei! I Receive an SMS!");
    } else {
      Serial.println("Substring is");
      Serial.println(Grsp.substring(1,6));
    }

//    PrintSms(Grsp);
  }
}

void PrintSms(String str) {
  printerSerial.begin(9600);
  printer.begin();
  printer.setCodePage(CODEPAGE_CP866);
  printer.justify('L');
  printer.println(str);
  printer.feed(3);
}
