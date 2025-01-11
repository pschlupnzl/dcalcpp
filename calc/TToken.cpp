#include <sstream>
#include <stdlib.h>
#include <numeric> // for gcd
#include <string> // for npos, find
#include "TToken.h"
#include "types.h"

#include <iostream>

/** Size of buffer for displaying a TTokenValue string. */
#define TTOKENVALUE_BUFFER_SIZE 32

std::string TTokenValue::toString(const ICalcOptions &options) {
  // Format using C++ snprintf into a C string.
  // C++20 introduces std::format.
  // See also https://stackoverflow.com/a/21162120/13253316.
  char buffer[TTOKENVALUE_BUFFER_SIZE];

std::cout << "fixedDecimals=" << options.fixedDecimals << std::endl;

  if (options.fixedDecimals >= 0 && options.fixedDecimals < 12) {
    char fmt[8];
    snprintf(fmt, 8, "%%.%df", options.fixedDecimals);
    snprintf(buffer, TTOKENVALUE_BUFFER_SIZE, fmt, m_value);
  } else {
    snprintf(buffer, TTOKENVALUE_BUFFER_SIZE, "%.15g", m_value);
  }
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
      decimal = std::string::npos;
    }
  }

  if (options.fixedDecimals > 0) {
    if (decimal == std::string::npos) {
      decimal = str.length();
      str += ".";
    }
std::cout << "decimal=" << decimal << " strlen=" << str.length() << std::endl;
    for (int k = options.fixedDecimals - (str.length() - decimal); k >= 0; --k) {
      str += "0";
    }
  }

  if (options.deciSep != 0x00 && decimal != std::string::npos) {
      // Substitute decimal character.
      str[decimal] = options.deciSep;
  }

  if (options.thouSep != 0x00) {
    int16_t thou = decimal == std::string::npos ? str.length() : decimal;
    for (thou -= 3; thou >= 1; thou -= 3) {
      str.insert(thou, std::string(1, options.thouSep));
    }
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

std::string TTokenFraction::toString(const ICalcOptions &options)
{
    char buffer[TTOKENVALUE_BUFFER_SIZE];
    if (m_whole) {
        snprintf(buffer, TTOKENVALUE_BUFFER_SIZE, "%s%i_%i_%i", m_negative ? "-" : "", m_whole, m_num, m_denom);
    } else {
        snprintf(buffer, TTOKENVALUE_BUFFER_SIZE, "%s%i_%i", m_negative ? "-" : "", m_num, m_denom);
    }
    return std::string(buffer);
}

std::string TTokenBinaryOp::toString(const ICalcOptions &options) {
    std::string str = std::string(
        m_action == eBinaryOpAction::BINARY_OP_ADD ? "+" :
        m_action == eBinaryOpAction::BINARY_OP_MULT ? "x" :
        m_action == eBinaryOpAction::BINARY_OP_MULTNEG ? " x" :
        m_action == eBinaryOpAction::BINARY_OP_SUB ? "-" :
        // m_action == eBinaryOpAction::BINARY_OP_DIV ? "÷" :
        m_action == eBinaryOpAction::BINARY_OP_DIV ? "/" :
        m_action == eBinaryOpAction::BINARY_OP_POW ? "^" :
        m_action == eBinaryOpAction::BINARY_OP_ROOT ? "V" :
        "?");
    if (m_asPercent) {
        str += "%";
    } else if (m_asTax) {
        str += "†";
    }
    return " " + str + " ";
}
