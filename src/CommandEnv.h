#pragma once
#include "Project.h"
#include "Sprite/SpriteView.h"
#include "Animation/AnimationView.h"
#include "Composition/CompositionView.h"

struct CommandEnv
{
	Project* project = nullptr;

	SpriteView* spriteView = nullptr;
	AnimationView* animationView = nullptr;
	CompositionView* compositionView = nullptr;

	QStringList spriteExpanded;
	QString spriteCurrent;
	QList<int> animationSelection;
	QList<int> compositionSelection;

	void restore()
	{
		spriteView->setExpandedNodeList(spriteExpanded);
		spriteView->setCurrentNode(spriteCurrent);
		animationView->setSelected(animationSelection);
		compositionView->setSelected(compositionSelection);
	}
};
