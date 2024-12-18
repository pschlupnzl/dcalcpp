// This is dcalcpp
#define ARDUINO_BUILD

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
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

// See https://rgbcolorpicker.com/565
#define ST77XX_GRAY 0x7BEF
#define ST77XX_LIGHTGRAY 0xC618

#define DISPLAY_FG ST77XX_BLACK
#define DISPLAY_FG2 ST77XX_LIGHTGRAY
#define DISPLAY_BG ST77XX_WHITE

Adafruit_ST7789 lcd = Adafruit_ST7789(LCD_CS, LCD_DC, LCD_RST);

String message = "DysCalculator++ screen /dev/ttyUSB0 115200";

CCalculate ccalc;
IEvalEquationOptions coptions = {
    .trigRad = true
};

/**
* Update the content in the lcd screen.
* @param erase Optional value indicating that the screen should be cleared
* before new content is written. This may cause a visible flicker.
*/
void redraw (bool erase=false) {
  if (erase) {
    lcd.fillScreen(DISPLAY_BG);
  }

  lcd.setFont(&FreeSans12pt7b);
  lcd.setTextColor(DISPLAY_FG2);
  lcd.setCursor(-30, 24);
  // lcd.print(message);
  lcd.print(String(ccalc.toString().c_str()));


  int16_t cx = 0, cy = 36;

  lcd.setFont(&FreeSans24pt7b);
  ccalc.forEach([lcd, &cx, &cy](TScan* scan) {
    eScanType type = scan->type();
    lcd.setTextColor(
      type == SCAN_BINARYOP ? ST77XX_BLUE
      : DISPLAY_FG);
    const char* str = scan->toString().c_str();
    int16_t tx, ty;
    uint16_t tw, th;
    lcd.getTextBounds(str, cx, cy, &tx, &ty, &tw, &th);
    lcd.drawRect(tx, ty, tw, th, ST77XX_RED);
    lcd.setCursor(cx, cy);
    lcd.print(str);
    cx += tw + 8;
    // cy += 12;
  });
}

// const char* startEquation = "1+2*(3+4";
const char* startEquation = "1+2*3";

void setup() {
  Serial.begin(115200);
  Serial.println("Starting demo");
  // put your setup code here, to run once:
  lcd.init(SCREEN_HEIGHT, SCREEN_WIDTH);
  lcd.setRotation(3);

  // Initial equation.
  for (const char* pch = startEquation; *pch; ++pch) {
      ccalc.scan(*pch);
  }
  ccalc.parseEquation();
  ccalc.evalEquation(coptions);

  redraw(true);
}

void loop() {
  while (Serial.available()) {
    char ch = Serial.read();
    Serial.printf("0x%x ", ch);
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
      ccalc.evalEquation(coptions);
      redraw(true);
    }
  }
}
