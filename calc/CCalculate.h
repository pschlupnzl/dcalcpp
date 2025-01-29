#ifndef CCALCULATE_H
#define CCALCULATE_H

#include <functional>
#include <string>
#include <vector>
#include "eErrors.h"
#include "eAction.h"
#include "TScan.h"
#include "TToken.h"
#include "types.h"

// /** Operator precedence added for each nested bracket. */
// #define OP_BRACKETOFFSET 100

// /** Operator precedence added for asTax or asPercent binary operaion. */
// #define OP_PERCENTOFFSET 1

// /**
//  * Binary operator (BEDMAS) precedence. THE PRECISE VALUES HERE MATTER A LOT.
//  * Another +1 is added for asPercent and asTax.
//  */
// #define BINARY_OP_PRECEDENCE(action) ( \
//   action == eBinaryOpAction::BINARY_OP_ADD ? 0 : \
//   action == eBinaryOpAction::BINARY_OP_SUB ? 0 : \
//   action == eBinaryOpAction::BINARY_OP_MULT ? 4 : \
//   action == eBinaryOpAction::BINARY_OP_DIV ? 4 :  \
//   action == eBinaryOpAction::BINARY_OP_MULTNEG ? 6 : \
//   action == eBinaryOpAction::BINARY_OP_ROOT ? 8 : \
//   action == eBinaryOpAction::BINARY_OP_POW ? 10 : \
//   0)

// /** 
//  * Unary operator precedence (they're in order anyway). These are higher than
//  * binary op precedence.
//  */
// #define UNARY_OP_PRECEDENCE (action) = ( \
//   action == UNARY_OP_ACTION_SQRT ? : 20 \
//   action == UNARY_OP_ACTION_SQRT3 ? : 21 \
//   action == UNARY_OP_ACTION_SIN ? : 22 \
//   action == UNARY_OP_ACTION_COS ? : 23 \
//   action == UNARY_OP_ACTION_TAN ? : 24 \
//   action == UNARY_OP_ACTION_ASIN ? : 25 \
//   action == UNARY_OP_ACTION_ACOS ? : 26 \
//   action == UNARY_OP_ACTION_ATAN ? : 27 \
//   20)

/**
 * Class to evaluate an equation to find the answer.
 */
class CCalculate {
private:
    ICalcOptions m_options = {
      .trigRad = false,
      .deciSep = 0x00,
      .thouSep = 0x00,
      .fixedDecimals = 0,
      .autoOpen = false,
    };
    
    /** Keys entered, used for backspace. */
    std::vector<eAction> m_actions;
    /** Parsed tokens in the order entered by the user. */
    std::vector<IScan*> m_scan;
    /** Tokens parsed into RPN representation. */
    std::vector<IToken*> m_pvoEquation;
    /** Final result, or `nullptr` if no solution to be had. */
    ITokenResultBase* m_presult = nullptr;
    /** Number of automatically-closing brackets after evaluation. */
    int m_iBrktOff = 0;
    /** Location of last error, when parsing. */
    size_t m_lastErrorLocation = 0;
    /** Last error that happened. */
    eErrors m_lastError = ERROR_NONE;
    /** Register a parsing error. */
    void errorParsing(size_t location, eErrors error) {
        m_lastErrorLocation = location;
        m_lastError = error;
    }
    /** Register an evaluation error. */
    void errorEvaluating(eErrors error) {
      m_lastError = error;
    }

    /**
     * Scan the action without adding to m_actions array, returning a value
     * indicating whether the action was scanned.
     */
    bool scanSilent(eAction action);

public:
    CCalculate() { }
    ~CCalculate() {
      reset();
    }

    /** Returns a reference to the current calculation options. */
    const ICalcOptions& getOptions() { return m_options; }
    /** Set the trig function radial / degree switch. */
    void setTrigRad(bool trigRad) { m_options.trigRad = trigRad; }
    /** Set the behaviour of automatically inserting parentheses after unary operators. */
    void setAutoOpen(bool autoOpen) { m_options.autoOpen = autoOpen; }
    /** Reset the equation - this is te AC (clear all) action. */
    void reset();
    /** Reset the `m_actions` vector. */
    void reset_actions();
    /** Reset the `m_scan` vector. */
    void reset_scan();
    /** Reset the `m_pvoEquation` vector. */
    void reset_pvoEquation();
    /** Reset the `m_presult` pointer. */
    void reset_result();

    /**
     * Process the given command, returning a value indicating whether the
     * command was handled.
     */
    bool command(eCommand cmd);

    /**
     * Scan the character and add to the current equation, returning a value
     * indicating whether the action was allowed.
     * @param action The keypress action to add to the current equation.
     * @param autoOpen Optional value to automatically insert open * parenthesis
     * after unary operators, useful for interactive scanning.
    */
    bool scan(eAction action);

    /** Remove the most recent action (character), returning the action removed. */
    void backspace();

    /** Parse the tokens into the RPN stack. */
    void parseEquation();
    /** Evaluate the RPN stack to attain the result. */
    void evalEquation();

    /**
     * Iterate over the scanned tokens, used e.g. to display the read equation.
     * @param fn Delegate that is called with each token.
     */
    // void forEach(void f(IScan* scan));
    void forEach(const std::function<void(IScan*)> fn);

    /** Returns a string representation of the entire calculation. */
    std::string toString();
    /** Returns a string representation that can be parsed for display. */
    std::string toDisplayString();
    /** Returns a string representation to be shown in the result preview display. */
    std::string toResultString();
    /** Returns a reference to the result token, or nullptr if not solved. */
    ITokenResultBase* result() const {
      return m_presult;
    }
};

#endif /* CCALCULATE_H */