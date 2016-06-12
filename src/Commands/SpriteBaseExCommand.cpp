#include "stdafx.h"
#include "SpriteBaseExCommand.h"

SpriteBaseExCommand::SpriteBaseExCommand(CommandEnv* commandEnv, QString spritePath)
	: BaseCommand(commandEnv), spritePath(spritePath)
{

}

void SpriteBaseExCommand::redo()
{
	commandEnv->restore();
	action();
}

void SpriteBaseExCommand::undo()
{
	commandEnv->restore();
}
