#ifndef EACTION_H
#define EACTION_H

/** Actions and operations of tokens. */
enum eAction {
    ACTION_UNDEFINED,

    ACTION_NUM0,
    ACTION_NUM1,
    ACTION_NUM2,
    ACTION_NUM3,
    ACTION_NUM4,
    ACTION_NUM5,
    ACTION_NUM6,
    ACTION_NUM7,
    ACTION_NUM8,
    ACTION_NUM9,
    ACTION_DECIMAL,
    ACTION_NEGATE,
    // // Constants.
    // ACTION_M_E,
    // ACTION_M_PI,
    // Basic binary operators.
    ACTION_MULT,
    ACTION_MULTNEG, // Multiply by negative number: -4/-2 ==> -1*4 / -1*2 but -2^2  -->  -(2^2)
    ACTION_DIV,
    ACTION_ADD,
    ACTION_SUB,
    // Brackets.
    ACTION_OPEN,
    ACTION_CLOSE,
    // ACTION_CLOSE_AUTO,
    // // Powers.
    ACTION_SQRT,
    ACTION_POW2,
    ACTION_POW,
    // // Trigonometry.
    // ACTION_SIN,
    // ACTION_COS,
    // ACTION_TAN,
    // ACTION_ASIN,
    // ACTION_ACOS,
    // ACTION_ATAN,

    // // // Extra functions
    // ACTION_POW3,      // cubed
    // ACTION_SQRT3,     // cubic root
    ACTION_ROOT = 'v',      // x root y
    // ACTION_POWE,      // e^x
    // ACTION_POW10,     // 10^x
    // ACTION_LOG,       // ln(x)
    // ACTION_LOG10,     // log10(x)
    // ACTION_FACTORIAL, // x!
    // ACTION_SINH,
    // ACTION_COSH,
    // ACTION_TANH,
    // ACTION_ASINH,
    // ACTION_ACOSH,
    // ACTION_ATANH,

    // // Percent and Tax.
    // ACTION_PERCENT,
    // ACTION_TAX,
    // Fractions
    ACTION_FRACTION = '_',
    // // Time
    // ACTION_HOURS,
    // ACTION_MINUTES,
    // ACTION_SECONDS,
    // ACTION_AMPM,
    // // Deg-min-sec
    // ACTION_ANGLE,
    // // Last answer as action
    // ACTION_LAST_ANSWER,
    // // Actions.
    // ACTION_CLEAR,
    // ACTION_DEL,
    // ACTION_EQUALS,
    // ACTION_SOLVE,
    // ACTION_SETTINGS,
    // ACTION_INV,
};

/**
 * Actions used in TOKEN_BINARYOP.
 */
enum eBinaryOpAction
{
    /** Binar */
    BINARY_OP_ACTION_PARSE_COMPLETE = -1,

    BINARY_OP_ACTION = 100,
    BINARY_OP_MULT,
    BINARY_OP_MULTNEG,
    BINARY_OP_DIV,
    BINARY_OP_ADD,
    BINARY_OP_SUB,

    BINARY_OP_ROOT,
    BINARY_OP_POW,
};

enum ePostUnaryOpAction {
    POST_UNARY_OP_ACTION = 200,
    POST_UNARY_OP_POW2,
};

enum eUnaryOpAction {
    UNARY_OP_ACTION = 300,
    UNARY_OP_SQRT,
};

#endif /* EACTION_H */