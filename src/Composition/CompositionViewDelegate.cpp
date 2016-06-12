#include "stdafx.h"
#include "CompositionViewDelegate.h"
#include "CompositionModel.h"

void CompositionViewDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	bool selected = option.state & QStyle::State_Selected;
	QBrush highlightBrush( option.palette.highlight() );
	QBrush darkBrush( option.palette.dark() );


	int l = option.rect.left();
	int r = option.rect.right();
	int t = option.rect.top();
	int b = option.rect.bottom();
	int w = option.rect.width();
	int h = option.rect.height();

	int eyeWidth = 24;

	QPen penGrayLine( QColor( 230, 230, 230, 255 ) );
	painter->setPen( penGrayLine );
	painter->drawLine( l, b, r, b );
	painter->drawLine( l + eyeWidth, t, l + eyeWidth, b );

	if ( index.data( CompositionModel::RoleVisible ).toBool() )
	{
		QPixmap pixmapEye( ":/misc/Resources/misc/eye.png" );
		painter->drawPixmap( l + 4, t + 10, pixmapEye );
	}


	if ( selected )
		painter->fillRect( l + eyeWidth, t, w - eyeWidth, h, highlightBrush );



	painter->drawRect( l + eyeWidth + 4, t + 2, 28, 28 );
	//painter->fillRect( l + eyeWidth + 4, t + 2, 28, 28, Qt::Dense4Pattern );

	QString fileName = index.data( CompositionModel::RoleFileName ).toString();
	QPixmap pixmap( fileName );

	int pixmapW = pixmap.width();
	int pixmapH = pixmap.height();

	double k = ( pixmapW > pixmapH ) ? 28.0 / ( double ) pixmapW : 28.0 / ( double ) pixmapH;
	pixmapW = ( double ) pixmapW * k;
	pixmapH = ( double ) pixmapH * k;
	int pixmapX = ( 28 - pixmapW ) / 2;
	int pixmapY = ( 28 - pixmapH ) / 2;

	painter->drawPixmap( l + eyeWidth + 4 + pixmapX, t + 2 + pixmapY, pixmapW, pixmapH, pixmap );


//	painter->setBrush( darkBrush );
	painter->setPen( QColor( "black" ) );

	if ( selected )
		painter->setPen( QColor( "white" ) );
	else
		painter->setPen( option.palette.foreground().color() );

	QString text = index.data( Qt::DisplayRole ).toString();

	QPoint picturePos = index.data( CompositionModel::RolePos ).toPoint();
	QString picturePosText = QString( "%1:%2" ).arg( picturePos.x() ).arg( picturePos.y() );

	int maxTextWidth = w - 64 - 8;

	QRect textBoundingRect = painter->boundingRect( QRect(), 0, text );

	int printFlag = ( textBoundingRect.width() > maxTextWidth ) ? Qt::AlignRight : Qt::AlignLeft;
	painter->drawText( l + 64, t + 3, maxTextWidth, h - 10, printFlag, text );

	
	if ( selected )
		painter->setPen( QColor( "white" ) );
	else
		painter->setPen( QColor( "gray" )  );
	painter->drawText( l + 64, t + 17, maxTextWidth, h - 10, Qt::AlignLeft, picturePosText );

}

QSize CompositionViewDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	return QSize( 64, 34 );
}

bool CompositionViewDelegate::editorEvent( QEvent* event, QAbstractItemModel* model,
	const QStyleOptionViewItem& option, const QModelIndex& index )
{
	int l = option.rect.left();
	int r = option.rect.right();
	int t = option.rect.top();
	int b = option.rect.bottom();
	int w = option.rect.width();
	int h = option.rect.height();
	int eyeWidth = 24;

	QRect eyeRect( l, t, eyeWidth, h );

	bool isMouseEvent = ( event->type() == QEvent::MouseButtonPress ) || 
		( event->type() == QEvent::MouseButtonRelease ) ||
		( event->type() == QEvent::MouseButtonDblClick ) ||
		( event->type() == QEvent::MouseMove );
	
	if ( isMouseEvent )
	{
		QMouseEvent* mouseEvent = static_cast< QMouseEvent* >( event );

		if ( eyeRect.contains( mouseEvent->pos() ) )
		{
			if ( ( mouseEvent->button() == Qt::LeftButton ) && ( event->type() == QEvent::MouseButtonRelease ) )
			{
				bool checked = index.model()->data( index, CompositionModel::RoleVisible ).toBool();
				model->setData( index, !checked, CompositionModel::RoleVisible );
			}

			return true;
		}
	}

	return false;
}

