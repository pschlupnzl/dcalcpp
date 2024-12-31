#ifndef ARDUINO_BUILD

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "CCalculate.h"
#include "TToken.h"

#include <ncurses.h>
#include "LcdToken.h"

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

IEvalEquationOptions options = {
    .trigRad = true
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
    TToken* result = calc.result();
    if (result == nullptr) {
        printw("Type to enter an equation.");
    } else {
        // printw("= %s", ((TToken*)result)->toString().c_str());
        lcdToken.getBounds(result, cx, cy);
        lcdToken.print(cx, cy);
    }

    cx = 0;
    cy = 2;
    calc.forEach([&cx, &cy, &lcdToken](TScan* scan) {
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

void interactive() {
    initscr();
    noecho();

    for (;;) {
        calc.parseEquation();
        calc.evalEquation(options);
        redraw();

        int ch = getch();
        // printw("%s", (char*)&ch);
        snprintf(message, MESSAGE_LENGTH, "Key %d", ch);
        if (ch == 'q') {
            break;
        } else if (ch == CHAR_ESCAPE) {
            calc.reset();
        } else if (ch == CHAR_BACKSPACE || ch == CHAR_DELETE) {
            calc.backspace();
        } else {
            calc.scan(ch);
        }
    }
    endwin();
}

int main(int argc, char **argv) {
    snprintf(message, MESSAGE_LENGTH, "%s", "DysCalculator - Press 'q' to exit.");

    const char* input = argc > 1 
        ? argv[1] :
        // "";
        // "(1+2)*3";
        "1+2*3";
        // "11";
        // "11+22*33";
        // "1_2_4 + 2_5_124";
        // "1_2+2_3";

    calc.reset();
    for (const char* pch = input; *pch; ++pch) {
        calc.scan(*pch);
    }

    interactive();

    calc.parseEquation();
    calc.evalEquation(options);
    std::cout << calc.toString() << std::endl;
}

#endif /* ARDUINO_BUILD */