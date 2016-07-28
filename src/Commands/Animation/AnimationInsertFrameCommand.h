#pragma once
#include "AnimationBaseCommand.h"

class AnimationInsertFrameCommand : public AnimationBaseCommand
{
public:
	AnimationInsertFrameCommand(CommandEnv* commandEnv, QString spritePath, int index,
		bool before, bool copyContent);

protected:
	virtual void action() override;

private:
	int index;
	bool before;
	bool copyContent;
};
