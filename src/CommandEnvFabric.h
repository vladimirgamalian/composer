#pragma once
#include "CommandEnv.h"

class CommandEnvFabric
{
public:
	CommandEnvFabric(Project* project, SpriteView* spriteView,
		AnimationView* animationView, CompositionView* compositionView)
		: project(project), spriteView(spriteView),
		animationView(animationView), compositionView(compositionView)
	{
		Q_ASSERT(project);
		Q_ASSERT(spriteView);
		Q_ASSERT(animationView);
		Q_ASSERT(compositionView);
	}

	CommandEnv* getCommandEnv() const
	{
		CommandEnv* commandEnv = new CommandEnv;

		commandEnv->project = project;

		commandEnv->spriteView = spriteView;
		commandEnv->animationView = animationView;
		commandEnv->compositionView = compositionView;

		commandEnv->spriteExpanded = spriteView->getExpandedNodeList();
		commandEnv->spriteCurrent = spriteView->getCurrentNode();

		commandEnv->animationSelection = animationView->getSelected();
		commandEnv->animationCurrent = animationView->getCurrent();

		commandEnv->compositionSelection = compositionView->getSelected();
		commandEnv->compositionCurrent = compositionView->getCurrent();

		return commandEnv;
	}

private:
	Project* project;
	SpriteView* spriteView;
	AnimationView* animationView;
	CompositionView* compositionView;
};
