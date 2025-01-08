#ifndef LCDTOKEN_H
#define LCDTOKEN_H

#include <string>
#include <cstring>
#include <vector>

#define MAX(a,b) ((a)>=(b) ? (a) : (b))

typedef void setCursor_t(int16_t cx, int16_t cy);
typedef void setFont_t(int size);
typedef void getTextBounds_t(const char* str, int16_t cx, int16_t cy, int16_t* tx, int16_t* ty, uint16_t* tw, uint16_t* th);
typedef void drawRect_t(int16_t tx, int16_t ty, uint16_t tw, uint16_t th, uint16_t color);
typedef void print_t(const char* str);

typedef struct {
  int16_t dx;
  int16_t dy;
  int size;
  char* str;
} LcdRect_t;

#define NUMBER_TO_STRING_BUFFER_SIZE 32

class LcdToken {
private:
  setCursor_t* m_setCursor;
  setFont_t* m_setFont;
  getTextBounds_t* m_getTextBounds;
  drawRect_t* m_drawRect;
  print_t* m_print;

  /** Options for handling calculations and display. */
  ICalcOptions m_options = {
    .trigRad = false,
    .deciSep = 0x00,
    .thouSep = 0x00,
  };

  std::vector<LcdRect_t> m_rects;
  /** Horizontal pad between digits in a number. */
  int16_t m_hpad;
  /** Vertical offset from cursor to reach numerator. */
  int16_t m_vnum;
  /** Vertical offset from cursor to reach denominator. */
  int16_t m_vdenom;

  std::string numberToString(int n) {
    char numberToStringBuffer[NUMBER_TO_STRING_BUFFER_SIZE];
      snprintf(numberToStringBuffer, NUMBER_TO_STRING_BUFFER_SIZE, "%i", n);
      return std::string(numberToStringBuffer);
  }

  uint16_t addString(std::string str, int16_t cx, int16_t cy, int size = 0, int16_t dx = 0, int16_t dy = 0) {
    LcdRect_t rect({
      dx: dx,
      dy: dy,
      size: size,
      str: new char[str.length() + 1]
    });
    std::strcpy(rect.str, str.c_str());

    int16_t tx, ty;
    uint16_t tw, th;
    m_setFont(rect.size);
    m_getTextBounds(rect.str, cx + dx, cy + dy, &tx, &ty, &tw, &th);
    m_rects.push_back(rect);
    return tw;
  }

  /** Render fraction with whole and fractional parts. */
  int16_t addFraction(int whole, int num, int denom, bool negative, int16_t cx, int16_t cy) {
    uint16_t tw = 0;
    uint16_t tww = whole
    ? addString(numberToString(negative ? -whole : whole), cx, cy, 0)
    : negative
    ? addString("-", cx, cy, 0) 
    : 0;
    tww += m_hpad;

    if (num) {
      uint16_t twn = addString(numberToString(num), cx, cy, 1, tww, m_vnum);
      uint16_t twd = !denom ? 0 : addString(numberToString(denom), cx, cy, 1, tww, m_vdenom);
      // Center-align the shorter string.
      if (twd > twn) {
        m_rects[1].dx += (twd - twn) >> 1;
        tw += tww + twd;
      } else {
        m_rects.back().dx += (twn - twd) >> 1;
        tw += tww + twn;
      }
    }
    return tw;
  }

public:
  LcdToken(
    setCursor_t* setCursor,
    setFont_t* setFont,
    getTextBounds_t* getTextBounds,
    drawRect_t* drawRect,
    print_t* print
  ) {
    m_setCursor = setCursor;
    m_setFont = setFont;
    m_getTextBounds = getTextBounds;
    m_drawRect = drawRect;
    m_print = print;
  }

  ~LcdToken() {
    for (LcdRect_t rect : m_rects) {
      delete[] rect.str;
    }
  }

  void setMetrics(
    int16_t hpad,
    int16_t vnum,
    int16_t vdenom
  ) {
    m_hpad = hpad;
    m_vnum = vnum;
    m_vdenom = vdenom;
  }

  void getBounds(IScan* scan, int16_t cx, int16_t cy, int16_t* out_tw) {
    int whole, num, denom;
    bool negative;
    m_rects.clear();
    if (
      scan->type() == SCAN_NUMBER 
      && ((TScanNumber*)scan)->toFractionParts(&whole, &num, &denom, &negative)
    ) {
      *out_tw = addFraction(whole, num, denom, negative, cx, cy);
    } else {
      *out_tw = addString(scan->toString(m_options), cx, cy, 0);
    }
  }

  void getBounds(IToken* token, int16_t cx, int16_t cy) {
    int whole, num, denom;
    bool negative;
    m_rects.clear();
    if (
      token->type() == TOKEN_FRACTION &&
      ((TTokenFraction*)token)->toFractionParts(&whole, &num, &denom, &negative)
    ) {
      addFraction(whole, num, denom, negative, cy, cy);
    } else {
      addString(token->toString(m_options), cx, cy);
    }
  }

  void print(int16_t cx, int16_t cy) {
    for (LcdRect_t rect : m_rects) {
      m_setFont(rect.size);
      m_setCursor(cx + rect.dx, cy + rect.dy);
      m_print(rect.str);
    }
  }
};

#endif /* LCDTOKEN_H */