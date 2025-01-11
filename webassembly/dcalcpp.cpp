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
ICalcOptions options;

EMSCRIPTEN_KEEPALIVE void reset() {
    calc.reset();
    options = ICalcOptions({
        .trigRad = true,
        .deciSep = 0x00,
        .thouSep = 0x00,
        .fixedDecimals = 0,
    });
}

EMSCRIPTEN_KEEPALIVE void handleCommand(
    eCommand cmd,
    CCalculate& calc,
    ICalcOptions& options
) {
    switch (cmd) {
        case eCommand::CMD_AC:
            calc.reset();
            break;
        case eCommand::CMD_TRIGRAD:
            options.trigRad = !options.trigRad;
            break;
        case eCommand::CMD_DECISEP:
            options.deciSep = options.deciSep == 0x00 ? ',' : 0x00;
            break;
        case eCommand::CMD_THOUSEP:
            options.thouSep = options.thouSep == 0x00 ? '\'' : 0x00;
            break;
        case eCommand::CMD_FIXEDDECIMALS:
            options.fixedDecimals =
                options.fixedDecimals < 1 ? 3 : options.fixedDecimals - 1;
            break;
        default: /* nop */ break;
    }
}

/**
 * Read a single command or action, appending the current calculator state.
 */
EXTERN EMSCRIPTEN_KEEPALIVE char* doScan(char ch) {
    eCommand cmd = commandFromKeyboard(ch);
    if (cmd != eCommand::CMD_UNDEFINED) {
        handleCommand(cmd, calc, options);
    } else {
        calc.scan(actionFromKeyboard(ch));
    }
    calc.parseEquation();
    calc.evalEquation(options);
    char *buf = (char*) malloc(512);
    snprintf(buf, 512, "%s", calc.toDisplayString(options).c_str());
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
            handleCommand(cmd, calc, options);
        } else {
            calc.scan(actionFromKeyboard(*pch));
        }
    }

    calc.parseEquation();
    calc.evalEquation(options);
    char *buf = (char*) malloc(512);
    snprintf(buf, 512, "%s", calc.toDisplayString(options).c_str());
    return buf;
}

int main(int argc, const char* argv[]) {
    printf("Hello there big wide world\n");
    return 0;
}
