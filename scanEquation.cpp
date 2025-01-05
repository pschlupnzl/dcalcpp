#include "eScanType.h"
#include "CCalculate.h"
#include "eAction.h"

void CCalculate::scan(eAction action) {
    if (scanSilent(action)) {
        m_actions.push_back(action);
    };
}

bool CCalculate::scanSilent(eAction action) {
    TScan* last = m_scan.size() <= 0 
        ? nullptr 
        : m_scan.back();
    eScanType type = last ? last->type() : SCAN_UNDEFINED;

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
            if (!last || type != SCAN_NUMBER) {
                last = new TScanNumber();
                m_scan.push_back(last);
            }
            ((TScanNumber*) last)->append(action);

            break;

        case eAction::ACTION_ADD:
        case eAction::ACTION_MULT:
        case eAction::ACTION_SUB:
        case eAction::ACTION_DIV:
            if (type == SCAN_NUMBER || type == SCAN_CLOSE) {
                m_scan.push_back(new TScanBinaryOp(
                    action == eAction::ACTION_ADD ? BINARY_OP_ACTION_ADD :
                    action == eAction::ACTION_MULT ? BINARY_OP_ACTION_MULT :
                    action == eAction::ACTION_SUB ? BINARY_OP_ACTION_SUB :
                    action == eAction::ACTION_DIV ? BINARY_OP_ACTION_DIV :
                    BINARY_OP_ACTION_ADD
                ));
            }
            break;

        case eAction::ACTION_OPEN:
            m_scan.push_back(new TScanOpen());
            break;

        case eAction::ACTION_CLOSE:
            m_scan.push_back(new TScanClose());
            break;
        default:
            // All other characters not scanned.
            return false;
    }
    return true;
}

void CCalculate::backspace() {
    if (m_actions.size() > 0) {
        m_actions.pop_back();
        reset_scan();
        for (eAction action : m_actions) {
            scanSilent(action);
        }
    }
}