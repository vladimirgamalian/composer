#include "stdafx.h"
#include "Picture.h"
#include "Scene/PicturePool.h"

void Picture::setFileName(QString value)
{
	this->fileName = value;
}

QString Picture::getFileName() const
{
	return fileName;
}

QPoint Picture::getPos() const
{
	return pos;
}

bool Picture::getVisible() const
{
	return visible;
}

void Picture::setVisible( bool value )
{
	visible = value;
}

Picture::Picture()
{
	visible = true;
}

void Picture::setPos( QPoint p )
{
	pos = p;
}

Picture::~Picture()
{
	//qDebug() << "picture delete";
}

void Picture::shiftPos( int x, int y )
{
	 pos += QPoint( x, y );
}

void Picture::shiftPos(QPoint shift)
{
	pos += shift;
}

QRect Picture::getRect() const
{
	QPixmap* pixmap = globalPicturePool.get(getFileName());
	return QRect(pos.x(), pos.y(), pixmap->width(), pixmap->height());
}

bool Picture::isEqual(Picture* picture) const
{
	if ( pos != picture->pos )
		return false;

	if ( visible != picture->visible )
		return false;

	if ( fileName != picture->fileName )
		return false;

	return true;
}
