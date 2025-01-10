#ifndef ELOOKFOR_H
#define ELOOKFOR_H

/** Type of token being scanned during parsing. */
enum eLookFor {
    /** number, unary op, parentheses, negative, constant */
    LOOKFOR_NUMBER,
    /** binary op only */
    LOOKFOR_BINARYOP,
    /** brackets only (after unary op) */
    LOOKFOR_BRACKET,
};

#define LOOKFOR_TO_STRING(uLookFor) ( \
    uLookFor == LOOKFOR_NUMBER ? "NUMBER" : \
    uLookFor == LOOKFOR_BINARYOP ? "BINARYOP" : \
    uLookFor == LOOKFOR_BRACKET ? "BRACKET" : \
    "?" \
)

#endif /* ELOOKFOR_H */