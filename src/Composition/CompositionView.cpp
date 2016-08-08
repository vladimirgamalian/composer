#include "stdafx.h"
#include "CompositionView.h"
#include "CompositionViewDelegate.h"

//TODO: remove when remake via signals
#include "Composition/CompositionModel.h"

CompositionView::CompositionView(QWidget *parent /*= 0 */)
	: QListView( parent )
{
	setItemDelegate( new CompositionViewDelegate );

	setDragEnabled( true );
	setAcceptDrops( true );
	setDropIndicatorShown( true );

	setDragDropMode( QAbstractItemView::DragDrop );
	setDefaultDropAction( Qt::MoveAction );

	setSelectionMode( ExtendedSelection );
}

void CompositionView::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
	QListView::selectionChanged(selected, deselected);
	emit selectChanged();
}

void CompositionView::keyPressEvent( QKeyEvent* event )
{
	if ( event->key() == Qt::Key_Delete )
	{
		emit deleteSelectedItem();
		return;
	}

	if ( event->key() == Qt::Key_Space )
	{
		CompositionModel* modelComposition = static_cast< CompositionModel* >(model());
		modelComposition->toggleVisible(getSelected());
		return;
	}

	QListView::keyPressEvent( event );
}

void CompositionView::dragEnterEvent( QDragEnterEvent *event )
{
	QListView::dragEnterEvent( event );

	const QMimeData *mimeData = event->mimeData();
	CompositionModel* modelComposition = static_cast< CompositionModel* >( model() );
	if ( modelComposition->isInternalData( mimeData ) )
		return;

	if ( event->dropAction() != Qt::CopyAction )
	{
		event->ignore();
		return;
	}

	if ( !modelComposition->checkDragEventForImage( mimeData ) )
		event->ignore();
}

void CompositionView::dragMoveEvent( QDragMoveEvent* event )
{
	QListView::dragMoveEvent( event );

	const QMimeData *mimeData = event->mimeData();
	CompositionModel* modelComposition = static_cast< CompositionModel* >( model() );
	if ( modelComposition->isInternalData( mimeData ) )
		return;

	if ( event->dropAction() != Qt::CopyAction )
	{
		event->ignore();
		return;
	}

	if ( !modelComposition->checkDragEventForImage( mimeData ) )
		event->ignore();
}

QList<int> CompositionView::getSelected() const
{
	QList<int> res;
	QModelIndexList selected = selectedIndexes();
	foreach ( QModelIndex i, selected )
		res << i.row();
	return res;
}

void CompositionView::setSelected(const QList<int>& list)
{
	QItemSelection selection;
	for (int row: list)
	{
		QModelIndex i = model()->index(row, 0);
		selection.select(i, i);
	}

	selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
}

void CompositionView::redrawAll()
{
	viewport()->update();
}
