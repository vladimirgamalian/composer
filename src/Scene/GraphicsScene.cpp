#include "stdafx.h"
#include "GraphicsScene.h"
#include "GraphicsItemPic.h"

GraphicsScene::GraphicsScene( Project* project, SpriteView* spriteView, AnimationView* animationView, 
	qreal x, qreal y, qreal width, qreal height, QObject *parent /*= 0*/ ) : QGraphicsScene( x, y, width, height, parent ),
	project(project), spriteView(spriteView), animationView(animationView)
{
	setItemIndexMethod( QGraphicsScene::NoIndex );	// NoIndex BspTreeIndex
}

GraphicsScene::~GraphicsScene()
{
	delete frameBackup;
}

void GraphicsScene::resetModel()
{
	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrent();

	clear();

	int count = 0;
	
	if (!spritePath.isEmpty())
		if (frameIndex >= 0)
			count = project->compositionGetRowCount(spritePath, frameIndex);

	for ( int i = count - 1; i >= 0; --i )
	{
		Picture* picture = project->compostionGetPicture(spritePath, frameIndex, i);
		if ( !picture )
			continue;

		GraphicsItemPic* item = new GraphicsItemPic( picture, i );
		addItem( item );
	}
}

bool GraphicsScene::isStickPos( int pos, int stickPos )
{
	int len = abs( pos - stickPos );
	return ( len < MagnetThresold );
}

bool GraphicsScene::across( int a, int b, int c, int d )
{
	int x = std::max( a, c );
	int y = std::min( b, d );
	return ( x <= y );
}

QPoint GraphicsScene::stickItem( const QRect& rect, QGraphicsItem* item )
{
	QList< int > xStickList;
	QList< int > yStickList;
	
	int x = rect.x();
	int y = rect.y();
	int w = rect.width();
	int h = rect.height();

	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrent();

	if ( snapToBorder )
	{
		xStickList.push_back( 0 );
		xStickList.push_back( ScreenWidth - w );
		yStickList.push_back( 0 );
		yStickList.push_back( ScreenHeight - h );
	}

	if ( snapToPictures )
	{
		foreach( QGraphicsItem *otherItem, items() )
		{
			if ( otherItem == item )
				continue;

			GraphicsItemPic* otherItemPic = static_cast< GraphicsItemPic* >( otherItem );
			if ( !project->compositionGetVisible(spritePath, frameIndex, otherItemPic->getIndex() ) )
				continue;

			QRect otherRect = QRect( otherItemPic->pos().toPoint().x(), otherItemPic->pos().toPoint().y(),
				otherItemPic->outlineRect().toRect().width(), otherItemPic->outlineRect().toRect().height() );

			bool heightAcross = across( rect.top(), rect.bottom(), otherRect.top() - 0, otherRect.bottom() + 0 );
			bool widthAcross = across( rect.left(), rect.right(), otherRect.left() - 0, otherRect.right() + 0 );

			if ( heightAcross )
			{
				xStickList.push_back( otherRect.right() + 1 );
				xStickList.push_back( otherRect.left() - w );
			}

			if ( widthAcross )
			{
				yStickList.push_back( otherRect.bottom() + 1 );
				yStickList.push_back( otherRect.top() - h );
			}
		}
	}

	if ( snapToGuides )
	{
		if ( graphicsView )
		{
			QList< int > horGuides;
			graphicsView->getHorGuides( horGuides );

			foreach( int guideY, horGuides )
			{
				yStickList.push_back( guideY );
				yStickList.push_back( guideY - h );
			}

			QList< int > verGuides;
			graphicsView->getVerGuides( verGuides );

			foreach( int guideX, verGuides )
			{
				xStickList.push_back( guideX );
				xStickList.push_back( guideX - w );
			}
		}
	}

	int minDistance = MagnetThresold;
	foreach( int xStick, xStickList )
	{
		int distance = abs( x - xStick );
		if ( distance < minDistance  )
		{
			minDistance = distance;
			x = xStick;
		}
	}

	minDistance = MagnetThresold;
	foreach( int yStick, yStickList )
	{
		int distance = abs( y - yStick );
		if ( distance < minDistance )
		{
			minDistance = distance;
			y = yStick;
		}
	}

	return QPoint( x, y );
}

void GraphicsScene::setGraphicsView( GraphicsView* graphicsView )
{
	this->graphicsView = graphicsView;
}

void GraphicsScene::triggerStickToBorder( bool state )
{
	snapToBorder = state;
}

void GraphicsScene::triggerStickToPictures( bool state )
{
	snapToPictures = state;
}

void GraphicsScene::triggerStickToGuides( bool state )
{
	snapToGuides = state;
}

