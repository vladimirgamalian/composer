#pragma once
#include "AnimationBaseCommand.h"

class AnimationCompressCommand : public AnimationBaseCommand
{
public:
	AnimationCompressCommand(CommandEnv* commandEnv, QString spritePath, const QList<int>& indexes);

protected:
	virtual void action() override;

private:
	QList<int> indexes;
};
