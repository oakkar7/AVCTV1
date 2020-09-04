# AVCTV1
Automatic Non-Contact Thermometer V1.2

 - ESP32 Dev Board
 - MLX90614 non contact IR sensor
 - TCRT5000 IR reflected sensor
 - MAX7219  Dotmatrix display driver x 2 nos
 
Connection:
MLX90614 -
VIN -> ESP32 3.3v
GND -> ESP32 GND
SCL -> ESP32 22
SDA -> ESP32 21

TCRT5000 - ESp32 34/ADC

MAX7219 -
Vcc - 5V
GND - GND
DOUT - ESP32
CS - ESP32
CLK - ESP32
 
 Used:
Adafruit_MLX90614 Lib
  https://github.com/adafruit/Adafruit-MLX90614-Library
  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
MAX72XX Display Lib 
  https://github.com/MajicDesigns/MD_MAX72XX

  By:
  oakkar7, oakkar7@gmail.com
  okelectronic.wordpress.com
  MYANMAR
