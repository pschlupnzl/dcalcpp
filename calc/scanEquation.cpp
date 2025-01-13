#include "eScanType.h"
#include "CCalculate.h"
#include "eAction.h"

bool CCalculate::scan(eAction action, bool noAutoOpen) {
    if (scanSilent(action, noAutoOpen = false)) {
        m_actions.push_back(action);
        return true;
    };
    return false;
}

bool CCalculate::scanSilent(eAction action, bool noAutoOpen) {
    IScan* last = m_scan.size() <= 0 
        ? nullptr 
        : m_scan.back();
    eScanType type = last ? last->type() : eScanType::SCAN_UNDEFINED;

    switch (action) {
        case eAction::ACTION_NUM0:
        case eAction::ACTION_NUM1:
        case eAction::ACTION_NUM2:
        case eAction::ACTION_NUM3:
        case eAction::ACTION_NUM4:
        case eAction::ACTION_NUM5:
        case eAction::ACTION_NUM6:
        case eAction::ACTION_NUM7:
        case eAction::ACTION_NUM8:
        case eAction::ACTION_NUM9:
        case eAction::ACTION_DECIMAL:
        case eAction::ACTION_FRACTION:
        case eAction::ACTION_NEGATE:
            if (!last || type != eScanType::SCAN_NUMBER) {
                last = new TScanNumber();
                m_scan.push_back(last);
            }
            ((TScanNumber*) last)->append(action);
            break;

        case eAction::ACTION_ADD:
        case eAction::ACTION_MULT:
        case eAction::ACTION_SUB:
        case eAction::ACTION_DIV:
        case eAction::ACTION_POW:
        case eAction::ACTION_ROOT:
            if (
                type == eScanType::SCAN_NUMBER || 
                type == eScanType::SCAN_CLOSE ||
                type == eScanType::SCAN_POSTUNARYOP
            ) {
                m_scan.push_back(new TScanBinaryOp(
                    action == eAction::ACTION_ADD ? eBinaryOpAction::BINARY_OP_ADD :
                    action == eAction::ACTION_MULT ? eBinaryOpAction::BINARY_OP_MULT :
                    action == eAction::ACTION_SUB ? eBinaryOpAction::BINARY_OP_SUB :
                    action == eAction::ACTION_DIV ? eBinaryOpAction::BINARY_OP_DIV :
                    action == eAction::ACTION_POW ? eBinaryOpAction::BINARY_OP_POW :
                    action == eAction::ACTION_ROOT ? eBinaryOpAction::BINARY_OP_ROOT :
                    eBinaryOpAction::BINARY_OP_ADD
                ));
            } else {
                return false;
            }
            break;

        case eAction::ACTION_OPEN:
        case eAction::ACTION_OPEN_AUTO:
            if (!last ||
                type == eScanType::SCAN_BINARYOP ||
                type == eScanType::SCAN_UNARYOP ||
                type == eScanType::SCAN_OPEN
            ) {
                m_scan.push_back(new TScanOpen());
            } else {
                return false;
            }
            break;

        case eAction::ACTION_CLOSE:
            if (type == eScanType::SCAN_NUMBER ||
                type == eScanType::SCAN_CLOSE
            ) {
                m_scan.push_back(new TScanClose());
            } else {
                return false;
            }
            break;

        case eAction::ACTION_SQRT:
        case eAction::ACTION_SIN:
        case eAction::ACTION_COS:
        case eAction::ACTION_TAN:
        case eAction::ACTION_ASIN:
        case eAction::ACTION_ACOS:
        case eAction::ACTION_ATAN:
            if (!last ||
                type == eScanType::SCAN_OPEN ||
                type == eScanType::SCAN_BINARYOP) {
                m_scan.push_back(new TScanUnaryOp(
                    action == eAction::ACTION_SQRT ? eUnaryOpAction::UNARY_OP_SQRT :
                    action == eAction::ACTION_SIN ? eUnaryOpAction::UNARY_OP_SIN :
                    action == eAction::ACTION_COS ? eUnaryOpAction::UNARY_OP_COS :
                    action == eAction::ACTION_TAN ? eUnaryOpAction::UNARY_OP_TAN :
                    action == eAction::ACTION_ASIN ? eUnaryOpAction::UNARY_OP_ASIN :
                    action == eAction::ACTION_ACOS ? eUnaryOpAction::UNARY_OP_ACOS :
                    action == eAction::ACTION_ATAN ? eUnaryOpAction::UNARY_OP_ATAN :
                    eUnaryOpAction::UNARY_OP_SQRT
                ));
            } else {
                return false;
            }
            break;

        case eAction::ACTION_POW2:
            if (type == eScanType::SCAN_NUMBER || type == eScanType::SCAN_CLOSE) {
                m_scan.push_back(new TScanPostUnaryOp(
                    ePostUnaryOpAction::POST_UNARY_OP_POW2
                ));
            } else {
                return false;
            }
            break;

        default:
            // All other characters not scanned.
            return false;
    }

    // Auto-open parenthesis after unary operator.
    if (!noAutoOpen && (
        action == eAction::ACTION_POW ||
        action == eAction::ACTION_ROOT ||
        action == eAction::ACTION_SQRT ||
        action == eAction::ACTION_SIN ||
        action == eAction::ACTION_COS ||
        action == eAction::ACTION_TAN ||
        action == eAction::ACTION_ASIN ||
        action == eAction::ACTION_ACOS ||
        action == eAction::ACTION_ATAN
    )) {
        m_scan.push_back(new TScanOpen());
    }
    return true;
}

void CCalculate::backspace() {
    if (m_actions.size() > 0) {
        eAction action = m_actions.back();
        m_actions.pop_back();

        // Remove auto-inserted unary operator with parentheses.
        if (action == eAction::ACTION_OPEN_AUTO && m_actions.size() > 0) {
            m_actions.pop_back();
        }
        reset_scan();
        for (eAction action : m_actions) {
            scanSilent(action);
        }
    }
}