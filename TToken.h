#ifndef TTOKEN_H
#define TTOKEN_H

#include <string>
#include "eAction.h"
#include "eTokenType.h"
#include "types.h"

/** Operator precedence added for each nested bracket. */
#define OP_BRACKETOFFSET 100

/** Operator precedence added for asTax or asPercent binary operaion. */
#define OP_PERCENTOFFSET 1

/**
 * Binary operator (BEDMAS) precedence. THE PRECISE VALUES HERE MATTER A LOT.
 * Another +1 is added for asPercent and asTax.
 */
#define BINARY_OP_PRECEDENCE(action) ( \
  action == eBinaryOpAction::BINARY_OP_ADD ? 0 : \
  action == eBinaryOpAction::BINARY_OP_SUB ? 0 : \
  action == eBinaryOpAction::BINARY_OP_MULT ? 4 : \
  action == eBinaryOpAction::BINARY_OP_DIV ? 4 :  \
  action == eBinaryOpAction::BINARY_OP_MULTNEG ? 6 : \
  action == eBinaryOpAction::BINARY_OP_ROOT ? 8 : \
  action == eBinaryOpAction::BINARY_OP_POW ? 10 : \
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
    TTokenValue() : TTokenResultBase(eTokenType::TOKEN_VALUE) { }
    TTokenValue(double value) : TTokenResultBase(eTokenType::TOKEN_VALUE)
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
    int m_negative;

    /** Update the `m_value` of the result base class. */
    void setValue() {
        double denom = m_denom == 0 ? 1.00 : (double)m_denom;
        m_value = ((double)m_whole) + ((double)m_num) / denom;
        if (m_negative) {
            m_value = -m_value;
        }
    }
public:
    TTokenFraction() : TTokenResultBase(eTokenType::TOKEN_FRACTION) { };
    TTokenFraction(int whole, int num, int denom, bool negative)
    : TTokenResultBase(eTokenType::TOKEN_FRACTION) {
        m_whole = whole;
        m_num = num;
        m_denom = denom;
        m_negative = negative;
        setValue();
    }
    /**
     * Initialize a new instance of the TTokenFraction class using signed
     * imperfect fraction components.
     */
    TTokenFraction(int num, int denom)
    : TTokenResultBase(eTokenType::TOKEN_FRACTION) {
        m_whole = 0;
        m_num = ABS(num);
        m_denom = ABS(denom);
        m_negative = (num < 0) != (denom < 0);
    }
    ~TTokenFraction() { }
    /**
     * Returns a value or fraction token, where possible, promoted to a fraction
     * with **signed improper** fraction parts, i.e. whole number is zero,
     * numerator is signed, denominator is positive, and the negative flag is
     * set. 
     */
    bool toSignedFraction(int* pnum, int* pdenom, bool* pneg);
    /** Returns the fraction parts as proper fraction with whole number. */
    bool toFractionParts(int* pwhole, int* pnum, int* pdenom, bool* pneg);
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
        : TToken(eTokenType::TOKEN_BINARYOP),
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

/**
 * A token representing a post-value unary action such as x! (factorial) or x²
 * (squared).
 */
class TTokenPostUnaryOp : public TToken {
private:
    ePostUnaryOpAction m_action;
public:
    /** 
     * Initializes a new instance of the TTokenPostUnaryOp class.
     */
    TTokenPostUnaryOp(ePostUnaryOpAction action)
        : TToken(eTokenType::TOKEN_POSTUNARYOP)
    {
        m_action = action;
    }
    /**
     * Evaluate the action on the given argument, returning a newly created
     * token that represents the result of the operation.
     */
    TToken* evaluate(TToken* pArg);

    std::string toString() {
        return m_action == ePostUnaryOpAction::POST_UNARY_OP_POW2 ? "^2"
        : "??";
    }
};

#endif /* TTOKEN_H */