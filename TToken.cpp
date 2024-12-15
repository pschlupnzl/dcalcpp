#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "TToken.h"

// std::string TTokenNumber::toString()
// {
//     return m_tok;
// }

// void TTokenNumber::append(const char ch) {
//     m_tok += ch;
//     // try {
//     //     m_value = std::stod(m_tok);
//     // } catch (std::invalid_argument ex) {
//     //     // Something like parsing just "." without any numbers.
//     //     // NOT SURE WHAT TO DO HERE - catch in CCalculate, probably
//     // }
// }

// std::string TTokenNumber::toString() {
//     return m_tok;
// }

std::string TTokenValue::toString() {
    // return "'" + m_tok + "':" + std::to_string(m_value);
    return std::to_string(m_value);
}


TToken* TTokenBinaryOp::evaluate(TToken* pArg1, TToken* pArg2) {
    /** Numerical arguments for unary or binary operators. */
    TTokenValue* dArg1 = dynamic_cast<TTokenValue*>(pArg1);
    TTokenValue* dArg2 = dynamic_cast<TTokenValue*>(pArg2);
    /** Evaluated value for numerical tokens. */
    double dVal;

    // if (dArg1 != nullptr && dArg2 != nullptr) ...
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
        m_action == BINARY_OP_ACTION_MULT ? "×" :
        m_action == BINARY_OP_ACTION_MULTNEG ? "-×" :
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