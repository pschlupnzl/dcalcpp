#ifndef ARDUINO_BUILD

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

#include "tests/runTests.h"

void interactive(CCalculate &calc, ICalcOptions &options);

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
        interactive(calc, options);
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
    std::cout << calc.toString(options) << std::endl;


    // std::cout << "C++ version: " << __cplusplus << std::endl;
    return 0;
}

#endif /* ARDUINO_BUILD */