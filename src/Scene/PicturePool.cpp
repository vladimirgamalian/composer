#include "stdafx.h"
#include "PicturePool.h"

PicturePool globalPicturePool;

QPixmap* PicturePool::get( QString fileName )
{
	QFileInfo fileInfo( fileName );
	QDateTime lastModified = fileInfo.lastModified();

	if ( pictures.contains( fileName ) )
	{
		MapItem i = pictures.value( fileName );
		if ( i.lastModified == lastModified )
			return i.pixmap;
	}

	QPixmap* pixmap = new QPixmap( fileName );
	pictures[ fileName ] = MapItem( pixmap, lastModified );

	//qDebug() << "picture added to pool, total count is " << pictures.size();

	return get( fileName );
}
