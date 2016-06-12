#pragma once
#include "Commands/BaseCommand.h"

class SpriteBaseExCommand : public BaseCommand
{
public:
	SpriteBaseExCommand(CommandEnv* commandEnv, QString spritePath);
	virtual void redo() override;
	virtual void undo() override;

protected:
	virtual void action() = 0;
	QString spritePath;

private:

};
