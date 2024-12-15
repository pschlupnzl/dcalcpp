#include <iostream>
#include <string>
#include "CCalculate.h"
#include "TScan.h"

/**
 * Reset the given array, deleting all items referenced by pointers in the
 * array. This is useful with arrays where items are added with
 * 
 *     `arr.push_back(new Token(...));`
 */
template<class T>
void reset_array(std::vector<T*> arr) {
    while (arr.size() > 0) {
        T* item = arr.back();
        arr.pop_back();
        delete item;
    }
    arr.clear();
}

void CCalculate::reset() {
    reset_scan();
    reset_pvoEquation();
    reset_result();
}

void CCalculate::reset_scan() {
    reset_array(m_scan);
}

void CCalculate::reset_pvoEquation() {
    reset_array(m_pvoEquation);
}

void CCalculate::reset_result() {
    if (m_presult != nullptr) {
        delete m_presult;
        m_presult = nullptr;
    }
}

// void CCalculate::pushToken(TToken *tok)
// {
//     m_tokens.push_back(tok);
// }

std::string CCalculate::toString() {
    std::string str;
    str += "Lexed:\n ";
    for (TScan* scan : m_scan) {
        str += "‹" + scan->toString() + "› ";
    }
    str += "\n";

    str += "Stack:\n ";
    for (TToken* tok : m_pvoEquation) {
        str += "«" + tok->toString() + "» ";
    }
    str += "\n";

    if (m_presult != nullptr) {
        str += "= " + m_presult->toString() + "\n";
    }

    return str;
}

void CCalculate::scan(const char ch) {
    TScan* last = m_scan.size() <= 0 
        ? nullptr 
        : m_scan.back();
    eScanType type = last ? last->type() : SCAN_UNDEFINED;

    switch (ch) {
        case ' ':
            // Ignore whitespace.
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
        case '.':
            if (!last || type != SCAN_NUMBER) {
                last = new TScanNumber();
                m_scan.push_back(last);
            }
            ((TScanNumber*) last)->append(ch);
            break;

        case '+':
        case '*':
        case '-':
        case '/':
            if (type == SCAN_NUMBER) {
                m_scan.push_back(new TScanBinaryOp(
                    ch == '+' ? BINARY_OP_ACTION_ADD :
                    ch == '*' ? BINARY_OP_ACTION_MULT :
                    ch == '-' ? BINARY_OP_ACTION_SUB :
                    ch == '/' ? BINARY_OP_ACTION_DIV :
                    BINARY_OP_ACTION_ADD
                ));
            }
            break;
    }    
}

