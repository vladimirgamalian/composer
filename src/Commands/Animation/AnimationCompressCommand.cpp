#include "stdafx.h"
#include "AnimationCompressCommand.h"

AnimationCompressCommand::AnimationCompressCommand(CommandEnv* commandEnv, QString spritePath, const QList<int>& indexes)
	: AnimationBaseCommand(commandEnv, spritePath),	indexes(indexes)
{
	setText(QString("compress sprite ") + spritePath);
}

void AnimationCompressCommand::action()
{
	commandEnv->project->spritesCompress(spritePath, indexes, false);
	commandEnv->animationView->setSelected(QList<int>());
}
