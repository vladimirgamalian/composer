#pragma once
#include "Commands/BaseCommand.h"

class CompositionTriggerPicsVisible : public BaseCommand
{
public:
	CompositionTriggerPicsVisible(CommandEnv* commandEnv, QString spritePath, int frameIndex,
		const QList<int>& pics);

	void redo() override;
	void undo() override;

private:
	QString spritePath;
	int frameIndex;
	QList<int> pics;
};
