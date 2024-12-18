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
    m_lastErrorLocation = 0;
    m_lastError = ERROR_NONE;
    reset_pvoEquation();
    int iBrktOff = 0;
    eLookFor uLookFor = LOOKFOR_NUMBER;
    std::vector<TTokenBinaryOp*> isOps;


    size_t n = m_scan.size();
    for (size_t iThisPt = 0; iThisPt < n; iThisPt++) {
        TScan* scan = m_scan[iThisPt];
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
                } else if (scanType == SCAN_OPEN) {
                    iBrktOff += 1;
                    uLookFor = LOOKFOR_NUMBER;
                } else {
                    return errorParsing(iThisPt, PARSE_NUMBER_EXPECTED);
                }
                break;

            case LOOKFOR_BINARYOP:
                if (scanType == SCAN_BINARYOP) {
                    TTokenBinaryOp* binaryToken = ((TScanBinaryOp*)scan)->toToken(iBrktOff);
                    processOps(binaryToken, m_pvoEquation, isOps);
                    isOps.push_back(binaryToken);
                    uLookFor = LOOKFOR_NUMBER;
                } else if (scanType == SCAN_CLOSE) {
                    iBrktOff -= 1;
                    if (iBrktOff < 0) {
                        return errorParsing(iThisPt, PARSE_MISSING_OPEN);
                    }
                    uLookFor = LOOKFOR_BINARYOP;
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

}

