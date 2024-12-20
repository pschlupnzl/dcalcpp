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
void reset_array(std::vector<T*>& arr) {
    // TODO:
    // I'm not sure whether I am responsible for deleting the (T*) item.
    // Doing so triggers a segfault on Arduino / ESP32, but without it I'm not
    // sure the memory gets freed properly.
    // I tried making m_scan and m_pvoEquation of type std::vector<TToken&>
    // but that didn't work either.
    // I'm just gonna run with this for now.
    while (arr.size() > 0) {
        T* item = arr.back();
        arr.pop_back();
        delete item;
    }
    // arr.clear();
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

std::string CCalculate::toString()
{
    std::string str;
    str += "Scan:\n ";
    for (TScan* scan : m_scan) {
        str += "‹" + scan->toString() + "› ";
    }
    str += "\n";

    str += "Tokens:\n ";
    for (TToken* tok : m_pvoEquation) {
        str += "«" + tok->toString() + "» ";
    }
    str += "\n";

    if (m_presult != nullptr) {
        str += "= " + m_presult->toString() + "\n";
    }

    return str;
}

void CCalculate::forEach(const std::function<void(TScan *)> fn)
{
    for (TScan* scan : m_scan) {
        fn(scan);
    }
}

