#include "TScan.h"

bool TScanNumber::toFractionParts(int *pwhole, int *pnum, int *pdenom)
{
    *pwhole = *pnum = 0;
    *pdenom = 1;
    if (m_fractionParts < 1) {
        return false;
    }

    /** Copy of string to be tokenized. */
    std::string copy = std::string(m_tok);
    /** Number of characters remaining in string. */
    size_t rem = copy.length();

    size_t index = copy.find('_');
    int v0 = std::stoi(copy.substr(0, index));
    rem -= index + 1;
    copy.erase(0, index + 1);
    if (m_fractionParts == 1) {
        *pnum = v0;
        *pdenom = rem <= 0 ? 1 : std::stoi(copy);
        return true;
    }

    index = copy.find('_');
    int v1 = std::stoi(copy.substr(0, index));
    rem -= index + 1;
    copy.erase(0, index + 1);
    *pwhole = v0;
    *pnum = v1;
    *pdenom = rem <= 0 ? 1 : std::stoi(copy);
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
        snprintf(buffer, 32, "%i_%i/%i", whole, num, denom);
        return std::string(buffer);
    }
    return std::string(m_tok);
}
