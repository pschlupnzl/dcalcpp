#include "actionFromCmd.h"

eAction actionFromCmd(std::string cmd)
{
    return 
        cmd == "0" ? eAction::ACTION_NUM0 :
        cmd == "1" ? eAction::ACTION_NUM1 :
        cmd == "2" ? eAction::ACTION_NUM2 :
        cmd == "3" ? eAction::ACTION_NUM3 :
        cmd == "4" ? eAction::ACTION_NUM4 :
        cmd == "5" ? eAction::ACTION_NUM5 :
        cmd == "6" ? eAction::ACTION_NUM6 :
        cmd == "7" ? eAction::ACTION_NUM7 :
        cmd == "8" ? eAction::ACTION_NUM8 :
        cmd == "9" ? eAction::ACTION_NUM9 :
        cmd == "." ? eAction::ACTION_DECIMAL :
        cmd == "±" ? eAction::ACTION_NEGATE :
        cmd == "_" ? eAction::ACTION_FRACTION :
        cmd == "*" ? eAction::ACTION_MULT :
        cmd == "/" ? eAction::ACTION_DIV :
        cmd == "+" ? eAction::ACTION_ADD :
        cmd == "-" ? eAction::ACTION_SUB :
        cmd == "^2" ? eAction::ACTION_POW2 :
        cmd == "^" ? eAction::ACTION_POW :
        cmd == "sqrt" ? eAction::ACTION_SQRT : 
        cmd == "v" ? eAction::ACTION_ROOT :
        cmd == "(" ? eAction::ACTION_OPEN :
        cmd == ")" ? eAction::ACTION_CLOSE :
        cmd == "sin" ? eAction::ACTION_SIN :
        cmd == "cos" ? eAction::ACTION_COS :
        cmd == "tan" ? eAction::ACTION_TAN :
        cmd == "asin" ? eAction::ACTION_ASIN :
        cmd == "acos" ? eAction::ACTION_ACOS :
        cmd == "atan" ? eAction::ACTION_ATAN :
        eAction::ACTION_UNDEFINED;
}