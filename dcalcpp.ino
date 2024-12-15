// This is dcalcpp
#define ARDUINO_BUILD

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include "CCalculate.h"

// Define pins of ESP32 connected to LCD.
#define LCD_MOSI 23 // SDA Pin on ESP32 D23
#define LCD_SCLK 18 // SCL Pin on ESP32 D18
#define LCD_CS   15 // Chip Select control pin on ESP32 D15
#define LCD_DC    2 // Data Command control pin on ESP32 D2
#define LCD_RST   4 // Reset pin (could connect to RST pin) ESP32 D4
#define LCD_BLK  32 // Black Light Pin on ESP32 D32

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 170

Adafruit_ST7789 lcd = Adafruit_ST7789(LCD_CS, LCD_DC, LCD_RST);

String message = "DysCalculator++ screen /dev/ttyUSB0 115200";

CCalculate ccalc;
IEvalEquationOptions coptions = {
    .trigRad = true
};

void redraw (bool erase=false) {
  if (erase) {
    lcd.fillScreen(ST77XX_WHITE);
  }
  lcd.setFont(&FreeSans12pt7b);
  lcd.setTextColor(ST77XX_BLACK);
  lcd.setCursor(0, 24);
  // lcd.print(message);
  lcd.print(String(ccalc.toString().c_str()));
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting demo");
  // put your setup code here, to run once:
  lcd.init(SCREEN_HEIGHT, SCREEN_WIDTH);
  lcd.setRotation(3);
  redraw(true);
}

void loop() {
  while (Serial.available()) {
    char ch = Serial.read();
    Serial.printf("0x%x ", ch);
    redraw(true);
    if (ch == 0x1B) {
      ccalc.reset();
      redraw(true);
    // } else if (ch == 0x08 || ch == 0x7F) {
    //   int n = message.length();
    //   if (n > 0) {
    //     message = message.substring(0, n - 1);
    //   }
    //   redraw(true);
    } else {
      ccalc.scan(ch);
      ccalc.parseEquation();
      // ccalc.evalEquation(coptions);
      redraw(true);
    }
  }
}
