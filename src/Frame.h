#pragma once

#include "Picture.h"
 
class Frame
{
public:

	Frame();
	Frame( const Frame& );
	~Frame();

	int getDuration() const
	{
		return duration;
	}

	QString getTag() const
	{
		return tag;
	}

	bool setDuration( int v )
	{
		if ( ( v < 1 ) || ( v > 1000 ) )
			return false;
		duration = v;
		return true;
	}

	void setTag( const QString& v )
	{
		//TODO: check value
		tag = v;
	}

	QList< Picture* > pictures;

	Frame& operator= ( Frame const & );

	bool isEqual( const Frame* frame ) const;

	Frame* clone() const
	{
		return new Frame(*this);
	}

private:

	enum 
	{
		DefaultDuration = 8
	};

	QString tag;
	int duration;
};
