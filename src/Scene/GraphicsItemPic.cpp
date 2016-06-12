#include "stdafx.h"
#include "GraphicsItemPic.h"
#include "GraphicsScene.h"
#include "PicturePool.h"

int GraphicsItemPic::drawCount = 0;

GraphicsItemPic::GraphicsItemPic( Picture* picture, int index )
{
	this->picture = picture;
	//GraphicsItemFlags graphicsItemFlags = 0;

	//bool visible = static_cast< GraphicsScene* >( scene() )->getPictureVisible( index );
	//if ( visible )
	//	graphicsItemFlags |= ItemIsMovable | ItemIsSelectable;

	setFlags( ItemIsMovable | ItemIsSelectable );
	//setFlags( 0 );
	//pixmap = new QPixmap;
	//pixmap->load( picture->getFileName() );
	pixmap = globalPicturePool.get( picture->getFileName() );
	setPos( picture->getPos() );
	this->index = index;
	//setCursor( Qt::OpenHandCursor );

	

	//setCacheMode( DeviceCoordinateCache );
}

GraphicsItemPic::~GraphicsItemPic()
{
	//delete pixmap;
}

QRectF GraphicsItemPic::boundingRect() const
{
	return outlineRect();

	const int margin = 1;
	return outlineRect().adjusted( -margin, -margin, margin, margin );
}

void GraphicsItemPic::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ )
{
	//painter->setClipRect( option->exposedRect );

	bool visible = static_cast< GraphicsScene* >( scene() )->getPictureVisible( index );

	if ( visible )
	{
		setPos( picture->getPos() );
		//setFlags( ItemIsMovable | ItemIsSelectable );
	}
// 	else
// 		setFlags( 0 );

	if ( visible )
	{
		painter->drawPixmap( 0, 0, *pixmap );
	}
	else
	{
		//int w = pixmap->width();
		//int h = pixmap->height();

		//painter->fillRect( 0, 0, w, h, Qt::DiagCrossPattern );
	}

// 
// 	int w = pixmap->width();
// 	int h = pixmap->height();
// 	//painter->drawPixmap( 0, 0, *pixmap );
// 
// 	QPen whitePen( Qt::white );
// 	QPen blackPen( Qt::black );
// 	blackPen.setStyle( Qt::DashLine );
// 	painter->setPen( blackPen );


	//QRectF objectRect = boundingRect();
	//QRectF mappedRect = painter->transform().mapRect( objectRect );
	//qreal widthRatio = objectRect.width() / mappedRect.width();
	//QRectF r( x, y, ( double ) w - widthRatio, ( double ) h - widthRatio );

	//painter->setPen( whitePen );
	//painter->drawRect( 0, 0, w, h );

	//painter->setPen( blackPen );
	//painter->drawRect( 0, 0, w, h );

	//painter->fillRect( 0, 0, w, h, Qt::Dense7Pattern );

	//if ( option->state & QStyle::State_Selected )
	//{
	//	QPen whitePen( Qt::white );
	//	QPen blackPen( Qt::black );
	//	blackPen.setStyle( Qt::DashLine );


	//	QRectF objectRect = boundingRect();
	//	QRectF mappedRect = painter->transform().mapRect( objectRect );
	//	qreal widthRatio = objectRect.width() / mappedRect.width();
	//	QRectF r( x, y, ( double ) w - widthRatio, ( double ) h - widthRatio );

	//	painter->setPen( whitePen );
	//	painter->drawRect( r );

	//	painter->setPen( blackPen );
	//	painter->drawRect( r );
	//}

	//drawCount++;
	//qDebug() << "draw count: " << drawCount;
}

QRectF GraphicsItemPic::outlineRect() const
{
	return pixmap->rect();
}

void GraphicsItemPic::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	QGraphicsItem::mouseMoveEvent( event );

	QRect rect( pos().toPoint().x(), pos().toPoint().y(), pixmap->width(), pixmap->height() );
	GraphicsScene* graphicsScene = static_cast< GraphicsScene* >( scene() );
	QPoint p = graphicsScene->stickItem( rect, this );
	setPos( p );

	graphicsScene->setPicturePos( index, pos().toPoint() );

// 	if ( picture )
// 		picture->setPos( pos().toPoint() );
}

int GraphicsItemPic::getIndex() const
{
	return index;
}

void GraphicsItemPic::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	QGraphicsItem::mouseReleaseEvent( event );

// 	if ( picture )
// 		picture->setPos( pos().toPoint() );

	GraphicsScene* graphicsScene = static_cast< GraphicsScene* >( scene() );
	graphicsScene->setPicturePos( index, pos().toPoint() );
}



