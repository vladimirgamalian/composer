#include "stdafx.h"
#include "CompositionBaseCommand.h"

CompositionBaseCommand::CompositionBaseCommand(CommandEnv* commandEnv, QString spritePath, int frameIndex)
	: BaseCommand(commandEnv),
	spritePath(spritePath),
	frameIndex(frameIndex)
{

}

CompositionBaseCommand::~CompositionBaseCommand()
{
	delete frameBackup;
}

void CompositionBaseCommand::redo()
{
	commandEnv->restore();
	frameBackup = commandEnv->project->cloneFrame(spritePath, frameIndex);
	action();
}

void CompositionBaseCommand::undo()
{
	commandEnv->project->replaceFrame(spritePath, frameIndex, frameBackup);
	frameBackup = nullptr;
	commandEnv->restore();
}
