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
    TScanNumber() : TScan(SCAN_NUMBER) {
        m_negative = false;
        m_hasDecimal = false;
        m_fractionParts = 0;
    };
    ~TScanNumber() { }
    /** Append a character, including `.` decimal and `_` fraction. */
    void append(const char ch);
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
    TScanBinaryOp(eBinaryOpAction action) : TScan(SCAN_BINARYOP) {
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
          m_action == BINARY_OP_ACTION_ADD ? " + " :
          m_action == BINARY_OP_ACTION_MULT ? " x " :
          m_action == BINARY_OP_ACTION_SUB ? " - " :
        //   m_action == BINARY_OP_ACTION_DIV ? " ÷ " :
          m_action == BINARY_OP_ACTION_DIV ? " / " :
          "?");
    }
};

class TScanOpen : public TScan {
public:
    TScanOpen() : TScan(SCAN_OPEN) { };
    ~TScanOpen() { }
    std::string toString() { return "("; }
};

class TScanClose : public TScan {
public:
    TScanClose() : TScan(SCAN_CLOSE) { };
    ~TScanClose() { }
    std::string toString() { return ")"; }
};

#endif /* TSCAN_H */