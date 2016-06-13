#include "stdafx.h"
#include "Frame.h"

Frame::Frame( const Frame& src )
{
	foreach ( auto i, src.pictures )
		pictures.append( new Picture( *i ) );

	duration = src.duration;
	tag = src.tag;
}

Frame::Frame()
{
	duration = DefaultDuration;
}

Frame::~Frame()
{
	qDeleteAll( pictures );
}

Frame& Frame::operator=( Frame const &src )
{
	pictures.clear();
	foreach( auto p, src.pictures )
	{
		Picture* picture = new Picture( *p );
		pictures.append( picture );
	}

	duration = src.duration;
	tag = src.tag;
	
	return *this;
}

bool Frame::isEqual( const Frame* frame ) const
{
	int s = pictures.size();

	if ( s != frame->pictures.size() )
		return false;

	for ( int i = 0; i < s; ++i )
		if ( !pictures[ i ]->isEqual( frame->pictures[ i ] ) )
			return false;

	return true;
}
