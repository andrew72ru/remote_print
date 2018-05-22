# remote_print

Arguino print text answer from webserver.

## Requirements:

- SIM800L module;
- [Seeeduino GPRS Library](https://github.com/Seeed-Studio/Seeeduino_GPRS) for interactions with SIM800;
- [Thermal printer](http://iarduino.ru/shop/Raznoe/termoprinter.html) 
- [Adafruit_Thermal](http://iarduino.ru/file/317.html) for interactions with printer;

## Pins

SIM800:

- RX — 3
- TX — 4
- OT — 2

Printer:

- RX — 7
- TX — 6
- DT — 5

## ATTENTION

Change init baudrate in Seeeduino GPRS Library for **minimal** (e.g. 300) value!

## TODO

- [x] Smart delay;
- [ ] Functions to handle **fatal** error;
- [x] Power switch;
- [ ] Error led;
- [x] Soldering;
- [ ] Case;
