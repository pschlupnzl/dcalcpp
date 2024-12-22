// This is dcalcpp
#define ARDUINO_BUILD

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include "CCalculate.h"

#include "LcdToken.h"

////#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "TScan.h"




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
// #define ST77XX_DARKGRAY 0x4208 // 8-16-8
#define ST77XX_DARKGRAY 0x2104 // 4-8-4

// #define DARK_THEME
#ifdef DARK_THEME
#define DISPLAY_FG ST77XX_LIGHTGRAY
#define DISPLAY_FG2 ST77XX_DARKGRAY
#define DISPLAY_BG ST77XX_BLACK
#else
#define DISPLAY_FG ST77XX_BLACK
#define DISPLAY_FG2 ST77XX_LIGHTGRAY
#define DISPLAY_BG ST77XX_WHITE
#endif


Adafruit_ST7789 lcd = Adafruit_ST7789(LCD_CS, LCD_DC, LCD_RST);

String message = "DysCalculator++ screen /dev/ttyUSB0 115200";

CCalculate ccalc;
IEvalEquationOptions coptions = {
    .trigRad = true
};

void setCursor(int16_t cx, int16_t cy) {
  lcd.setCursor(cx, cy);
}
void setFont(int size) {
  lcd.setFont(
    size == 1 ? &FreeSansBold12pt7b
    : &FreeSans24pt7b
  );
}
void getTextBounds(const char* str, int16_t cx, int16_t cy, int16_t* ptx, int16_t* pty, uint16_t* ptw, uint16_t* pth) {
  lcd.getTextBounds(str, cx, cy, ptx, pty, ptw, pth);
}
void drawRect(int16_t tx, int16_t ty, uint16_t tw, uint16_t th, uint16_t color) {
  lcd.drawRect(tx, ty, tw, th, color);
}
void print(const char* str) {
  lcd.print(str);
}

bool showWorking = true;

/**
* Update the content in the lcd screen.
* @param erase Optional value indicating that the screen should be cleared
* before new content is written. This may cause a visible flicker.
*/
void redraw (bool erase=false) {
  if (erase) {
    lcd.fillScreen(DISPLAY_BG);
  }

  if (showWorking) {
    lcd.setFont(&FreeSans12pt7b);
    lcd.setTextColor(DISPLAY_FG2);
    lcd.setCursor(0, 24);
    lcd.print(String(ccalc.toString().c_str()));
  }

  int16_t cx = 0, cy = 48;
  LcdToken lcdToken(&setCursor, &setFont, &getTextBounds, &drawRect, &print);
  lcdToken.setMetrics(8, -20, 0);

  // lcd.setFont(&FreeSans24pt7b);
  ccalc.forEach([lcd, &lcdToken, &cx, &cy](TScan* scan) {
    eScanType type = scan->type();
    lcd.setTextColor(
      type == SCAN_BINARYOP ? ST77XX_BLUE
      : DISPLAY_FG);

    int16_t out_tw;
    lcdToken.getBounds(scan, cx, cy, &out_tw);
    lcdToken.print(cx, cy);
    cx += out_tw;
  });

  lcd.setCursor(20, 120);
  lcd.setTextColor(DISPLAY_FG);
  setFont(0);
  TToken* result = ccalc.result();
  if (result != nullptr) {
    lcd.print("= ");
    cx = lcd.getCursorX();
    cy = lcd.getCursorY();
    lcdToken.getBounds(result, cx, cy);
    lcdToken.print(cx, cy);
  }
}

// const char* startEquation = "1+2*(3+4";
// const char* startEquation = "1+2*3";
const char* startEquation = 
  "1_2";
  // "12_2_13*4_5_6";

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
    // } else if (ch == 'q') {
    //   // resetFunc();
    //   abort();
    } else if (ch == '`') {
      showWorking = !showWorking;
      redraw(true);
    } else {
      ccalc.scan(ch);
      ccalc.parseEquation();
      ccalc.evalEquation(coptions);
      redraw(true);
    }
  }
}
