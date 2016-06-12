#pragma once
#include "BaseCommand.h"

class CompositionBaseCommand : public BaseCommand
{
public:
	CompositionBaseCommand(CommandEnv* commandEnv, QString spritePath, int frameIndex);
	virtual ~CompositionBaseCommand();

	void redo() override;
	void undo() override;

protected:
	virtual void action() = 0;
	QString spritePath;
	int frameIndex;

private:
	Frame* frameBackup = nullptr;
};
