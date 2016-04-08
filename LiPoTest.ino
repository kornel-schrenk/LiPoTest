#include "MAX17043.h"
#include <SPI.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

MAX17043 batteryMonitor;

// Declare which fonts we will be using
extern uint8_t BigFont[];                  //16x16 pixel
extern uint8_t SevenSegNumFont[];

//Define the Arduino pin numbers for the TFT screen
#define TFT_CS     10
#define TFT_RST    9
#define TFT_DC     8
#define TFT_SCLK   13
#define TFT_MOSI   11

// Library initializations
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {

  Serial.begin(9600);
  Serial.println("");
  Serial.println("LiPoTest - START");
  Serial.println("");

  Wire.begin();         //Start the I2C interface

  // Initialize the TFT screen
  tft.initR(INITR_BLACKTAB);

  batteryMonitor.reset();
  batteryMonitor.quickStart();
  delay(1000);

  Serial.print("Version:\t\t");
  Serial.println(batteryMonitor.getVersion());  

  batteryMonitor.setAlertThreshold(20);
  Serial.print("Alert:\t\t\t");
  Serial.println(batteryMonitor.getAlertThreshold()); 
  Serial.println(""); 
  
  displayScreen();
  displayLiPoData();
}

void loop() {
  delay(2000);
  displayLiPoData();
}

void displayScreen()
{
  //Clear the screen
  tft.fillScreen(ST7735_BLACK);

  // Clear the screen and set text size
  tft.setTextWrap(false);
  tft.setTextSize(1);

  // Display the header of the menu - the header is the first item
  tft.setCursor(40, 5);
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft.println("LiPo Test");

  // Draw the screen border
  tft.drawRect(0, 0, 128, 160, ST7735_WHITE);

  // Draw separation line
  tft.drawLine(0, 16, tft.width() - 1, 16, ST7735_WHITE);
}

void displayLiPoData()
{
  float cellVoltage = batteryMonitor.getVCell();
  Serial.print("Voltage:\t\t");
  Serial.print(cellVoltage, 4);
  Serial.println("V");

  float stateOfCharge = batteryMonitor.getSoC();
  Serial.print("State of charge:\t");
  Serial.print(stateOfCharge);
  Serial.println("%");

  // Draw numerical data 
  tft.setTextSize(2);

  tft.setTextColor(ST7735_CYAN, ST7735_BLACK);
  tft.setCursor(25, 30);
  tft.print(cellVoltage);

  tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
  tft.setCursor(25, 60);
  tft.print(stateOfCharge);

  tft.setTextColor(ST7735_GREEN, ST7735_BLACK);
  tft.setCursor(25, 90);
  tft.print(batteryMonitor.getVersion());

  tft.setTextColor(ST7735_RED, ST7735_BLACK);
  tft.setCursor(25, 120);
  tft.print(batteryMonitor.getAlertThreshold());

  // Draw unit data
  tft.setTextSize(1);

  tft.setTextColor(ST7735_CYAN, ST7735_BLACK);
  tft.setCursor(80, 37);
  tft.print(" volt");

  tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
  tft.setCursor(100, 67);
  tft.print(" %");

  tft.setTextColor(ST7735_GREEN, ST7735_BLACK);
  tft.setCursor(50, 97);
  tft.print(" version");

  tft.setTextColor(ST7735_RED, ST7735_BLACK);
  tft.setCursor(50, 127);
  tft.print(" alert %");

}

