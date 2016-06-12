#pragma once

class Picture
{
public:

	Picture();
	virtual ~Picture();
	//Picture( const Picture& );

	void setFileName( QString fileName );
	QString getFileName() const;

	void setPos( QPoint p );
	QPoint getPos() const;
	void shiftPos( int x, int y );

	bool getVisible() const;
	void setVisible( bool value );

	bool isEqual( Picture* picture ) const;

private:

	Picture& operator= ( Picture const & );

	QString fileName;
	QPoint pos;
	bool visible;
};