QList< int > GraphicsScene::getSelectedItemsIndexes() const
{
	QList< int > indexes;

	foreach( QGraphicsItem *item, selectedItems() )
	{
		if ( item->type() == QGraphicsItem::UserType + 1 )
		{
			GraphicsItemPic* pic = static_cast< GraphicsItemPic* >( item );
			indexes.append( pic->getIndex() );
		}
	}

	return indexes;
}

void GraphicsScene::setSelectedIndexes( const QList<int>& selectedIndexes )
{
	clearSelection();

	QList< QGraphicsItem* > allItems = items();
	foreach( QGraphicsItem *item, allItems )
	{
		if ( item->type() == QGraphicsItem::UserType + 1 )
		{
			GraphicsItemPic* pic = static_cast< GraphicsItemPic* >( item );
			if ( selectedIndexes.indexOf( pic->getIndex() ) != -1 )
				pic->setSelected( true );
		}
	}
}

void GraphicsScene::dragEnterEvent( QGraphicsSceneDragDropEvent* event )
{
	QGraphicsScene::dragEnterEvent( event );

	if ( event->dropAction() != Qt::CopyAction )
		return;

	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrent();

	const QMimeData *mimeData = event->mimeData();
	if ( project->checkDragEventForImage(spritePath, frameIndex, mimeData ) )
	{
		event->accept();
		event->acceptProposedAction();
	}
}

void GraphicsScene::dragMoveEvent( QGraphicsSceneDragDropEvent* event )
{
	Qt::DropAction dp = event->dropAction();
	QGraphicsScene::dragMoveEvent( event );

	if ( dp != Qt::CopyAction )
		return;

	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrent();

	const QMimeData *mime = event->mimeData();
	if (project->checkDragEventForImage(spritePath, frameIndex, mime))
	{
		event->accept();
		event->acceptProposedAction();
	}
}

void GraphicsScene::dropEvent( QGraphicsSceneDragDropEvent* event )
{
	QGraphicsScene::dropEvent( event );

	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrent();

	if ( event->dropAction() != Qt::CopyAction )
		return;

	const QMimeData *mimeData = event->mimeData();
	if (project->checkDragEventForImage(spritePath, frameIndex, mimeData))
	{
		event->accept();
		event->acceptProposedAction();

		QPoint pos = event->scenePos().toPoint();

		QStringList fileList;
		foreach( QUrl url, mimeData->urls() )
			if ( url.isLocalFile() )
				fileList << url.toLocalFile();


		emit dropPictures(0, pos, fileList);
	}
}

void GraphicsScene::dragLeaveEvent( QGraphicsSceneDragDropEvent* event )
{
	QGraphicsScene::dragLeaveEvent( event );
}

bool GraphicsScene::getPictureVisible( int index )
{
	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrent();
	return project->compositionGetVisible(spritePath, frameIndex, index);
}

void GraphicsScene::setPicturePos( int index, const QPoint& pos )
{
	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrent();
	project->scenePictureMove(spritePath, frameIndex, index, pos );
}

void GraphicsScene::picturesShift(int shiftX, int shiftY)
{
	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrent();
	QList<int> pics = getSelectedItemsIndexes();

	project->compositionPicturesShift(spritePath, frameIndex, pics, shiftX, shiftY);

	graphicsView->redrawAll();
}

void GraphicsScene::picturesToggleVisible()
{
	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrent();
	QList<int> pics = getSelectedItemsIndexes();

	project->compositionPicturesToggleVisible(spritePath, frameIndex, pics);
}

void GraphicsScene::startMoving()
{
	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrent();
	delete frameBackup;
	frameBackup = project->cloneFrame(spritePath, frameIndex);
}

void GraphicsScene::finishMoving()
{
	Q_ASSERT(frameBackup);
	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrent();
	Frame* newFrame = project->getFrame(spritePath, frameIndex);
	Q_ASSERT(frameBackup->pictures.size() == newFrame->pictures.size());
	QList<Project::MovePicData> moveData;
	for (int i = 0; i < frameBackup->pictures.size(); ++i)
	{
		QPoint oldPos = frameBackup->pictures.at(i)->getPos();
		QPoint newPos = newFrame->pictures.at(i)->getPos();
		if (oldPos != newPos)
		{
			moveData << Project::MovePicData{i, oldPos, newPos};
			newFrame->pictures.at(i)->setPos(oldPos);
		}
	}
	Q_ASSERT(newFrame->isEqual(frameBackup));
	delete frameBackup;
	frameBackup = nullptr;

	if (moveData.isEmpty())
		return;

	emit movePictures(spritePath, frameIndex, moveData);
}
