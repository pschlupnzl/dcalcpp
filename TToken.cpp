#include <sstream>
#include <stdlib.h>
#include <numeric> // for gcd
#include <string> // for npos, find
#include "TToken.h"
#include "types.h"

/** Decimal separator. */
const char DECIMAL = '.';
/** Thousands separator. */
const char THOU = ',';

/** Size of buffer for displaying a TTokenValue string. */
#define TTOKENVALUE_BUFFER_SIZE 32

std::string TTokenValue::toString() {
  // Format using C++ snprintf into a C string.
  // C++20 introduces std::format.
  // See also https://stackoverflow.com/a/21162120/13253316.
  char buffer[TTOKENVALUE_BUFFER_SIZE];
  snprintf(buffer, TTOKENVALUE_BUFFER_SIZE, "%.15g", m_value);
  std::string str = buffer;

  // Trim unused zeros.
  size_t decimal = str.find('.');
  if (decimal != std::string::npos) {
    size_t pos = str.length() - 1;
    for (; pos > decimal && str[pos] == '0'; --pos) {
      // Remove last zero.
      str.pop_back();
    }
    if (pos == decimal) {
      // Remove decimal at end.
      str.pop_back();
    } else {
      str[decimal] = DECIMAL;
    }
  }

  int16_t thou = decimal == std::string::npos ? str.length() : decimal;
  for (thou -= 3; thou >= 1; thou -= 3) {
    str.insert(thou, std::string(1, THOU));
  }

  return str;
}

void TTokenFraction::simplify() {
  if (m_denom) {
    int improper = m_num / m_denom;
    m_whole += improper;
    m_num -= improper * m_denom;
    int gcd = std::gcd(m_num, m_denom);
    m_num /= gcd;
    m_denom /= gcd;
  }
  setValue();
}

bool TTokenFraction::toSignedFraction(int *pnum, int *pdenom, bool *pneg)
{
    int num = m_whole * m_denom + m_num;
    *pnum = m_negative ? -num : num;
    *pdenom = m_denom;
    *pneg = m_negative;
    return true;
}

bool TTokenFraction::toFractionParts(int *pwhole, int *pnum, int *pdenom, bool *neg) {
    *pwhole = m_whole;
    *pnum = m_num;
    *pdenom = m_denom;
    *neg = m_negative;
    return true;
}

std::string TTokenFraction::toString()
{
    char buffer[TTOKENVALUE_BUFFER_SIZE];
    if (m_whole) {
        snprintf(buffer, TTOKENVALUE_BUFFER_SIZE, "%s%i_%i_%i", m_negative ? "-" : "", m_whole, m_num, m_denom);
    } else {
        snprintf(buffer, TTOKENVALUE_BUFFER_SIZE, "%s%i_%i", m_negative ? "-" : "", m_num, m_denom);
    }
    return std::string(buffer);
}

std::string TTokenBinaryOp::toString() {
    std::string str = std::string(
        m_action == BINARY_OP_ACTION_ADD ? " + " :
        m_action == BINARY_OP_ACTION_MULT ? " x " :
        m_action == BINARY_OP_ACTION_MULTNEG ? " -x " :
        m_action == BINARY_OP_ACTION_SUB ? " - " :
        // m_action == BINARY_OP_ACTION_DIV ? " ÷ " :
        m_action == BINARY_OP_ACTION_DIV ? " / " :
        "?");
    if (m_asPercent) {
        str += "%";
    } else if (m_asTax) {
        str += "†";
    }
    return str;
}
