////#include <iostream>
#include "TScan.h"

bool TScanNumber::toFractionParts(int *pwhole, int *pnum, int *pdenom)
{
  // *pwhole = 1;
  // *pnum = 2;
  // *pdenom = 3;
  // return true;

    *pwhole = *pnum = *pdenom = 0;

    if (m_fractionParts < 1) {
        return false;
    }

    /** Copy of string to be tokenized. */
    std::string copy = std::string(m_tok);
    /** Number of characters remaining in string. */
    size_t rem = copy.length();

    int k = 0;
    int parts[3] = { 0, 0, 0 };
    ////std::cout << "Scanning fractionParts=" << m_fractionParts << " copy='" << copy << "' rem=" << rem << std::endl;
    for (; k <= m_fractionParts && rem > 0; ++k) {
      ////std::cout << "Loop k=" << k;
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
        ////std::cout << " index=" << index << " parts[k]=" << parts[k] << " rem=" << rem << " copy='" << copy << "'" << std::endl;
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

    ////std::cout << "Returning " << *pwhole << " " << *pnum << "/" << *pdenom << std::endl;

    return true;
}

void TScanNumber::append(const char ch)
{
    if (ch == '.' && m_fractionParts == 0 && !m_hasDecimal) {
        m_tok += ch;
        m_hasDecimal = true;
    } else if (ch == '_' && m_fractionParts < 2 && !m_hasDecimal) {
        m_tok += ch;
        m_fractionParts += 1;
    } else if ('0' <= ch && ch <= '9') {
        m_tok += ch;
    }
}

TToken *TScanNumber::toToken()
{
    int whole, num, denom;
    if (toFractionParts(&whole, &num, &denom)) {
        return new TTokenFraction(whole, num, denom);
    }

    return new TTokenValue(std::stod(m_tok));

    std::string copy = std::string(m_tok);
    size_t index = copy.find('_');
    int v0 = std::stoi(copy.substr(0, index));
    copy.erase(0, index + 1);
    if (m_fractionParts == 1) {
        return new TTokenFraction(0, v0, std::stoi(copy));
    }

    index = copy.find('_');
    int v1 = std::stoi(copy.substr(0, index));
    copy.erase(0, index + 1);
    int v2 = std::stoi(copy);

    return new TTokenFraction(v0, v1, v2);
}

std::string TScanNumber::toString()
{
    int whole, num, denom;
    if (toFractionParts(&whole, &num, &denom)) {
        char buffer[32];
        if (whole == 0) {
            snprintf(buffer, 32, "%i/%i", num, denom);
        } else {
            snprintf(buffer, 32, "%i_%i/%i", whole, num, denom);
        }
        return std::string(buffer);
    }
    return std::string(m_tok);
}
