#include "stdafx.h"
#include "AnimationView.h"
#include "AnimationViewDelegate.h"
#include "AnimationModel.h"

AnimationView::AnimationView( Project* project, QWidget *parent /*= 0 */ ) :
	QListView( parent )
{
	this->project = project;

	setFlow( QListView::LeftToRight );
	setItemDelegate( new AnimationViewDelegate );

	setDragEnabled( true );
	setAcceptDrops( true );
	setDropIndicatorShown( true );

	setDragDropMode( QAbstractItemView::DragDrop );
	setDefaultDropAction( Qt::MoveAction );

	setSelectionMode( ExtendedSelection );
}

void AnimationView::selectionChanged( const QItemSelection& selected, const QItemSelection& deselected )
{
	QListView::selectionChanged(selected, deselected);
	updateDurations();
}

void AnimationView::currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	emit resetCurrentFrame();
	updateDurations();
}

void AnimationView::updateDurations()
{
	bool durationFound = false;
	bool differentDurationFound = false;
	int duration = 0;

	QModelIndexList indexes = selectionModel()->selectedIndexes();
	foreach(QModelIndex index, indexes)
	{
		int d = model()->data(index, Qt::UserRole + 3).toInt();
		if ((durationFound) && (duration != d))
		{
			differentDurationFound = true;
			break;
		}

		duration = d;
		durationFound = true;
	}

	emit frameDuration(durationFound, differentDurationFound, duration);
}

void AnimationView::keyPressEvent(QKeyEvent* event)
{
	QListView::keyPressEvent( event );

	if ( event->key() == Qt::Key_Delete )
		emit deleteSelectedItem();
}

QList<int> AnimationView::getSelected() const
{
	QList<int> res;
	QModelIndexList selected = selectedIndexes();
	foreach ( QModelIndex i, selected )
		res << i.row();
	return res;
}

int AnimationView::getCurrent() const
{
	return currentIndex().row();
}

void AnimationView::setCurrentFrame(int i)
{
	setCurrentIndex(model()->index(i, 0));
	emit resetCurrentFrame();
}

void AnimationView::setSelected(const QList< int >& selected)
{
	QItemSelection selection;
	foreach( int row, selected )
	{
		QModelIndex index = model()->index( row, 0 );
		selection.select( index, index );
	}

	selectionModel()->select( selection, QItemSelectionModel::ClearAndSelect );
}

void AnimationView::setCurrent( int row )
{
	QModelIndex i = model()->index( row, 0 );
	setCurrentIndex( i );
}
