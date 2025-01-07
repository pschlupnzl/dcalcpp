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
        size_t index = copy.find('_');
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

void TScanNumber::append(eAction action)
{
    switch (action) {
        case eAction::ACTION_DECIMAL:
            if (m_fractionParts == 0 && !m_hasDecimal) {
                if (m_tok.length() <= 0) {
                    m_tok += "0";
                }
                m_tok += ".";
                m_hasDecimal = true;
            }
            break;
        case eAction::ACTION_FRACTION:
            if (m_fractionParts < 2 && !m_hasDecimal) {
                m_tok += "_";
                m_fractionParts += 1;
            }
            break;
        case eAction::ACTION_NEGATE:
            m_negative = !m_negative;
            break;
        case eAction::ACTION_NUM0: m_tok += "0"; break;
        case eAction::ACTION_NUM1: m_tok += "1"; break;
        case eAction::ACTION_NUM2: m_tok += "2"; break;
        case eAction::ACTION_NUM3: m_tok += "3"; break;
        case eAction::ACTION_NUM4: m_tok += "4"; break;
        case eAction::ACTION_NUM5: m_tok += "5"; break;
        case eAction::ACTION_NUM6: m_tok += "6"; break;
        case eAction::ACTION_NUM7: m_tok += "7"; break;
        case eAction::ACTION_NUM8: m_tok += "8"; break;
        case eAction::ACTION_NUM9: m_tok += "9"; break;
    }
}

ITokenResultBase *TScanNumber::toToken()
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
