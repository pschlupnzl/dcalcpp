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

void redraw(CCalculate &calc, ICalcOptions &options) {
    clear();
    mvprintw(0, 0, "%s", message);

    LcdToken lcdToken(&setCursor, &setFont, &getTextBounds, &drawRect, &print);
    lcdToken.setMetrics(0, -1, +1);
    lcdToken.setOptions(options);
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

const char* const WELCOME_MESSAGE = "DysCalculator - Press 'q' to exit.";

void interactive(CCalculate &calc, ICalcOptions& options) {
    snprintf(message, MESSAGE_LENGTH, "%s %d", WELCOME_MESSAGE, options.fixedDecimals);
    initscr();
    noecho();

    for (;;) {
        calc.parseEquation();
        calc.evalEquation(options);
        redraw(calc, options);

        int ch = getch();

        eCommand cmd = commandFromKeyboard(ch);

        // printw("%s", (char*)&ch);
        if (cmd == eCommand::CMD_QUIT) {
            break;
        } else if (cmd == eCommand::CMD_AC) {
            calc.reset();
        } else if (cmd == eCommand::CMD_DEL) {
            calc.backspace();
        } else if (cmd == eCommand::CMD_TRIGRAD) {
            options.trigRad = !options.trigRad;
        } else if (cmd == eCommand::CMD_DECISEP) {
            options.deciSep = options.deciSep == 0x00 ? ',' : 0x00;
        } else if (cmd == eCommand::CMD_THOUSEP) {
            options.thouSep = options.thouSep == 0x00 ? '\'' : 0x00;
        } else if (cmd == eCommand::CMD_FIXEDDECIMALS) {
            options.fixedDecimals =
                options.fixedDecimals < 1 ? 3 : options.fixedDecimals - 1;
        } else {
            eAction action = actionFromKeyboard(ch);
            if(calc.scan(action)) {
                // Add auto-open bracket for unary operator.
                switch(action) {
                    case eAction::ACTION_POW:
                    case eAction::ACTION_ROOT:
                    case eAction::ACTION_SQRT:
                    case eAction::ACTION_SIN:
                    case eAction::ACTION_COS:
                    case eAction::ACTION_TAN:
                    case eAction::ACTION_ASIN:
                    case eAction::ACTION_ACOS:
                    case eAction::ACTION_ATAN:
                        calc.scan(eAction::ACTION_OPEN_AUTO);
                        break;
                    default: /* nop */ break;
                }
            }
        }
        snprintf(message, MESSAGE_LENGTH, "%s Key=%d", WELCOME_MESSAGE, options.fixedDecimals, ch);
    }
    endwin();
}
