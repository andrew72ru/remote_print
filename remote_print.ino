#include        <EEPROM.h>
#include        <gprs.h>
#include        <SoftwareSerial.h>
#include        <Adafruit_Thermal.h>
#include        "russian_decode.h"
#include        "http_header.h"

#define HTTP_CMD "GET /api/summary/list HTTP/1.0\r\n\r\n"
#define HTTP_SRV "pricestat.p72t.ru"

GPRS gprs;
char buffer[512];
char savedHash[32];

SoftwareSerial    printerSerial(5, 6);
Adafruit_Thermal  printer(&printerSerial, 4);

void setup() {
  Serial.begin(9600);
  printerSerial.begin(9600);
}

/**
 * HERE is the printer up, check and print
 */
void readAndPrint() {
  printer.begin();
  printer.setDefault();
  printer.setCodePage(CODEPAGE_CP866);

  String answer;
  while (1) {
    answer = gprs.serialSIM800.readStringUntil('\n');
    if(answer.indexOf("CLOSED") != -1) break;
    int l = answer.length() + 1;
    char b[l];
    answer.toCharArray(b, l);
    RUS(b);
    printer.println(b);
    
//    Serial.println(b);
  }
  printer.feed(3);
}

void loop() {
  Serial.println("start");
  gprsJoin();

  if (0 == gprsConnectTCP() && 0 == gprs.sendTCPData(HTTP_CMD)) {
    while (!gprs.serialSIM800.available()); // wait for response

    int     i = 0;
    long    contentLength = 0;
    String  checkHash;
    String  prevHash;
    while (1) {
      String answer = gprs.serialSIM800.readStringUntil('\n');
      answer.trim();
      if (answer.length() == 0 && i > 1) {
        readAndPrint();
        break;
      } else {
        if (answer.startsWith("Check-hash")) {
          checkHash = getStringFromHeader(answer);
          if(checkHash == "no-hash") break;
          
          prevHash = readEEPROM(0, 32);
          Serial.println("checkHash is " + checkHash);
          Serial.println("prevHash is  " + prevHash);
          if(checkHash.compareTo(prevHash) == 0) {
            Serial.println("Content already printed");
            break;
          } else {
            writeEEPROM(checkHash, 0, 32);
          }
        }
        if(answer.startsWith("Is-empty")) {
          String isEmpty = getStringFromHeader(answer);
          if(isEmpty == "empty") break;
        }
      }
      if (answer.indexOf("CLOSED") != -1) break;
      if(answer.length() > 0) i++; // Iterate ONLY if string is not empty
    }
  } else {
    Serial.println("sendTCPData fail");
  }

  Serial.println("complete");
//  delay(60000);
}

String readEEPROM(int start, int end) {
  String result;
  for(int i = start; i <= end; i++) {
    char c = EEPROM.read(i);
    result.concat(c);
  }

  return result;
}

void writeEEPROM(String value, int start, int end) {
  int length = (end - start) + 1;
  char val[length];
  value.toCharArray(val, length);
  int arIndex = 0;
  for(int i = start; i <= end; i++) {
    EEPROM.write(i, val[arIndex]);
    arIndex++;
  }
}

int gprsConnectTCP() {
  int reTries = 0;
  while (0 != gprs.connectTCP(HTTP_SRV, 8008)) {
    Serial.println("TCP connect error");
    delay(2000);
    reTries ++;

    if (reTries > 10) return -1;
  }
  return 0;
}

void gprsJoin() {
  gprs.serialSIM800.listen();
  gprs.preInit();
  while (0 != gprs.init()) {
    Serial.println("init error");
    delay(1000);
  }
  while (0 != gprs.checkSIMStatus()) {
    Serial.println("SIM status fail");
    delay(1000);
  }

  while (0 != gprs.networkCheck()) {
    Serial.println("Network status fail");
    delay(1000);
  }

  while (!gprs.join("internet.beeline.ru", "beeline", "beeline")) {
    Serial.println("gprs join network error");
    delay(2000);
  }
  // successful DHCP
  Serial.print("IP Address is ");
  Serial.println(gprs.getIPAddress());
}

void closeAll() {
  gprs.closeTCP();
  gprs.shutTCP();
}

