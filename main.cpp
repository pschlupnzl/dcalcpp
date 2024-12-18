#ifndef ARDUINO_BUILD

#include <iostream>
#include <vector>
#include "CCalculate.h"
#include "TToken.h"

/*
* TO BUILD AND RUN
* 
* g++ main.cpp TToken.cpp CCalculate.cpp parseEquation.cpp evalEquation.cpp TTokenBinaryOp.cpp && ./a.out
* 
*/

CCalculate calc;

IEvalEquationOptions options = {
    .trigRad = true
};

int main(int argc, char **argv) {
    const char* input = argc > 1 
        ? argv[1] 
        // : "(1+2)*3";
        : "1+2*3";
        // : "11";
        // : "11+22*33";

    for (int k = 1; k > 0; k--) {
        calc.reset();

        for (const char* pch = input; *pch; ++pch) {
            std::cout << "[" << *pch << "] ";
            calc.scan(*pch);
        }
        calc.parseEquation();
        calc.evalEquation(options);
        std::cout << std::endl;


        std::cout << calc.toString() << std::endl;

        // calc.forEach([](TScan* scan) {
        //     std::cout << "scan: " << scan->toString() << std::endl;
        // });

        int16_t cx = 0, cy = 20;

        calc.forEach([&cx, &cy](TScan* scan) {
            eScanType type = scan->type();
            const char* str = scan->toString().c_str();
            int16_t tx, ty;
            std::cout << cx << " " << str << std::endl;
            // uint16_t tw, th;
            // lcd.getTextBounds(str, cx, cy, &tx, &ty, &tw, &th);
            // lcd.drawRect(tx, ty, tw, th, DISPLAY_FG2);
            // lcd.setCursor(cx, cy);
            // lcd.print(str);
            cx += 1;

        });

    }
    return 0;
}

#endif /* ARDUINO_BUILD */