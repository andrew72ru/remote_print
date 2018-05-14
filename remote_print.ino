#include        <gprs.h>
#include        <SoftwareSerial.h>
#include        <Adafruit_Thermal.h>
#include        "russian_decode.h"

#define HTTP_CMD "GET /api/summary/list HTTP/1.0\r\n\r\n"

// api.dinner.zhdanovskih.name/api/summary/list?day=04.05.2018

GPRS              gprs;
char              buffer[512];

SoftwareSerial    printerSerial(5, 6);
Adafruit_Thermal  printer(&printerSerial, 4);

void setup() {
  Serial.begin(9600);
  printerSerial.begin(9600);

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
      answer.trim();
      if (answer.length() == 0 && i > 1) {
        readAndPrint();
        break;
      } else {
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
 * Я понял, в чем дело. Принтер берет данные в собственный буфер, и делает ПАУЗЫ, пока не напечатает то, что в буфере
 * А порт продолжает читаться в это время. Поэтому в принтере получается вот такое.
 * Экспериментально можно подтвердить, установив delay в любом месте цикла этой функции и посмотрев в вывод дебага.
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

