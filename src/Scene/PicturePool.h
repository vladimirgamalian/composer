#pragma once

class PicturePool
{
public:

	QPixmap* get( QString fileName );

private:

	struct MapItem
	{
		MapItem() {}
		MapItem( QPixmap* pixmap, QDateTime lastModified ) : pixmap( pixmap ), lastModified( lastModified ) {}
		QPixmap* pixmap;
		QDateTime lastModified;
	};

	QMap< QString, MapItem > pictures;
	//QMap< QString, QPixmap* > pictures;
};

extern PicturePool globalPicturePool;
