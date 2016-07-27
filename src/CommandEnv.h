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
	//int animationCurrent = -1;

	QList<int> compositionSelection;
	int compositionCurrent = -1;

	void restore()
	{
		spriteView->setExpandedNodeList(spriteExpanded);
		spriteView->setCurrentNode(spriteCurrent);

//		animationView->setCurrent(animationCurrent);
		animationView->setSelected(animationSelection);

		compositionView->setCurrent(compositionCurrent);
		compositionView->setSelected(compositionSelection);
	}
};
