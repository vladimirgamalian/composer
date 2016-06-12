#include "stdafx.h"
#include "AnimationCompressCommand.h"

AnimationCompressCommand::AnimationCompressCommand(CommandEnv* commandEnv, QString spritePath)
	: AnimationBaseCommand(commandEnv, spritePath)
{
	setText(QString("compress sprite ") + spritePath);
}

void AnimationCompressCommand::action()
{
	commandEnv->project->spritesCompress(spritePath);
}
