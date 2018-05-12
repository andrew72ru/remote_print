#include        <gprs.h>
#include        <SoftwareSerial.h>
#include        <Adafruit_Thermal.h>
#include        "russian_decode.h"

#define HTTP_CMD "GET /api/summary/list HTTP/1.0\r\n\r\n"

// api.dinner.zhdanovskih.name/api/summary/list?day=04.05.2018

GPRS              gprs;
char              buffer[512];

SoftwareSerial    printerSerial(5, 6);
Adafruit_Thermal  printer(&printerSerial);

void setup() {
  Serial.begin(9600);
  printerSerial.begin(9600);
  printer.begin();
  printer.setCodePage(CODEPAGE_CP866);

  gprsJoin();

//  char http_cmd[] = "GET /api/summary/list HTTP/1.0\r\n\r\n";

  if (0 == gprsConnectTCP() && 0 == gprs.sendTCPData(HTTP_CMD)) {
    Serial.println("Command sent");
    while (!gprs.serialSIM800.available()); // wait for response

    int i = 0;
    long contentLength = 0;
    String checkHash;
    while (1) {
      String answer = gprs.serialSIM800.readStringUntil('\n');
      if (answer.length() == 1 && i > 1) {
        readAndPrint();
      } else {
        answer.trim();
        if (answer.startsWith("Check-hash")) {
          checkHash = getStringFromHeader(answer);
          Serial.print("Check-hash is ");
          Serial.println(checkHash);
        }
        if(answer.startsWith("Is-empty")) {
          Serial.print("Is-empty is ");
          Serial.println(getStringFromHeader(answer));
        }
      }

      if (answer.indexOf("CLOSED") != -1) break;
      i++;
    }

  } else {
    Serial.println("sendTCPData fail");
  }

  Serial.println("");
  Serial.println("Exit");
}

/**
 * HERE is the printer up, check and print
 */
void readAndPrint() {
  while (1) {
    int i = 0;
    char r;
    String check;
    while (gprs.serialSIM800.available()) {
      r = gprs.serialSIM800.read();
      Serial.write(r);
      if (r == '\n' || r == '\r') break;
    }
    if (r == '\n' || r == '\r') break;
  }
}

void loop() {
}

long getIntegerValueFromHeader(String header) {
  String valuePart = getStringFromHeader(header);
  long result = valuePart.toInt();

  return result;
}

String getStringFromHeader(String header) {
  int colon = header.indexOf(':');
  String valuePart = header.substring(colon + 2); // "+2" for ": "
  valuePart.trim();

  return valuePart;
}

int gprsConnectTCP() {
  int reTries = 0;
  while (0 != gprs.connectTCP("pricestat.p72t.ru", 8008)) {
    Serial.println("TCP connect error");
    delay(2000);
    reTries ++;

    if (reTries > 10) return -1;
  }
  return 0;
}

void gprsJoin() {
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

