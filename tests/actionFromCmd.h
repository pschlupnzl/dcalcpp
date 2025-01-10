#ifndef ACTIONFROMCMD_H
#define ACTIONFROMCMD_H

#include <string>
#include "../calc/eAction.h"

/**
 * Convert a command string (such as "+" or "sqrt") into the corresponding
 * action.
 */
eAction actionFromCmd(std::string cmd);

#endif /* ACTIONFROMCMD_H */