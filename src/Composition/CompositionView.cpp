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

//void CompositionView::selectionChanged( const QItemSelection& selected, const QItemSelection& deselected )
//{
//	QListView::selectionChanged( selected, deselected );
//
//	QList< int > l;
//	foreach ( auto index, selectedIndexes() )
//		l.append( index.row() );
//
//	project->compositionSelect( l );
//}

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

// void CompositionView::delPressed()
// {
// 	emit deleteSelectedItem();
// }

// void CompositionView::delSelectedItems()
// {
// 	QModelIndexList indexes = selectionModel()->selectedIndexes();
// 	if ( !indexes.size() )
// 		return;
// 
// 	QList< int > rows;
// 	foreach(QModelIndex i, indexes)
// 		rows << i.row();
// 
// 	QString spritePath = spriteView->getCurrentNode();
// 	int frameIndex = animationView->getCurrent();
// 
// 	//CompositionModel* modelComposition = static_cast< CompositionModel* >( model() );
// 	//modelComposition->delItems( indexes );
// 
// 	emit 
// }

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

//void CompositionView::setSelectedFrames( const QModelIndexList& selected )
//{
//	QItemSelection selection;
//	foreach( QModelIndex i, selected )
//		selection.select( i, i );
//	selectionModel()->select( selection, QItemSelectionModel::ClearAndSelect );
//}

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

int CompositionView::getCurrent() const
{
	return currentIndex().row();
}

void CompositionView::setCurrent(int row)
{
	QModelIndex i = model()->index(row, 0);
	setCurrentIndex(i);
}

void CompositionView::redrawAll()
{
	viewport()->update();
}
