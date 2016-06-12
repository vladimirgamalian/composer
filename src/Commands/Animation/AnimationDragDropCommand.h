#pragma once
#include "AnimationBaseCommand.h"

class AnimationDragDropCommand : public AnimationBaseCommand
{
public:
	AnimationDragDropCommand(CommandEnv* commandEnv, QString spritePath,
		const QList<int>& indexes, int row, bool copyAction);

protected:
	virtual void action() override;

private:
	QList<int> indexes;
	int row;
	bool copyAction;
};
