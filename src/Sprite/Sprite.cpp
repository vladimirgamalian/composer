#include "stdafx.h"
#include "Sprite.h"

Sprite::Sprite( QString name )
{
	text = name;
	icon = QIcon( ":/composer/Resources/icons/Sprite" );
}

Sprite::~Sprite()
{
	qDeleteAll( frames );
}

Sprite* Sprite::clone()
{
	Sprite* newSprite = new Sprite( text );

	foreach( auto frame, frames )
	{
		Frame* newFrame = new Frame;
		*newFrame = *frame;
		newSprite->frames.append( newFrame );
	}

	return newSprite;
}

void Sprite::setFrom(const Sprite* sprite)
{
	text = sprite->text;
	qDeleteAll(frames);
	frames.clear();

	//TODO: try to exchange node instead copy
	foreach(auto frame, sprite->frames)
	{
		Frame* newFrame = new Frame;
		*newFrame = *frame;
		frames.append(newFrame);
	}
}
