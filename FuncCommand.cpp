#include "FuncCommand.h"

void FuncCommand::execute() {
    for (auto& command : commands) {
        command->execute();
    }
}

void FuncCommand::addCommand(Command* command) {
    commands.push_back(command);
}

FuncCommand::~FuncCommand() {
    for (vector<Command*>::iterator it = commands.begin(); it!= commands.end();it++){
        delete *it;
    }
}
