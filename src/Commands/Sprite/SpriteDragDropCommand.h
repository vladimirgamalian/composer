#pragma once
#include "Commands/BaseCommand.h"

class SpriteDragDropCommand : public BaseCommand
{
public:
	SpriteDragDropCommand(CommandEnv* commandEnv,
		QString oldPath, QString newPath, bool copy);
	virtual void redo() override;
	virtual void undo() override;

private:
	QString oldPath;
	QString newPath;
	int row;
	bool copy;
};
