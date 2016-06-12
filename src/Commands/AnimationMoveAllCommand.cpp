#include "stdafx.h"
#include "AnimationMoveAllCommand.h"

AnimationMoveAllCommand::AnimationMoveAllCommand(CommandEnv* commandEnv, QString spritePath,
	int shiftX, int shiftY) :
	AnimationBaseCommand(commandEnv, spritePath),
	shiftX(shiftX), shiftY(shiftY)
{
	setText(QString("move sprite ") + spritePath);
}

void AnimationMoveAllCommand::action()
{
	//TODO: after move by 1000 scene wont refresh
	commandEnv->project->spriteMoveSprite(spritePath, shiftX, shiftY);
}
