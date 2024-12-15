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

// TTokenNumber* n = new TTokenNumber();

int main(int argc, char **argv) {
    const char* input = argc > 1 
        ? argv[1] 
        : "(1+2)*3";
    // calc.pushToken(new TTokenValue(1.00));
    // calc.pushToken(new TTokenBinaryOp(BINARY_OP_ACTION_DIV));
    // calc.pushToken(new TTokenValue(0.00));
    // calc.pushToken(new TTokenValue(1.00));
    // calc.pushToken(new TTokenBinaryOp(BINARY_OP_ACTION_ADD));
    // calc.pushToken(new TTokenValue(2.00));
    // calc.pushToken(new TTokenBinaryOp(BINARY_OP_ACTION_MULT));
    // calc.pushToken(new TTokenValue(3.00));

    // std::cout << "Input: " << input << std::endl;
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