#pragma once
#include "Project.h"

class AnimationView : public QListView
{
Q_OBJECT
public:
	AnimationView( Project* project, QWidget *parent = 0 );
	QList<int> getSelected() const;
	int getCurrent() const;
	void setCurrentFrame(int i);
	void setSelected(const QList< int >& selected);
	void setCurrent(int row);

signals:
	void selectChanged();
	void deleteSelectedItem();
	void resetCurrentFrame();

protected:
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) override;
	virtual void currentChanged(const QModelIndex& current, const QModelIndex& previous) override;

private:
	Project* project;
};
