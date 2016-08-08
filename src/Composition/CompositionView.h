#pragma once
#include "Scene/GraphicsScene.h"

class CompositionView : public QListView
{
	Q_OBJECT
public:
	CompositionView(QWidget *parent = 0 );
	QList<int> getSelected() const;
	void setSelected( const QList<int>& list );

public slots:
	void redrawAll();

signals:
	void deleteSelectedItem();
	void selectChanged();

protected:
	virtual void selectionChanged( const QItemSelection& selected, const QItemSelection& deselected );
	virtual void keyPressEvent( QKeyEvent* event );
	virtual void dragEnterEvent( QDragEnterEvent *event );
	virtual void dragMoveEvent( QDragMoveEvent* event );
};
