#ifndef PROJECT_DEFINEVARCOMMAND_H
#define PROJECT_DEFINEVARCOMMAND_H

#include "Command.h"

class DefineVarCommand : Command {
public:
    virtual double getValue() = 0;
};

#endif //PROJECT_DEFINEVARCOMMAND_H