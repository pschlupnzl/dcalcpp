#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../calc/CCalculate.h"
#include "../calc/TToken.h"
#include "../calc/keymap.h"

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

#ifndef EMSCRIPTEN_KEEPALIVE
#define EMSCRIPTEN_KEEPALIVE __attribute__((used))
#endif /* EMSCRIPTEN_KEEPALIVE */


// Compile with e.g. in Docker container
//   #  emcc -o dist.html --shell-file dcalcpp.htm -s NO_EXIT_RUNTIME=1 -s "EXPORTED_RUNTIME_METHODS=['ccall']" dcalcpp.cpp ../calc/*.cpp
// The generated code needs to be hosted
//   $ python3 -m http.server
// And then load it in the browser
//   http://localhost:8000/dist.html
// Call for JavaScript
//  Module.ccall('myStringFunction', 'number', ['string'], ['hello world'])


// EXTERN EMSCRIPTEN_KEEPALIVE int myFunction (char ch) {
//     // Invoke from JavaScript with Module.ccall('myFunction', 'number', ['number'], [49])
//     printf("myFunction called with char %d\n", ch);
//     t.hello();
//     return (int)ch;
// }

// EXTERN EMSCRIPTEN_KEEPALIVE int myStringFunction (char *str) {
//     // Invoke from JavaScript with Module.ccall('myStringFunction', 'number', ['string'], ['hello'])
//     printf("Hello %s", str);
//     return strlen(str);
// }

// int count = 0;

// EXTERN EMSCRIPTEN_KEEPALIVE char* myFunctionString() {
//     char *buf = (char*) malloc(32);
//     snprintf(buf, 32, "hello %d", ++count);
//     return buf;
// }

CCalculate calc;

EMSCRIPTEN_KEEPALIVE void reset() {
    calc.reset();
}

/**
 * Read a single command or action, appending the current calculator state.
 */
EXTERN EMSCRIPTEN_KEEPALIVE char* doScan(char ch) {
    eCommand cmd = commandFromKeyboard(ch);
    if (cmd != eCommand::CMD_UNDEFINED) {
        calc.command(cmd);
    } else {
        calc.scan(actionFromKeyboard(ch));
    }
    calc.parseEquation();
    calc.evalEquation();
    char *buf = (char*) malloc(512);
    snprintf(buf, 512, "%s", calc.toDisplayString().c_str());
    return buf;
}

/**
 * Read an entire string of commands or actions.
 */
EXTERN EMSCRIPTEN_KEEPALIVE char* doCalc(char* src) {
    // Invoke from JavaScript with result = Module.ccall('doCalc', 'string', ['string'], ['1+2*3'])
    reset();
    for (const char* pch = src; *pch; ++pch) {
        eCommand cmd = commandFromKeyboard(*pch);
        if (cmd != eCommand::CMD_UNDEFINED) {
            calc.command(cmd);
        } else {
            calc.scan(actionFromKeyboard(*pch));
        }
    }

    calc.parseEquation();
    calc.evalEquation();
    char *buf = (char*) malloc(512);
    snprintf(buf, 512, "%s", calc.toDisplayString().c_str());
    return buf;
}

int main(int argc, const char* argv[]) {
    printf("Hello there big wide world\n");
    return 0;
}
