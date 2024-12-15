#ifndef TSCAN_H
#define TSCAN_H

#include <string>
#include "eAction.h"
#include "TToken.h"

enum eScanType {
  SCAN_UNDEFINED,
  /** Input number only. */
  SCAN_NUMBER,
  /** Input binary operator. */
  SCAN_BINARYOP,
};

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
public:
    TScanNumber() : TScan(SCAN_NUMBER) { };
    void append(const char ch) {
        m_tok += ch;
    };
    /**
     * Instantiates a new TTokenValue representing the current string value.
     */
    TTokenValue* toToken() {
        return new TTokenValue(std::stod(m_tok));
    }
    std::string toString() {
        return m_tok;
    }
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
        return BINARY_OP_ACTION_ADD ? "+" :
            BINARY_OP_ACTION_MULT ? "*" :
            BINARY_OP_ACTION_SUB ? "-" :
            BINARY_OP_ACTION_DIV ? "÷" :
            "?";
    }
};

#endif /* TSCAN_H */