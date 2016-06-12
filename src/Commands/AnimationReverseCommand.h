#pragma once
#include "AnimationBaseCommand.h"

class AnimationReverseCommand : public AnimationBaseCommand
{
public:
	AnimationReverseCommand(CommandEnv* commandEnv, QString spritePath,	const QList<int>& indexes);

protected:
	virtual void action() override;

private:
	QList<int> indexes;
};
