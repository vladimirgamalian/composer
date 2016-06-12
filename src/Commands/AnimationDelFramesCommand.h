#pragma once
#include "AnimationBaseCommand.h"

class AnimationDelFramesCommand : public AnimationBaseCommand
{
public:
	AnimationDelFramesCommand(CommandEnv* commandEnv, QString spritePath, const QList<int>& frames);

protected:
	virtual void action() override;

private:
	QList<int> frames;
};
