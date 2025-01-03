#include <iostream>
#include <cstring>
#include <tuple>
#include "../CCalculate.h"
#include "../TToken.h"
#include "../eAction.h"

#define evalCase_t std::tuple<const char*, const char*, const char*>

evalCase_t numberCases[] = {
    { "1", "1", "1" },
    { "1 2", "12", "12" },
    { "1 2 3 . 4", "123.4", "123.4" },
    { "1 2 3 . 4 5", "123.45", "123.45" },
    { "1 2 3 . 4 5 9", "123.459", "123.459" },
    { "1 2 3 . 4 5 6", "123.456", "123.456" },
    { ". 1", "0.1", "0.1" },
    { "1 + 2", "1 + 2", "3" },
    { "1 - 2", "1 - 2", "-1" },
    { "1 + 2 * 3", "1 + 2 * 3", "7" },
    { "1 ±", "-1", "-1" },
    { "± 1", "-1", "-1" }, // neg can precede a number, too
    { "2 . 5", "2.5", "2.5" },
    { "1 ± ±", "1", "1" },
    { "2 ± / 4 ±", "-2 / -4", "0.5" },
    { "1 + ± 2", "1 + -2", "-1" },
    { "1 . 2 ± 3", "-1.23", "-1.23" },
    { "1 + 2", "1 + 2", "3" },
    { "1 + 2 . 3 / 4", "1 + 2.3 / 4", "1.575" },
    { "( 1", "( 1", "1" },
    { "( 1 )", "( 1 )", "1" },
    { "1 + 2 * 3", "1 + 2 * 3", "7" },
    { "( 1 + 2 ) * 3", "( 1 + 2 ) * 3", "9" },
    { "9 * 2 + 3", "9 * 2 + 3", "21" },
    { "( 9 * 2 ) + 3", "( 9 * 2 ) + 3", "21" },
    { "9 * ( 2 + 3 )", "9 * ( 2 + 3 )", "45" },
    { "9 * ( 2 + 3", "9 * ( 2 + 3", "45" }, // auto-close bracket
    { "1 2 * 3 4", "12 * 34", "408" },
    { "1 + ( 2 - 3 ± )", "1 + ( 2 - -3 )", "6" },
    { 
      "1 + ( 3 - 5 ) / ( 1 3 - 8 )",
      "1 + ( 3 - 5 ) / ( 13 - 8 )",
      "0.6",
     },
    { 
      "1 + ( 2 - 3 ± ) / ( 4 ± - 5 )",
      "1 + ( 2 - -3 ) / ( -4 - 5 )",
      "0.444444444444444",
     },
    { "( 1 + 2 ) * 3", "( 1 + 2 ) * 3", "9" },
    // { 
    //   "1 - ( 3 + 4 ) ^ ( 5 + 6 )",
    //   "1 - ( 3 + 4 ) ^ ( 5 + 6 )",
    //   "-1977326742" // (1 - Math.pow(7, 11)).toString(),
    //  },
    // { "sqrt ( 4 )", "sqrt ( 4 )", "2" },
    { "1 0 / 2 * 5", "10 / 2 * 5", "25" },
    { "1 0 * 5 / 2", "10 * 5 / 2", "25" },
};

void runTests() {
    int pass = 0;
    CCalculate testCalc;
    IEvalEquationOptions options = {
        .trigRad = true
    };

    for (evalCase_t t : numberCases) {
        std::string chars = std::get<0>(t);
        std::string expect = std::get<2>(t);

        testCalc.reset();
        const char* cc = chars.c_str();

        for (size_t pos = 0, n = chars.length(); pos < n;) {
            size_t next = chars.find(" ", pos);
            if (next == std::string::npos) {
                next = n;
            }
            std::string cmd = chars.substr(pos, next - pos);
            pos = next + 1;

            const char ch =
                cmd == "±" ? eAction::ACTION_NEGATE
                    : cmd.c_str()[0];
            testCalc.scan(ch);
        }

        testCalc.parseEquation();
        testCalc.evalEquation(options);
        TToken* result = testCalc.result();
        bool success =
            (result != nullptr && result->toString() == expect) ||
            (result == nullptr && expect == "");


        if (success) {
            pass++;
        } else {
            std::cout << (success ? " ok  " : "FAIL ") << chars << ": ";
            if (result == nullptr) {
                std::cout << "(no result)";
            } else {
                std::cout << result->toString();
            }
            std::cout << std::endl;
        }
    }

    std::cout << pass << " passed" << std::endl;
}