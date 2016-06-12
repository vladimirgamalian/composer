#include "stdafx.h"
#include "AnimationBaseCommand.h"

AnimationBaseCommand::AnimationBaseCommand(CommandEnv* commandEnv, QString spritePath)
	: BaseCommand(commandEnv), spritePath(spritePath)
{

}

AnimationBaseCommand::~AnimationBaseCommand()
{
	delete spriteBackup;
}

void AnimationBaseCommand::redo()
{
	commandEnv->restore();

	TreeNode* node = commandEnv->project->animCloneNode(spritePath);
	Q_ASSERT(node->getType() == TreeNode::NodeType::Sprite);
	spriteBackup = reinterpret_cast<Sprite*>(node);

	action();
}

void AnimationBaseCommand::undo()
{
	commandEnv->project->replaceSprite(spritePath, spriteBackup);
	delete spriteBackup;
	spriteBackup = nullptr;

	commandEnv->restore();
}
