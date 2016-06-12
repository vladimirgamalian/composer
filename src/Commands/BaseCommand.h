#pragma once
#include "CommandEnv.h"

class BaseCommand : public QUndoCommand
{
public:
	BaseCommand(CommandEnv* commandEnv) : commandEnv(commandEnv) {}
	virtual ~BaseCommand() 
	{
		delete commandEnv;
	}

protected:
	CommandEnv* commandEnv = nullptr;

private:

};
