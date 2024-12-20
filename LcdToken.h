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
char buffer[NUMBER_TO_STRING_BUFFER_SIZE];
std::string numberToString(int n) {
    snprintf(buffer, NUMBER_TO_STRING_BUFFER_SIZE, "%i", n);
    return std::string(buffer);
}

class LcdToken {
private:
  setCursor_t* m_setCursor;
  setFont_t* m_setFont;
  getTextBounds_t* m_getTextBounds;
  drawRect_t* m_drawRect;
  print_t* m_print;
  std::vector<LcdRect_t> m_rects;

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

  void getBounds(TScan* scan, int16_t cx, int16_t cy, int16_t* out_tw) {
    uint16_t tw = 0;
    eScanType type = scan->type();
    int whole, num, denom;
    if (type == SCAN_NUMBER 
      && ((TScanNumber*)scan)->toFractionParts(&whole, &num, &denom)) {
      uint16_t tww = addString(numberToString(whole), cx, cy, 0);
      tww += 8;

      uint16_t twn = addString(numberToString(num), cx, cy, 1, tww, -20);
      uint16_t twd = addString(numberToString(denom), cx, cy, 1, tww, 0);
      if (twd > twn) {
        m_rects[1].dx += (twd - twn) >> 1;
        tw += tww + twd;
      } else {
        m_rects.back().dx += (twn - twd) >> 1;
        tw += tww + twn;
      }
    } else {
        tw += addString(scan->toString(), cx, cy, 0);
    }
    // LcdRect_t rect({
    //   dx: 0,
    //   dy: 0,
    //   size: 0,
    //   str: new char[str.length() + 1]
    // });
    // std::strcpy(rect.str, str.c_str());

    // int16_t tx, ty;
    // uint16_t tw, th;
    // m_setFont(rect.size);
    // m_getTextBounds(rect.str, cx, cy, &tx, &ty, &tw, &th);
    // m_rects.push_back(rect);

    *out_tw = tw;
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