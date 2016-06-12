#pragma once
#include "AnimationBaseCommand.h"

class AnimationCompressCommand : public AnimationBaseCommand
{
public:
	AnimationCompressCommand(CommandEnv* commandEnv, QString spritePath);

protected:
	virtual void action() override;
};
