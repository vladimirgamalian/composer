#pragma once
#include "BaseCommand.h"

class SpritesDeleteNodeCommand : public BaseCommand
{
public:
	SpritesDeleteNodeCommand(CommandEnv* commandEnv, QString nodePath);
	virtual ~SpritesDeleteNodeCommand();
	virtual void redo() override;
	virtual void undo() override;

private:
	QString nodePath;
	TreeNode* deletedNode = nullptr;
};
