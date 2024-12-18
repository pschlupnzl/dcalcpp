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
    /** Returns the type of this scanned token. */
    eScanType type() { return m_type; }
    virtual std::string toString() {
        return std::string("Scan:") + std::to_string(m_type);
    }
};

/** A scanned type number. */
class TScanNumber : public TScan {
private:
    std::string m_tok;
    bool m_hasDecimal;
    int m_fractionParts;
    /**
     * Parse the token string into fraction parts, returning a value indicating
     * whether the fraction was parsed.
     */
    bool toFractionParts(int* pwhole, int* pnum, int* pdenom);
public:
    TScanNumber() : TScan(SCAN_NUMBER) {
        m_hasDecimal = false;
        m_fractionParts = 0;
    };

    /** Append a character, including `.` decimal and `_` fraction. */
    void append(const char ch);
    /**
     * Instantiates a new TTokenValue representing the current string value.
     */
    TToken* toToken();
    std::string toString();
};

class TScanBinaryOp : public TScan {
private:
    eBinaryOpAction m_action;
public:
    TScanBinaryOp(eBinaryOpAction action) : TScan(SCAN_BINARYOP) {
        m_action = action;
    }
    /**
     * Instantiates a new TtokenBinaryOp representing the current operation.
     * @param iBrktOff Current bracket offset for operator precedence.
     */
    TTokenBinaryOp* toToken(int iBrktOff) {
        return new TTokenBinaryOp(m_action, iBrktOff);
    }
    std::string toString() {
        return m_action == BINARY_OP_ACTION_ADD ? "+" :
            m_action == BINARY_OP_ACTION_MULT ? "*" :
            m_action == BINARY_OP_ACTION_SUB ? "-" :
            m_action == BINARY_OP_ACTION_DIV ? "÷" :
            "?";
    }
};

class TScanOpen : public TScan {
public:
    TScanOpen() : TScan(SCAN_OPEN) { };
    std::string toString() { return "("; }
};

class TScanClose : public TScan {
public:
    TScanClose() : TScan(SCAN_CLOSE) { };
    std::string toString() { return ")"; }
};

#endif /* TSCAN_H */