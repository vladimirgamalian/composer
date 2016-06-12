#pragma once
#include "CompositionBaseCommand.h"

class CompositionDragDropCommand : public CompositionBaseCommand
{
public:
	CompositionDragDropCommand(CommandEnv* commandEnv, QString spritePath, int frameIndex,
		const QList<int>& indexes, int row, bool copyAction);

protected:
	virtual void action();

private:
	QList<int> indexes;
	int row;
	bool copyAction;
};
