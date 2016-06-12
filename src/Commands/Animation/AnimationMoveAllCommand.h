#pragma once
#include "AnimationBaseCommand.h"

class AnimationMoveAllCommand : public AnimationBaseCommand
{
public:
	AnimationMoveAllCommand(CommandEnv* commandEnv, QString spritePath,
		int shiftX, int shiftY);

protected:
	virtual void action() override;

private:
	int shiftX;
	int shiftY;
};
