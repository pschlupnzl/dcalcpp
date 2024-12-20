#include <sstream>
#include <stdlib.h>
#include <numeric> // for gcd
#include "TToken.h"

/** Size of buffer for displaying a TTokenValue string. */
#define TTOKENVALUE_BUFFER_SIZE 32

std::string TTokenValue::toString() {
  char buffer[TTOKENVALUE_BUFFER_SIZE];
  snprintf(buffer, TTOKENVALUE_BUFFER_SIZE, "%.2lf", m_value);
  return std::string(buffer);
}

void TTokenFraction::simplify() {
    // TODO: NEGATIVE!
    m_whole += m_num / m_denom;
    m_num -= m_whole * m_denom;
    int gcd = std::gcd(m_num, m_denom);
    m_num /= gcd;
    m_denom /= gcd;
    setValue();
}

bool TTokenFraction::toFractionParts(int *pnum, int *pdenom)
{
    *pnum = m_whole * m_denom + m_num;
    *pdenom = m_denom;
    return true;
}

std::string TTokenFraction::toString()
{
    char buffer[TTOKENVALUE_BUFFER_SIZE];
    snprintf(buffer, TTOKENVALUE_BUFFER_SIZE, "%i~%i/%i", m_whole, m_num, m_denom);
    return std::string(buffer);
}

std::string TTokenBinaryOp::toString() {
    std::string str = std::string(
        m_action == BINARY_OP_ACTION_ADD ? " + " :
        m_action == BINARY_OP_ACTION_MULT ? " x " :
        m_action == BINARY_OP_ACTION_MULTNEG ? " -x " :
        m_action == BINARY_OP_ACTION_SUB ? " - " :
        m_action == BINARY_OP_ACTION_DIV ? " ÷ " :
        "?");
    if (m_asPercent) {
        str += "%";
    } else if (m_asTax) {
        str += "†";
    }
    return str;
}
