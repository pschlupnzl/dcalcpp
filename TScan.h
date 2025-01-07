#ifndef TSCAN_H
#define TSCAN_H

#include <string>
#include "eAction.h"
#include "eScanType.h"
#include "TToken.h"

class TScan {
protected:
    eScanType m_type;
public:
    TScan(eScanType type) {
        m_type = type;
    }
    virtual ~TScan() { }
    /** Returns the type of this scanned token. */
    eScanType type() { return m_type; }
    virtual std::string toString() {
        return std::string("Scan:") + std::to_string(m_type);
    }
};

/** A scanned type number. */
class TScanNumber : public TScan {
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
    TScanNumber() : TScan(eScanType::SCAN_NUMBER) {
        m_negative = false;
        m_hasDecimal = false;
        m_fractionParts = 0;
    };
    ~TScanNumber() { }
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
    TToken* toToken();
    std::string toString();
};

class TScanBinaryOp : public TScan {
private:
    eBinaryOpAction m_action;
public:
    TScanBinaryOp(eBinaryOpAction action) : TScan(eScanType::SCAN_BINARYOP) {
        m_action = action;
    }
    ~TScanBinaryOp() { }
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

class TScanUnaryOp : public TScan {
private:
    eUnaryOpAction m_action;
public:
    TScanUnaryOp(eUnaryOpAction action)
        : TScan(eScanType::SCAN_UNARYOP) {
        m_action = action;
    }
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

class TScanPostUnaryOp : public TScan {
private:
    ePostUnaryOpAction m_action;
public:
    TScanPostUnaryOp(ePostUnaryOpAction action)
        : TScan(eScanType::SCAN_POSTUNARYOP) {
        m_action = action;
    };
    TTokenPostUnaryOp* toToken(int iBrktOff) {
        return new TTokenPostUnaryOp(m_action, iBrktOff);
    }
    std::string toString() {
        return std::string(
            m_action == ePostUnaryOpAction::POST_UNARY_OP_POW2 ? "^2" :
            "?");
    }
};

class TScanOpen : public TScan {
public:
    TScanOpen() : TScan(eScanType::SCAN_OPEN) { };
    ~TScanOpen() { }
    std::string toString() { return "("; }
};

class TScanClose : public TScan {
public:
    TScanClose() : TScan(eScanType::SCAN_CLOSE) { };
    ~TScanClose() { }
    std::string toString() { return ")"; }
};

#endif /* TSCAN_H */