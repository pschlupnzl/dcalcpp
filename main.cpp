#ifndef ARDUINO_BUILD

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "CCalculate.h"
#include "TToken.h"
#include "keymap.h"
#include "eAction.h"
#include "types.h"

#include <ncurses.h>
#include "LcdToken.h"

#include "tests/runTests.h"

/*
* To build and run the Docker container, which has Ubuntu and full g++ compiler:
*
* docker build -t dcalcpp .
* docker run --rm -it -v .:/app dcalcpp bash
*
* We use the ncurses library to allow some controlled graphical display.
* Need to install the ncurses library
*
* sudo pt install libncurses5-dev
*
* TO BUILD AND RUN
* 
* g++ *.cpp -lncurses && ./a.out
* 
*/

CCalculate calc;

ICalcOptions options = {
    .trigRad = true,
    .deciSep = 0x00,
    .thouSep = 0x00
};

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

void redraw() {
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

void interactive() {
    snprintf(message, MESSAGE_LENGTH, "%s", WELCOME_MESSAGE);
    initscr();
    noecho();

    for (;;) {
        calc.parseEquation();
        calc.evalEquation(options);
        redraw();

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

int main(int argc, char **argv) {
    const char* input = nullptr;

    bool isTests = false;
    bool isInteractive = false;
    for (int argn=1; argn < argc; argn++) {
        if (strcmp(argv[argn], "-i") == 0) {
            isInteractive = true;
        } else if (strcmp(argv[argn], "-t") == 0) {
            isTests = true;
        } else {
            input = argv[argn];
        }
    }

    if (isTests) {
        runTests();
    }

    calc.reset();
    if (input != nullptr) {
        for (const char* pch = input; *pch; ++pch) {
            calc.scan(actionFromKeyboard(*pch));
        }
    }

    if (isInteractive) {
        interactive();
    } else if (input == nullptr) {
        input =
            // "(1+2)*3";
            // "1+2*3";
            // "11";
            // "11+22*33";
            // "1_2_4 + 2_5_124";
            // "1_2+2_3";
            // "1+2*34567.34";
            // "V(4)";
            "4^3";
        calc.reset();
        for (const char* pch = input; *pch; ++pch) {
            calc.scan(actionFromKeyboard(*pch));
        }
    }

    calc.parseEquation();
    calc.evalEquation(options);
    std::cout << calc.toString() << std::endl;


    // std::cout << "C++ version: " << __cplusplus << std::endl;
    return 0;
}

#endif /* ARDUINO_BUILD */