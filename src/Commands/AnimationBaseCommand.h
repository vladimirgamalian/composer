#pragma once
#include "BaseCommand.h"

class AnimationBaseCommand : public BaseCommand
{
public:
	AnimationBaseCommand(CommandEnv* commandEnv, QString spritePath);
	virtual ~AnimationBaseCommand();

	void redo() override;
	void undo() override;

protected:
	virtual void action() = 0;
	QString spritePath;

private:
	Sprite* spriteBackup = nullptr;
};
