#include "keymap.h"

/*
Mapping QWERTY keyboard to DysCalculator Original

+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+--------+
|  ±  |     |  x² |     |     |     |  xʸ |     |     |  (  |  )  | ˣ⁄y |     |        |
|     |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |     |   ⌫    |
|  ~  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |  DEL   |
+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+--------+
|        |     |     |     |dg/rd|thSep|     |     |sin⁻¹|cos⁻¹|tan⁻¹|     |     |     |
|        |     |     |     |     |     |     |     | sin | cos | tan |     |     |     |
|  TAB   |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |  \  |
+--------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
|          |     |     |dSep | dp  |     |     |     |     |     |     |     |         |
|          |     |     |     |     |     |     |     |     |     |     |     |         |
| CAPSLOCK |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  | RETURN  |
+----------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+---------------+
|             |     |     | AC  |  √x |     |     |     |     |     |     |            |
|             |     |     |     | ˣ√y |     |     |     |     |  .  |  /  |            |
|   SHIFT     |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |   SHIFT    |
+-------------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+------------+


DysCalculator original
+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
|  ~  |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  .  | 
|  ±  |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  .  | 
+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
|  _  |  (  |  )  |  v  |  @  |    /    |    *    |    +     |    -     |
| x/y |  (  |  )  |  √  |  ²  |    ÷    |    ×    |    +     |    -     |
+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
|     |     |     |     |     |     |     |     |  i  |  o  |  p  |     | 
|  ⚙️  |  ▲  |  %  |  °  |  hr | min | sec |am/pm| sin | cos | tan |  =  | 
+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
*/

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
        ch == '@' ? eAction::ACTION_POW2 :
        ch == '^' ? eAction::ACTION_POW :
        ch == 'V' ? eAction::ACTION_SQRT : 
        ch == 'v' ? eAction::ACTION_ROOT :
        ch == '(' ? eAction::ACTION_OPEN :
        ch == ')' ? eAction::ACTION_CLOSE :
        ch == 'i' ? eAction::ACTION_SIN :
        ch == 'o' ? eAction::ACTION_COS :
        ch == 'p' ? eAction::ACTION_TAN :
        ch == 'I' ? eAction::ACTION_ASIN :
        ch == 'O' ? eAction::ACTION_ACOS :
        ch == 'P' ? eAction::ACTION_ATAN :
        eAction::ACTION_UNDEFINED;
}

#define CHAR_BACKSPACE 0x08
#define CHAR_DELETE 0x7F
#define CHAR_ESCAPE 0x1B

eCommand commandFromKeyboard(const char ch) {
    return
        ch == 'q' ? eCommand::CMD_QUIT :
        ch == CHAR_ESCAPE ? eCommand::CMD_AC :
        ch == 'C' ? eCommand::CMD_AC :
        ch == CHAR_BACKSPACE ? eCommand::CMD_DEL :
        ch == CHAR_DELETE ? eCommand::CMD_DEL :
        ch == 'R' ? eCommand::CMD_TRIGRAD :
        ch == 'D' ? eCommand::CMD_DECISEP :
        ch == 'T' ? eCommand::CMD_THOUSEP :
        ch == 'F' ? eCommand::CMD_FIXEDDECIMALS :
        CMD_UNDEFINED;
}