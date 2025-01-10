#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "calc/CCalculate.h"
#include "calc/TToken.h"
#include "calc/keymap.h"
#include "calc/eAction.h"
#include "calc/types.h"
#include "LcdToken.h"
#include <ncurses.h>

void setCursor(int16_t cx, int16_t cy) {
    move(cy, cx);
}
int currentFontSize = 0;
void setFont(int size) {
    currentFontSize = size;
}
void getTextBounds(const char* str, int16_t cx, int16_t cy, int16_t* tx, int16_t* ty, uint16_t* tw, uint16_t* th) {
    *tx = cx;
    *ty = cy;
    *tw = strlen(str);
    *th = 1;
}
void drawRect(int16_t tx, int16_t ty, uint16_t tw, uint16_t th, uint16_t color) {
    // nop
}
void print(const char* str) {
    printw("%s", str);
}

#define MESSAGE_LENGTH 64
char message[MESSAGE_LENGTH];

void redraw(CCalculate &calc) {
    clear();
    mvprintw(0, 0, "%s", message);

    LcdToken lcdToken(&setCursor, &setFont, &getTextBounds, &drawRect, &print);
    lcdToken.setMetrics(0, -1, +1);
    int16_t cx, cy;
    
    cx = 0;
    cy = 6;
    move(cy, cx);
    ITokenResultBase* result = calc.result();
    if (result == nullptr) {
        printw("Type to enter an equation.");
    } else {
        // printw("= %s", ((TToken*)result)->toString().c_str());
        lcdToken.getBounds(result, cx, cy);
        lcdToken.print(cx, cy);
    }

    cx = 0;
    cy = 2;
    calc.forEach([&cx, &cy, &lcdToken](IScan* scan) {
        int16_t out_tw;
        lcdToken.getBounds(scan, cx, cy, &out_tw);
        lcdToken.print(cx, cy);
        cx += out_tw;
    });

    move(cy, cx);
    refresh();
}

#define CHAR_BACKSPACE 0x08
#define CHAR_DELETE 0x7F
#define CHAR_ESCAPE 0x1B

const char* const WELCOME_MESSAGE = "DysCalculator - Press 'q' to exit.";

void interactive(CCalculate &calc, ICalcOptions& options) {
    snprintf(message, MESSAGE_LENGTH, "%s", WELCOME_MESSAGE);
    initscr();
    noecho();

    for (;;) {
        calc.parseEquation();
        calc.evalEquation(options);
        redraw(calc);

        int ch = getch();

        // printw("%s", (char*)&ch);
        snprintf(message, MESSAGE_LENGTH, "%s Key=%d", WELCOME_MESSAGE, ch);
        if (ch == 'q') {
            break;
        } else if (ch == CHAR_ESCAPE) {
            calc.reset();
        } else if (ch == CHAR_BACKSPACE || ch == CHAR_DELETE) {
            calc.backspace();
        } else {
            eAction action = actionFromKeyboard(ch);
            if(calc.scan(action)) {
                // Add auto-open bracket for unary operator.
                switch(action) {
                    case eAction::ACTION_SQRT:
                    case eAction::ACTION_ROOT:
                        calc.scan(eAction::ACTION_OPEN_AUTO);
                        break;
                }
            }
        }
    }
    endwin();
}
