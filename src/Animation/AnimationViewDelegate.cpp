#include "stdafx.h"
#include "AnimationViewDelegate.h"

void AnimationViewDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	bool selected = option.state & QStyle::State_Selected;
	QBrush highlightBrush(option.palette.highlight());
	QBrush cuurentHighlightBrush(option.palette.shadow());
	QBrush darkBrush( option.palette.dark() );

	painter->drawRect( option.rect );

	if (selected)
		painter->fillRect( option.rect, highlightBrush);

	int x = option.rect.left();
	int y = option.rect.top();

	painter->drawPixmap( x, y, framePixmap );

	//painter->setBrush( darkBrush );
	//painter->setPen( QColor( "black" ) );
	//painter->drawRect( x + 10, y + 10, 80, 80 );

	//painter->setBrush( option.palette.foreground() );
	
	if ( selected )
		painter->setPen( QColor( "white" ) );
	else
		painter->setPen( option.palette.foreground().color() );

	int duration = index.data( Qt::UserRole + 3 ).toInt();

	QString tag = index.data( Qt::UserRole + 4 ).toString();

	QString frameNumber = QString::number( index.row() );
	QString frameDuration = QString::number( duration );
	


	QFont font;
	font.setPointSize( 8 );
	painter->setFont( font );

	painter->drawText( x + 15, y + 120, frameNumber + "/" + frameDuration );
	painter->drawText( x + 15, y + 135, tag );
}

QSize AnimationViewDelegate::sizeHint( const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/ ) const
{
	return QSize( 100, 150 );
}

AnimationViewDelegate::AnimationViewDelegate()
{
	framePixmap.load( ":/misc/Resources/misc/aniframe.png" );
}
