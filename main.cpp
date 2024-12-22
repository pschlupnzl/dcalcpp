#ifndef ARDUINO_BUILD

#include <iostream>
#include <vector>
#include <string.h>
#include "CCalculate.h"
#include "TToken.h"

// #include <ncurses.h>
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

#ifndef int16_t
#define int16_t short int
#endif /* int16_t */

#ifndef uint16_t
#define uint16_t unsigned short int
#endif /* uint16_t */

// void setCursor(int16_t cx, int16_t cy) {
//     move(cy, cx);
// }
// int currentFontSize = 0;
// void setFont(int size) {
//     currentFontSize = size;
// }
// void getTextBounds(const char* str, int16_t cx, int16_t cy, int16_t* tx, int16_t* ty, uint16_t* tw, uint16_t* th) {
//     *tx = cx;
//     *ty = cy;
//     *tw = strlen(str);
//     *th = 1;
// }
// void drawRect(int16_t tx, int16_t ty, uint16_t tw, uint16_t th, uint16_t color) {
//     // nop
// }
// void print(const char* str) {
//     printw("%s", str);
// }

// void redraw() {
//     clear();
//     mvprintw(0, 0, "DysCalculator - Press 'q' to exit.");

//     LcdToken lcdToken(&setCursor, &setFont, &getTextBounds, &drawRect, &print);
//     lcdToken.setMetrics(0, -1, +1);
//     int16_t cx, cy;
    
//     cx = 0;
//     cy = 6;
//     move(cy, cx);
//     TToken* result = calc.result();
//     if (result == nullptr) {
//         printw("Type to enter an equation.");
//     } else {
//         // printw("= %s", ((TToken*)result)->toString().c_str());
//         lcdToken.getBounds(result, cx, cy);
//         lcdToken.print(cx, cy);
//     }

//     cx = 0;
//     cy = 2;
//     calc.forEach([&cx, &cy, &lcdToken](TScan* scan) {
//         int16_t out_tw;
//         lcdToken.getBounds(scan, cx, cy, &out_tw);
//         lcdToken.print(cx, cy);
//         cx += out_tw;
//     });

//     move(cy, cx);
//     refresh();
// }

// void interactive() {
//     // initscr();
//     // noecho();

//     // for (;;) {
//     //     calc.parseEquation();
//     //     calc.evalEquation(options);
//     //     redraw();

//     //     int ch = getch();
//     //     // printw("%s", (char*)&ch);
//     //     if (ch == 'q') {
//     //         break;
//     //     } else if (ch == 0x1b) {
//     //         calc.reset();
//     //     } else {
//     //         calc.scan(ch);
//     //     }
//     // }
//     // endwin();
// }

int main(int argc, char **argv) {
    const char* input = argc > 1 
        ? argv[1] :
        // "";
        // "(1+2)*3";
        // "1+2*3";
        // "11";
        // "11+22*33";
        // "1_2_4 + 2_5_124";
        "1_2+2_3";

    calc.reset();
    for (const char* pch = input; *pch; ++pch) {
        calc.scan(*pch);
    }

    // interactive();

    calc.parseEquation();
    calc.evalEquation(options);
    std::cout << calc.toString() << std::endl;

        // int16_t cx = 0, cy = 20;
        // calc.forEach([&cx, &cy](TScan* scan) {
        //     eScanType type = scan->type();
        //     const char* str = scan->toString().c_str();
        //     int16_t tx, ty;
        //     std::cout << cx << " " << str << std::endl;
        //     // uint16_t tw, th;
        //     // lcd.getTextBounds(str, cx, cy, &tx, &ty, &tw, &th);
        //     // lcd.drawRect(tx, ty, tw, th, DISPLAY_FG2);
        //     // lcd.setCursor(cx, cy);
        //     // lcd.print(str);
        //     cx += strlen(str);

        // });

}

#endif /* ARDUINO_BUILD */