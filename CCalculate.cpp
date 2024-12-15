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
