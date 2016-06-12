#pragma once
#include "Sprite/SpriteModel.h"
#include "BaseCommand.h"

class SpriteRenameCommand : public BaseCommand
{
public:
	SpriteRenameCommand(CommandEnv* commandEnv, SpriteModel* spriteModel, QString nodePath, QString newName);
	virtual void redo() override;
	virtual void undo() override;

private:
	SpriteModel* spriteModel;
	QString nodePath;
	QString newName;
	QString oldName;
};
