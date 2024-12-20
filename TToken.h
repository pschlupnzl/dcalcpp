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
    virtual ~TToken() { }

    /** Returns the type of entity represented by this token. */
    eTokenType type() const { return m_type; }

    virtual std::string toString() {
        return std::string("Tok:") + std::to_string(m_type);
    }
};

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
    TTokenResultBase(eTokenType type) : TToken(type) { m_value = 0; }
    virtual ~TTokenResultBase() { }
    /** Returns the numerical value of this token. */
    double value() { return m_value; }
};

/**
 * A token representing a numerical quantity, as resulting from an evaluation.
 */
class TTokenValue : public TTokenResultBase {
private:
public:
    TTokenValue() : TTokenResultBase(TOKEN_VALUE) { }
    TTokenValue(double value) : TTokenResultBase(TOKEN_VALUE)
    {
        m_value = value;
    }
    std::string toString();
};

class TTokenFraction : public TTokenResultBase {
private:
    int m_whole;
    int m_num;
    int m_denom;

    /** Update the `m_value` of the result base class. */
    void setValue() {
        m_value = ((double)m_whole) + ((double)m_num) / ((double)(m_denom));
    }
public:
    TTokenFraction() : TTokenResultBase(TOKEN_FRACTION) { };
    TTokenFraction(int whole, int num, int denom)
    : TTokenResultBase(TOKEN_FRACTION) {
        m_whole = whole;
        m_num = num;
        m_denom = denom;
        setValue();
    }
    ~TTokenFraction() { }
    /**
     * Fills the numerator and denominator references with the values of this
     * token represented as an imporper fraction, i.e. without whole number.
     */
    bool toFractionParts(int* pnum, int* pdenom);
    /** Simplifies the fraction. */
    void simplify();
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
    ~TTokenBinaryOp() { }
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