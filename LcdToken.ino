// #include <iostream>
// #include <Adafruit_GFX.h>
// #include <Adafruit_ST7789.h>
// #include <Fonts/FreeSans12pt7b.h>
// #include <Fonts/FreeSans18pt7b.h>
// #include <Fonts/FreeSansBold18pt7b.h>
// #include <Fonts/FreeSans24pt7b.h>
// #include <Fonts/FreeSansBold24pt7b.h>
// #include <Fonts/FreeSans9pt7b.h>
// #include <vector>
// #include <string>
// #include <cstring>
// #include "TScan.h"

// typedef struct {
//   int16_t tx, ty;
//   uint16_t tw, th;
//   char* str;
//   const GFXfont* font;
// } TLcdRect;


// /**
// * Class to handle the rendering of a token on the LCD.
// */
// class LcdToken {
// private:
//   std::vector<TLcdRect> m_rects;

  
// public:
//   // static 
//   /**
//   * Initialize a new instance of the LcdToken class with the given token.
//   */
//   LcdToken(TScan* src, Adafruit_ST7789 &lcd, int16_t cx, int16_t cy, uint16_t &out_tw);
//   /** Releases resources allocated by the token. */
//   ~LcdToken();
//   /** Draw the token. */
//   void print(Adafruit_ST7789 &lcd, int16_t cx, int16_t cy);
// };

// LcdToken::LcdToken(TScan* src, Adafruit_ST7789 &lcd, int16_t cx, int16_t cy, uint16_t &out_tw) {
//   std::string str = src->toString();
//   char * cstr = new char [str.length()+1];
//   std::strcpy (cstr, str.c_str());

//   int16_t tx, ty;
//   uint16_t tw, th;
//   lcd.getTextBounds(cstr, cx, cy, &tx, &ty, &tw, &th);

//   TLcdRect rect = {
//     tx: tx,
//     ty: ty,
//     tw: tw,
//     th: th,
//     str: cstr,
//     font: &FreeSansBold18pt7b
//   };
//   m_rects.push_back(rect);

//   out_tw = tw;
// }

// LcdToken::~LcdToken() {
//   for (TLcdRect& rect : m_rects) {
//     delete[] rect.str;
//   }
// }

// void LcdToken::print(Adafruit_ST7789 &lcd, int16_t cx, int16_t cy) {
//     for (TLcdRect rect : m_rects) {
//       lcd.drawRect(rect.tx, rect.ty, rect.tw, rect.th, ST77XX_GREEN);
//       lcd.setCursor(rect.cx, rect.cy);
//       lcd.print(rect.str);
//     }

// }