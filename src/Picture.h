#pragma once

class Picture
{
public:
	Picture();
	virtual ~Picture();

	void setFileName( QString fileName );
	QString getFileName() const;

	void setPos( QPoint p );
	QPoint getPos() const;
	void shiftPos(int x, int y);
	void shiftPos(QPoint shift);
	QRect getRect() const;

	bool getVisible() const;
	void setVisible( bool value );

	bool isEqual( Picture* picture ) const;

private:
	Picture& operator= ( Picture const & );
	QString fileName;
	QPoint pos;
	bool visible;
};
