#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "TToken.h"

/** Size of buffer for displaying a TTokenValue string. */
#define TTOKENVALUE_BUFFER_SIZE 32

std::string TTokenValue::toString() {
  char buffer[TTOKENVALUE_BUFFER_SIZE];
  snprintf(buffer, TTOKENVALUE_BUFFER_SIZE, "%.2lf", m_value);
  return std::string(buffer);
}


TToken* TTokenBinaryOp::evaluate(TToken* pArg1, TToken* pArg2) {
    /** Numerical arguments for unary or binary operators. */
    TTokenValue* dArg1 = pArg1->type() == TOKEN_VALUE ? (TTokenValue*)pArg1 : nullptr;
    TTokenValue* dArg2 = pArg2->type() == TOKEN_VALUE ? (TTokenValue*)pArg2 : nullptr;
    /** Evaluated value for numerical tokens. */
    double dVal;

    switch (m_action) {
        case ACTION_ADD:
            dVal = dArg1->value() + dArg2->value();
            break;

        case ACTION_SUB:
            dVal = dArg1->value() - dArg2->value();
            break;

        case ACTION_MULT:
        case ACTION_MULTNEG:
            dVal = dArg1->value() * dArg2->value();
            break;

        case ACTION_DIV:
            dVal = dArg1->value() / dArg2->value();
            break;
    }

    return new TTokenValue(dVal);
}


std::string TTokenBinaryOp::toString() {
    std::string str = std::string(
        m_action == BINARY_OP_ACTION_MULT ? "*" :
        m_action == BINARY_OP_ACTION_MULTNEG ? "#" :
        m_action == BINARY_OP_ACTION_DIV ? "÷" :
        m_action == BINARY_OP_ACTION_ADD ? "+" :
        m_action == BINARY_OP_ACTION_SUB ? "-" :
        "??");
    if (m_asPercent) {
        str += "%";
    } else if (m_asTax) {
        str += "†";
    }
    return str;
}