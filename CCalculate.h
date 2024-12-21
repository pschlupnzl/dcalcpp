#ifndef CCALCULATE_H
#define CCALCULATE_H

#include <functional>
#include <string>
#include <vector>
#include "eErrors.h"
#include "TScan.h"
#include "TToken.h"

// /** Operator precedence added for each nested bracket. */
// #define OP_BRACKETOFFSET 100

// /** Operator precedence added for asTax or asPercent binary operaion. */
// #define OP_PERCENTOFFSET 1

// /**
//  * Binary operator (BEDMAS) precedence. THE PRECISE VALUES HERE MATTER A LOT.
//  * Another +1 is added for asPercent and asTax.
//  */
// #define BINARY_OP_PRECEDENCE(action) ( \
//   action == BINARY_OP_ACTION_ADD ? 0 : \
//   action == BINARY_OP_ACTION_SUB ? 0 : \
//   action == BINARY_OP_ACTION_MULT ? 4 : \
//   action == BINARY_OP_ACTION_DIV ? 4 :  \
//   action == BINARY_OP_ACTION_MULTNEG ? 6 : \
//   action == BINARY_OP_ACTION_ROOT ? 8 : \
//   action == BINARY_OP_ACTION_POW ? 10 : \
//   0)

/** 
 * Unary operator precedence (they're in order anyway). These are higher than
 * binary op precedence.
 */
#define UNARY_OP_PRECEDENCE (action) = ( \
  action == UNARY_OP_ACTION_SQRT ? : 20 \
  action == UNARY_OP_ACTION_SQRT3 ? : 21 \
  action == UNARY_OP_ACTION_SIN ? : 22 \
  action == UNARY_OP_ACTION_COS ? : 23 \
  action == UNARY_OP_ACTION_TAN ? : 24 \
  action == UNARY_OP_ACTION_ASIN ? : 25 \
  action == UNARY_OP_ACTION_ACOS ? : 26 \
  action == UNARY_OP_ACTION_ATAN ? : 27 \
  20)

typedef struct {
  /** Value indicating whether to use Radians for trig functions (instead of rad). */
    bool trigRad;
} IEvalEquationOptions;

/**
 * Class to evaluate an equation to find the answer.
 */
class CCalculate {
private:
    /** Parsed tokens in the order entered by the user. */
    std::vector<TScan*> m_scan;
    /** Tokens parsed into RPN representation. */
    std::vector<TToken*> m_pvoEquation;
    /** Final result, or `nullptr` if no solution to be had. */
    TToken* m_presult = nullptr;
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

public:
    CCalculate() { }
    ~CCalculate() {
      reset_scan();
      reset_pvoEquation();
      reset_result();
    }

    /** Reset the equation - this is te AC (clear all) action. */
    void reset();
    /** Reset the `m_scan` vector. */
    void reset_scan();
    /** Reset the `m_pvoEquation` vector. */
    void reset_pvoEquation();
    /** Reset the `m_presult` pointer. */
    void reset_result();

    /**
     * Scan the character and add to the current equation.
     * @param ch The character or keypress to add to the current equation.
    */
    void scan(const char ch);

    /** Parse the tokens into the RPN stack. */
    void parseEquation();
    /** Evaluate the RPN stack to attain the result. */
    void evalEquation(IEvalEquationOptions& options);

    /**
     * Iterate over the scanned tokens, used e.g. to display the read equation.
     * @param fn Delegate that is called with each token.
     */
    // void forEach(void f(TScan* scan));
    void forEach(const std::function<void(TScan*)> fn);

    /** Returns a string representation of the entire calculation. */
    std::string toString();
    /** Returns a reference to the result token, or nullptr if not solved. */
    TToken* result() {
      return m_presult;
    }
};

#endif /* CCALCULATE_H */