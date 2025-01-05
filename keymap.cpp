#include "keymap.h"

eAction actionFromKeyboard(const char ch) {
    return 
        ch == '0' ? eAction::ACTION_NUM0 :
        ch == '1' ? eAction::ACTION_NUM1 :
        ch == '2' ? eAction::ACTION_NUM2 :
        ch == '3' ? eAction::ACTION_NUM3 :
        ch == '4' ? eAction::ACTION_NUM4 :
        ch == '5' ? eAction::ACTION_NUM5 :
        ch == '6' ? eAction::ACTION_NUM6 :
        ch == '7' ? eAction::ACTION_NUM7 :
        ch == '8' ? eAction::ACTION_NUM8 :
        ch == '9' ? eAction::ACTION_NUM9 :
        ch == '.' ? eAction::ACTION_DECIMAL :
        ch == '~' ? eAction::ACTION_NEGATE :
        ch == '_' ? eAction::ACTION_FRACTION :
        ch == '*' ? eAction::ACTION_MULT :
        ch == '/' ? eAction::ACTION_DIV :
        ch == '+' ? eAction::ACTION_ADD :
        ch == '-' ? eAction::ACTION_SUB :
        ch == '^' ? eAction::ACTION_POW :
        ch == 'v' ? eAction::ACTION_ROOT :
        ch == '(' ? eAction::ACTION_OPEN :
        ch == ')' ? eAction::ACTION_CLOSE :
        eAction::ACTION_UNDEFINED;
}