#pragma once
#include "AnimationBaseCommand.h"

class AnimationChangeFrameDurationCommand : public AnimationBaseCommand
{
public:
	AnimationChangeFrameDurationCommand(CommandEnv* commandEnv, QString spritePath, const QList<int>& frames, int value);

protected:
	virtual void action() override;

private:
	QList<int> frames;
	int value;
};
