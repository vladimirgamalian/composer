#include "stdafx.h"
#include "GraphicsItemPic.h"
#include "GraphicsScene.h"
#include "PicturePool.h"

int GraphicsItemPic::drawCount = 0;

GraphicsItemPic::GraphicsItemPic( Picture* picture, int index )
{
	this->picture = picture;
	setFlags( ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
	pixmap = globalPicturePool.get( picture->getFileName() );
	setPos( picture->getPos() );
	this->index = index;
}

GraphicsItemPic::~GraphicsItemPic()
{

}

QRectF GraphicsItemPic::boundingRect() const
{
	return outlineRect();

	const int margin = 1;
	return outlineRect().adjusted( -margin, -margin, margin, margin );
}

void GraphicsItemPic::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ )
{
	bool visible = static_cast< GraphicsScene* >( scene() )->getPictureVisible( index );

	if ( visible )
	{
		setPos( picture->getPos() );
		painter->drawPixmap(0, 0, *pixmap);
	}
}

QRectF GraphicsItemPic::outlineRect() const
{
	return pixmap->rect();
}

QVariant GraphicsItemPic::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionChange && scene()) 
	{
		GraphicsScene* graphicsScene = static_cast< GraphicsScene* >(scene());
		QPoint newPos = value.toPoint();
		QRect rect(newPos.x(), newPos.y(), pixmap->width(), pixmap->height());
		QPoint p = graphicsScene->stickItem(rect, this);
		graphicsScene->setPicturePos(index, p);

		return p;
	}
	return QGraphicsItem::itemChange(change, value);
}

void GraphicsItemPic::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	QGraphicsItem::mouseMoveEvent( event );
}

int GraphicsItemPic::getIndex() const
{
	return index;
}

void GraphicsItemPic::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	QGraphicsItem::mouseReleaseEvent( event );
}
