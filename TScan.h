#ifndef TSCAN_H
#define TSCAN_H

#include <string>
#include "eAction.h"
#include "eScanType.h"
#include "TToken.h"

class IScan {
// protected:
//     eScanType m_type;
public:
    /** Returns the type of this scanned token. */
    virtual eScanType type() = 0;
    /** Returns a string representation of this token. */
    virtual std::string toString() = 0;
};

/** A scanned type number. */
class TScanNumber : public IScan {
private:
    /** Characters representing string being entered. */
    std::string m_tok;
    /** Value indicating whether value is negative. */
    bool m_negative;
    /** Value indicating whether string has a decimal point. */
    bool m_hasDecimal;
    /** Number of fraction parts, i.e. count of '_' characters in m_tok. */
    int m_fractionParts;
public:
    TScanNumber() {
        m_negative = false;
        m_hasDecimal = false;
        m_fractionParts = 0;
    };
    eScanType type() { return eScanType::SCAN_NUMBER; }
    /** Append a character, including `.` decimal and `_` fraction. */
    void append(eAction action);
    /**
     * Parse the token string into fraction parts, returning a value indicating
     * whether the fraction was parsed.
     */
    bool toFractionParts(int* pwhole, int* pnum, int* pdenom, bool* pneg);
    /**
     * Instantiates a new TTokenValue or TTokenFracton that represents the 
     * current string value.
     */
    ITokenResultBase* toToken();
    std::string toString();
};

class TScanBinaryOp : public IScan {
private:
    eBinaryOpAction m_action;
public:
    TScanBinaryOp(eBinaryOpAction action) {
        m_action = action;
    }
    eScanType type() { return eScanType::SCAN_BINARYOP; }
    /**
     * Instantiates a new TtokenBinaryOp representing the current operation.
     * @param iBrktOff Current bracket offset for operator precedence.
     */
    TTokenBinaryOp* toToken(int iBrktOff) {
        return new TTokenBinaryOp(m_action, iBrktOff);
    }
    std::string toString() {
        return std::string(
          m_action == eBinaryOpAction::BINARY_OP_ADD ? "+" :
          m_action == eBinaryOpAction::BINARY_OP_MULT ? "x" :
          m_action == eBinaryOpAction::BINARY_OP_SUB ? "-" :
        //   m_action == eBinaryOpAction::BINARY_OP_DIV ? "÷" :
          m_action == eBinaryOpAction::BINARY_OP_DIV ? "/" :
          m_action == eBinaryOpAction::BINARY_OP_POW ? "^" :
          m_action == eBinaryOpAction::BINARY_OP_ROOT ? "v" :
          "?");
    }
};

class TScanUnaryOp : public IScan {
private:
    eUnaryOpAction m_action;
public:
    TScanUnaryOp(eUnaryOpAction action) {
        m_action = action;
    }
    eScanType type() { return eScanType::SCAN_UNARYOP; }
    TTokenUnaryOp* toToken(int iBrktOff) {
        return new TTokenUnaryOp(m_action, iBrktOff);
    }
    std::string toString() {
        return std::string(
            m_action == eUnaryOpAction::UNARY_OP_SQRT ? "2v" :
            m_action == eUnaryOpAction::UNARY_OP_SIN ? "sin" :
            m_action == eUnaryOpAction::UNARY_OP_COS ? "cos" :
            m_action == eUnaryOpAction::UNARY_OP_TAN ? "tan" :
            m_action == eUnaryOpAction::UNARY_OP_ASIN ? "asin" :
            m_action == eUnaryOpAction::UNARY_OP_ACOS ? "acos" :
            m_action == eUnaryOpAction::UNARY_OP_ATAN ? "atan" :
            "?");
    }
};

class TScanPostUnaryOp : public IScan {
private:
    ePostUnaryOpAction m_action;
public:
    TScanPostUnaryOp(ePostUnaryOpAction action) {
        m_action = action;
    };
    eScanType type() { return eScanType::SCAN_POSTUNARYOP; }
    TTokenPostUnaryOp* toToken(int iBrktOff) {
        return new TTokenPostUnaryOp(m_action, iBrktOff);
    }
    std::string toString() {
        return std::string(
            m_action == ePostUnaryOpAction::POST_UNARY_OP_POW2 ? "^2" :
            "?");
    }
};

class TScanOpen : public IScan {
public:
    eScanType type() { return eScanType::SCAN_OPEN; }
    std::string toString() { return "("; }
};

class TScanClose : public IScan {
public:
    eScanType type() { return eScanType::SCAN_CLOSE; }
    std::string toString() { return ")"; }
};

#endif /* TSCAN_H */