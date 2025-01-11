#ifndef KEYMAP_H
#define KEYMAP_H

#include "eAction.h"

eAction actionFromKeyboard(char ch);
eCommand commandFromKeyboard(char ch);

#endif /* KEYMAP_H */