Buy an ESP8266 for a dollar.  
Firmware: [https://github.com/SpacehuhnTech/esp8266_deauther/releases](https://github.com/SpacehuhnTech/esp8266_deauther/releases) or [[https://github.com/SpacehuhnTech/esp8266_deauther/releases](https://github.com/Jahamars/esp/archive/refs/heads/main.zip)]([https://github.com/SpacehuhnTech/esp8266_deauther/releases](https://github.com/Jahamars/esp/archive/refs/heads/main.zip))
Install `esptool` via repos or `pip`.

Find the ESP:
```bash
ls /dev/ttyUSB*
```

Flash command:
```bash
sudo esptool.py --port /dev/ttyUSB0 write_flash 0x00000 ~/esp/esp8266_deauther.bin
```

Power the device and visit `192.168.4.1`.
