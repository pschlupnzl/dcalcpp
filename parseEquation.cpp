#include <iostream>
#include <string>
#include "eLookFor.h"
#include "CCalculate.h"

/**
 * Process the next operator, evaluating higher precedence operators
 * before adding the new one.
 * @param iThisOp Current operator to be added.
 */
void processOps (
  int iThisOp,
  std::vector<TToken*> &pvoEquation,
  std::vector<TTokenBinaryOp*> &isOps
) {
    /** Previous operator on stack. */
    TTokenBinaryOp* iPrevOp;

    for (; isOps.size() > 0; ) {
        iPrevOp = isOps.back();
        if (iPrevOp->op() >= iThisOp) {
            // Add to RPN stack.
            isOps.pop_back();
            pvoEquation.push_back(iPrevOp);
            //// std::cout << "Push token " << iPrevOp->toString() << std::endl;
        } else {
            // Exit on lower precedence.
            break;
        }
    }
}

void processOps(
    TTokenBinaryOp* iThisOp,
    std::vector<TToken*> &pvoEquation,
    std::vector<TTokenBinaryOp*> &isOps
) {
    return processOps(iThisOp->op(), pvoEquation, isOps);
}

/**
 * Parse the equation tokens into the RPN stack. This takes into account
 * operator precedence and such.
 */
void CCalculate::parseEquation() {
    std::cout << "parseEquation" << std::endl;

    std::vector<TTokenBinaryOp*> isOps;

    m_lastErrorLocation = 0;
    m_lastError = ERROR_NONE;
    reset_pvoEquation();
    int iBrktOff = 0;
    eLookFor uLookFor = LOOKFOR_NUMBER;

    size_t n = m_scan.size();
    for (size_t iThisPt = 0; iThisPt < n; iThisPt++) {
        TScan* scan = m_scan[iThisPt];
        std::cout << iThisPt << ": Look for " << uLookFor << " Token is " << scan->toString() << "  |";
        eScanType scanType = scan->type();
        switch (uLookFor) {
            case LOOKFOR_NUMBER:
                if (
                    scanType == SCAN_NUMBER
                    // token.typ === eTokenType.FRACTION ||
                    // token.typ === eTokenType.TIME ||
                    // token.typ === eTokenType.ANGLE ||
                    // token.typ === eTokenType.CONSTANT
                ) {
                    m_pvoEquation.push_back(((TScanNumber*)scan)->toToken());
                    uLookFor = LOOKFOR_BINARYOP;
                    //// std::cout << "Found number " << token->toString() << std::endl;
                } else {
                    return errorParsing(iThisPt, PARSE_NUMBER_EXPECTED);
                }
                break;

            case LOOKFOR_BINARYOP:
                if (scanType == SCAN_BINARYOP) {
                // TTokenBinaryOp* binaryToken = dynamic_cast<TTokenBinaryOp*>(token);
                // if (binaryToken != nullptr) { // tokenType  == TOKEN_BINARYOP) {
                    TTokenBinaryOp* binaryToken = ((TScanBinaryOp*)scan)->toToken(iBrktOff);
                    // binaryToken->setPrecedence(iBrktOff);

                    processOps(binaryToken, m_pvoEquation, isOps);

                    isOps.push_back(binaryToken);
                    uLookFor = LOOKFOR_NUMBER;
                    std::cout << "Found binary op " << scan->toString() << std::endl;
                } else {
                    return errorParsing(iThisPt, PARSE_BINARYOP_EXPECTED);
                }
                break;
        } // end switch
    } // end for

    if (iBrktOff > 0) {
        // NOP - Automatically close brackets.
    } else if (uLookFor == LOOKFOR_NUMBER) {
        return errorParsing(n, PARSE_NUMBER_EXPECTED);
    }

    processOps(-1, m_pvoEquation, isOps);

    std::cout << "Parse success" << std::endl;
}

