#pragma once
#include "AnimationBaseCommand.h"

class AnimationChangeTagCommand : public AnimationBaseCommand
{
public:
	AnimationChangeTagCommand(CommandEnv* commandEnv, QString spritePath, const QList<int>& frames, QString value);

protected:
	virtual void action() override;

private:
	QList<int> frames;
	QString value;
};
