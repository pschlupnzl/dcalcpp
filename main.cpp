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
        : "(1+2)*3";

    calc.reset();

    for (const char* pch = input; *pch; ++pch) {
        std::cout << "[" << *pch << "] ";
        calc.scan(*pch);
    }
    std::cout << std::endl;

    calc.parseEquation();
    calc.evalEquation(options);

    std::cout << calc.toString() << std::endl;

    return 0;
}

#endif /* ARDUINO_BUILD */