#ifndef TTOKEN_H
#define TTOKEN_H

#include <string>
#include "eAction.h"
#include "eTokenType.h"

/** Operator precedence added for each nested bracket. */
#define OP_BRACKETOFFSET 100

/** Operator precedence added for asTax or asPercent binary operaion. */
#define OP_PERCENTOFFSET 1

/**
 * Binary operator (BEDMAS) precedence. THE PRECISE VALUES HERE MATTER A LOT.
 * Another +1 is added for asPercent and asTax.
 */
#define BINARY_OP_PRECEDENCE(action) ( \
  action == BINARY_OP_ACTION_ADD ? 0 : \
  action == BINARY_OP_ACTION_SUB ? 0 : \
  action == BINARY_OP_ACTION_MULT ? 4 : \
  action == BINARY_OP_ACTION_DIV ? 4 :  \
  action == BINARY_OP_ACTION_MULTNEG ? 6 : \
  action == BINARY_OP_ACTION_ROOT ? 8 : \
  action == BINARY_OP_ACTION_POW ? 10 : \
  0)



class TToken {
protected:
    eTokenType m_type;
public:
    /**
     * Initializes a new instance of the TToken class with the given token type.
     * @param type Type of token derived from this base class.
     */
    TToken(eTokenType type)
        : m_type(type) {}

    /** Returns the type of entity represented by this token. */
    eTokenType type() const { return m_type; }

    virtual std::string toString() {
        return std::string("Tok:") + std::to_string(m_type);
    }
};

// /**
//  * Number-type token being scanned during input and lexing.
//  */
// class TTokenNumber : public TToken {
// private:
//     /** String representation as entered by the user. */
//     std::string m_tok;
// public:
//     TTokenNumber() : TToken(TOKEN_NUMBER) { }
//     /** Append a character to the string. */
//     void append(const char ch);
//     std::string toString();
// };

/**
 * A result-type token, entered or derived. This is limited to:
 *  - eTokenType.VALUE
 *  - eTokenType.FRACTION
 *  - eTokenType.TIME
 *  - eTokenType.ANGLE.
 */
class TTokenResultBase : public TToken {
protected:
    /** Numerical value of the token. */
    double m_value;
    // /** Parent token, if derived. */
    // TToken* m_parent;
    // /** Value indicating that this token represents the solved RHS. */
    // bool m_solve;
public:
    TTokenResultBase(eTokenType type) : TToken(type), m_value(0.0) { }

    TTokenResultBase(eTokenType type, double value) : TToken(type), m_value(value) { }
};

/**
 * A token representing a numerical quantity, as resulting from an evaluation.
 */
class TTokenValue : public TTokenResultBase {
private:
    /** String representation as entered by the user. */
    // std::string m_tok;
public:
    TTokenValue() : TTokenResultBase(TOKEN_VALUE) { m_value = 0.0; }
    TTokenValue(double value) : TTokenResultBase(TOKEN_VALUE)
    {
        m_value = value;
    }
    // /** Append a character to the string. */
    // void append(const char ch);
    /** Return the numerical value of this token. */
    double value() { return m_value; }
    std::string toString();
};

/**
 * A token representing a numerical value during execution.
 */
class TTokenBinaryOp : public TToken {
private:
    eBinaryOpAction m_action;
    bool m_asPercent;
    bool m_asTax;

    /** Operator precedence, set at parseEquation. */
    int m_op;
public:
    /**
     * Initializes a new instance of the TTokenBinaryOp class, setting the
     * operator precedence based on the current bracket level.
     * @param action Action for this operator.
     * @param iBrktOff Current bracket offset.
     */
    TTokenBinaryOp(eBinaryOpAction action, int iBrktOff)
        : TToken(TOKEN_BINARYOP),
        m_action(action),
        m_asPercent(false),
        m_asTax(false)
    {
        m_op = BINARY_OP_PRECEDENCE(m_action)
            + (m_asTax || m_asPercent ? OP_PERCENTOFFSET : 0)
            + iBrktOff * OP_BRACKETOFFSET;
    }

    /** Sets this operator's precedence, including the bracket offset. */
    void setPrecedence(int iBrktOff);
    /** Returns this operator's precedence. */
    int op() { return m_op; };
    /**
     * Evaluate the action on the arguments, returning a newly created token
     * that represents the result of the oporation.
     */
    TToken* evaluate(TToken* dArg1, TToken* dArg2);

    std::string toString();
};

#endif /* TTOKEN_H */