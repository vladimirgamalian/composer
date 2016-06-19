#pragma once
#include "Picture.h"

class GraphicsItemPic : public QGraphicsItem
{
public:
	GraphicsItemPic( Picture* picture, int index );
	virtual ~GraphicsItemPic();

	QRectF boundingRect() const;
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ );
	QRectF outlineRect() const;
	
	int getIndex() const;

protected:
	virtual int	type () const override  { return Type; }
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
	virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event ) override;
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event ) override;

private:
	static int drawCount;
	enum { Type = UserType + 1 };

	Picture* picture;
	QPixmap* pixmap;
	int index;
};
