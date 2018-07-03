# remote_print

Arguino print text answer from webserver.

## Requirements:

- SIM800L module;
- [Seeeduino GPRS Library](https://github.com/Seeed-Studio/Seeeduino_GPRS) for interactions with SIM800;
- [Thermal printer](http://iarduino.ru/shop/Raznoe/termoprinter.html) 
- [Adafruit_Thermal](http://iarduino.ru/file/317.html) for interactions with printer;

## Pins

SIM800:

```
#define SIM800_TX_PIN           4
#define SIM800_RX_PIN           3
#define SIM800_POWER_PIN        9
#define SIM800_POWER_STATUS     2
```

Printer:

```
SoftwareSerial    printerSerial(7, 6); // RX, TX
Adafruit_Thermal  printer(&printerSerial, 5);
```

## ATTENTION

Change init baudrate in Seeeduino GPRS Library for **minimal** (e.g. 300) value!

## TODO

- [x] Smart delay;
- [ ] Functions to handle **fatal** error;
- [x] Power switch;
- [ ] Error led;
- [x] Soldering;
- [x] Case;
