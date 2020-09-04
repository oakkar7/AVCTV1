/*************************************************** 
 V1. Automatic Contactless Thermometer
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
  
 ****************************************************/
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <MD_MAX72xx.h>

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES  3

#define CLK_PIN   18  // or SCK
#define DATA_PIN  23  // or MOSI
#define CS_PIN    15  // or SS

// SPI hardware interface
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Text parameters
#define CHAR_SPACING  1 // pixels between characters
#define   MAX_INTENSITY   0x4

// The amount of time (in milliseconds) between tests
#define  DELAYTIME  2000  // in milliseconds

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

const int adcPin = 34;
int adcValue = 0;
int numCounter = 0;
float objT = 0;

// Global message buffers shared by Serial and Scrolling functions
#define BUF_SIZE  7
char message1[5] = "";
char message[BUF_SIZE] = "Hello!";
bool newMessageAvailable = true;

void printText(uint8_t modStart, uint8_t modEnd, char *pMsg)
// Print the text string to the LED matrix modules specified.
// Message area is padded with blank columns after printing.
{
  uint8_t   state = 0;
  uint8_t   curLen;
  uint16_t  showLen;
  uint8_t   cBuf[8];
  int16_t   col = ((modEnd + 1) * COL_SIZE) - 1;

  mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

  do     // finite state machine to print the characters in the space available
  {
    switch(state)
    {
      case 0: // Load the next character from the font table
        // if we reached end of message, reset the message pointer
        if (*pMsg == '\0')
        {
          showLen = col - (modEnd * COL_SIZE);  // padding characters
          state = 2;
          break;
        }

        // retrieve the next character form the font file
        showLen = mx.getChar(*pMsg++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
        curLen = 0;
        state++;
        // !! deliberately fall through to next state to start displaying

      case 1: // display the next part of the character
        mx.setColumn(col--, cBuf[curLen++]);

        // done with font character, now display the space between chars
        if (curLen == showLen)
        {
          showLen = CHAR_SPACING;
          state = 2;
        }
        break;

      case 2: // initialize state for displaying empty columns
        curLen = 0;
        state++;
        // fall through

      case 3:  // display inter-character spacing or end of message padding (blank columns)
        mx.setColumn(col--, 0);
        curLen++;
        if (curLen == showLen)
          state = 0;
        break;

      default:
        col = -1;   // this definitely ends the do loop
    }
  } while (col >= (modStart * COL_SIZE));

  mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}

void setup() {

  Serial.begin(9600);

  Serial.println("Automatic Non-Contact Thermometer V1"); 
  Serial.println("Closed Target near sensor (2-5cm) to measure.");  
  mlx.begin();  
  
  mx.begin();
  mx.clear();
  //scrollText("MD_MAX72xx Test  ");
}

void loop() {
  adcValue = analogRead(adcPin);
  Serial.println(adcValue);
  if (adcValue < 4000) 
  {  
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");
  objT = mlx.readObjectTempC();
  //Serial.println(objT);
  // int part
  int objT_int = int(objT);
  Serial.println(objT_int);
  int objT_dec = (float(objT-objT_int)*10);
  Serial.println(objT_dec);

  dtostrf(objT,4, 1, message1);
  message1[4] = '°';
  printText(0, MAX_DEVICES-1, message1);
  Serial.println();
  }

  //printText(0, MAX_DEVICES-1, message);
  delay(DELAYTIME);
  mx.clear();
  //delay(1000);
}
