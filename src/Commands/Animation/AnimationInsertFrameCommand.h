#pragma once
#include "AnimationBaseCommand.h"

class AnimationInsertFrameCommand : public AnimationBaseCommand
{
public:
	AnimationInsertFrameCommand(CommandEnv* commandEnv, QString spritePath, int index,
		bool before, bool copyContent);

	int getNewIndex() const
	{
		return newIndex;
	}

protected:
	virtual void action() override;

private:
	int index;
	bool before;
	bool copyContent;
	int newIndex;
};
