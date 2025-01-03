#include "TScan.h"
#include "eAction.h"

bool TScanNumber::toFractionParts(int *pwhole, int *pnum, int *pdenom, bool *pneg)
{
    *pwhole = *pnum = *pdenom = 0;
    *pneg = m_negative;

    if (m_fractionParts < 1) {
        return false;
    }

    /** Copy of string to be tokenized. */
    std::string copy = std::string(m_tok);
    /** Number of characters remaining in string. */
    size_t rem = copy.length();

    int k = 0;
    int parts[3] = { 0, 0, 0 };
    for (; k <= m_fractionParts && rem > 0; ++k) {
      // | Progression | m_fractionParts |
      // | ----------- | --------------- |
      // |           1 |               0 | Not parsed
      // |          1_ |               1 |
      // |         1_2 |               1 |
      // |        1_2_ |               2 |
      // |       1_2_3 |               3 |
        size_t index = copy.find(eAction::ACTION_FRACTION);
        parts[k] = std::stoi(copy.substr(0, index));
        rem -= index + 1;
        copy.erase(0, index + 1);
    }


    if (m_fractionParts == 1) {
        // "1_" or "1_2". 
        *pwhole = 0;
        *pnum = parts[0];
        *pdenom = parts[1];
    } else if (m_fractionParts == 2) {
        // "1_2_" or "1_2_3"
        *pwhole = parts[0];
        *pnum = parts[1];
        *pdenom = parts[2];
    }


    return true;
}

void TScanNumber::append(const char ch)
{
    switch (ch) {
        case eAction::ACTION_DECIMAL:
            if (m_fractionParts == 0 && !m_hasDecimal) {
                if (m_tok.length() <= 0) {
                    m_tok += "0";
                }
                m_tok += ch;
                m_hasDecimal = true;
            }
            break;
        case eAction::ACTION_FRACTION:
            if (m_fractionParts < 2 && !m_hasDecimal) {
                m_tok += ch;
                m_fractionParts += 1;
            }
            break;
        case eAction::ACTION_NEGATE:
            m_negative = !m_negative;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            m_tok += ch;
            break;
    }
}

TToken *TScanNumber::toToken()
{
    int whole, num, denom;
    bool negative;
    if (toFractionParts(&whole, &num, &denom, &negative)) {
        return new TTokenFraction(whole, num, denom, negative);
    }

    double val = m_tok.length() > 0 ? std::stod(m_tok) : 0;
    if (m_negative) {
        val = -val;
    }
    return new TTokenValue(val);
}

std::string TScanNumber::toString()
{
    int whole, num, denom;
    bool negative;
    std::string prefix = m_negative ? "-" : "";

    if (toFractionParts(&whole, &num, &denom, &negative)) {
        char buffer[32];
        if (whole == 0) {
            snprintf(buffer, 32, "%s %i/%i", negative ? "-" : "", num, denom);
        } else {
            snprintf(buffer, 32, "%s %i_%i/%i", negative ? "-" : "", whole, num, denom);
        }
        return prefix + std::string(buffer);
    }
    return prefix + std::string(m_tok);
}
