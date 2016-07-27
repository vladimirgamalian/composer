#pragma once
#include "Project.h"
#include "AnimationViewDelegate.h"

class AnimationView : public QListView
{
Q_OBJECT
public:
	AnimationView( Project* project, QWidget *parent = 0 );
	QList<int> getSelected() const;
	void setSelected(const QList< int >& selected);
	int getCurrentFrame() const;

signals:
	void deleteSelectedItem();
	void resetCurrentFrame();

protected:
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) override;

private:
	Project* project;
};
