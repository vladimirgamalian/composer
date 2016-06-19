#include "stdafx.h"
#include "WidgetRuler.h"

WidgetRuler::WidgetRuler( QWidget *parent, QGraphicsView* gv, bool vertRuler )
	: QWidget( parent ), graphicsView( gv ), drag(false), isVertRuler( vertRuler )
{

}

void WidgetRuler::paintEvent( QPaintEvent * event )
{
	QPainter painter( this );

	int pixelPerSmallStroke = 1;
	int delta = 0;
	forever
	{
		QPoint firstPixel = graphicsView->mapFromScene( 0, 0 );
		QPoint secondPixel = graphicsView->mapFromScene( pixelPerSmallStroke, 0 );

		delta = secondPixel.x() - firstPixel.x();
		if ( delta > 4 )
			break;

		pixelPerSmallStroke = next125( pixelPerSmallStroke );
		if ( pixelPerSmallStroke > 100000 )	//WTF?
			break;
	}

	QColor back = palette().color( QPalette::Normal, QPalette::Window );

	painter.fillRect( rect(), back );
	QPen pen( Qt::black );
	painter.setPen( pen );

	if ( isVertRuler )
	{
		drawRulerStrokesVer( &painter, pixelPerSmallStroke, delta, 1 );
		drawRulerStrokesVer( &painter, pixelPerSmallStroke, delta, -1 );
		painter.drawLine( 19, 0, 19, height() );
	}
	else
	{
		drawRulerStrokesHor( &painter, pixelPerSmallStroke, delta, 1 );
		drawRulerStrokesHor( &painter, pixelPerSmallStroke, delta, -1 );
		painter.drawLine( 0, 19, width(), 19 );
	}
}

void WidgetRuler::repaintRulers()
{
	repaint();
}

int WidgetRuler::next125( int v )
{
	double magnitude = floor( log10( ( double ) v ) );
	v /= pow( 10.0, magnitude );
	if ( v == 1 )
		v = 2;
	else if ( v == 2 )
		v = 5;
	else
		v = 10;
	v *= pow( 10.0, magnitude );
	return v;
}

void WidgetRuler::drawRulerStrokesHor( QPainter* painter, int pixelPerSmallStroke,
	int delta, int dir )
{

	int mediumStrokeInterval = 5;
	int bigStrokeInterval = 10;

	if ( delta > 10 )
		bigStrokeInterval = mediumStrokeInterval = 5;
	if ( delta > 20 )
		bigStrokeInterval = mediumStrokeInterval = 2;
	if ( delta > 30 )
		bigStrokeInterval = mediumStrokeInterval = 1;


	int curStroke =  ( dir == -1 ) ? -1 : 0;
	forever
	{
		int curPos = curStroke * pixelPerSmallStroke;

		bool isMediumStroke = ( !( curStroke % mediumStrokeInterval ) );
		bool isBigStroke =    ( !( curStroke % bigStrokeInterval    ) );

		QPoint strokePos = graphicsView->mapFromScene( curPos, 0 );
		int x = strokePos.x();

		int strokeSize = 1;
		if ( isMediumStroke )
			strokeSize = 3;
		if ( isBigStroke )
			strokeSize = 6;

		painter->drawLine( x, 18 - strokeSize, x, 18 );

		if ( isBigStroke )
		{
			QFont font;
			font.setPointSize( 7 );
			painter->setFont( font );
			QRectF textRect( x - 20, 3, 40, 10 );
			painter->drawText( textRect, Qt::AlignCenter, QString::number( abs( curPos ) ) );
		}

		curStroke += dir;
		if ( ( dir > 0 ) && ( x > width() ) )
			break;
		if ( ( dir < 0 ) && ( x < 0 ) )
			break;
	}
}

void WidgetRuler::drawRulerStrokesVer( QPainter* painter, int pixelPerSmallStroke,
	int delta, int dir )
{

	int mediumStrokeInterval = 5;
	int bigStrokeInterval = 10;

	if ( delta > 10 )
		bigStrokeInterval = mediumStrokeInterval = 5;
	if ( delta > 20 )
		bigStrokeInterval = mediumStrokeInterval = 2;
	if ( delta > 30 )
		bigStrokeInterval = mediumStrokeInterval = 1;


	int curStroke =  ( dir == -1 ) ? -1 : 0;
	forever
	{
		int curPos = curStroke * pixelPerSmallStroke;

		bool isMediumStroke = ( !( curStroke % mediumStrokeInterval ) );
		bool isBigStroke =    ( !( curStroke % bigStrokeInterval    ) );

		QPoint strokePos = graphicsView->mapFromScene( 0, curPos );
		int y = strokePos.y();

		int strokeSize = 1;
		if ( isMediumStroke )
			strokeSize = 3;
		if ( isBigStroke )
			strokeSize = 6;

		painter->drawLine( 18 - strokeSize, y, 18, y );

		if ( isBigStroke )
		{
			QFont font;
			font.setPointSize( 7 );
			painter->setFont( font );

			painter->save();
			painter->translate( 3, y + 20 );
			painter->rotate( -90 );
			QRectF textRect( 0, 0, 40, 10 );
			painter->drawText( textRect, Qt::AlignCenter, QString::number( abs( curPos ) ) );
			painter->restore();
		}

		curStroke += dir;
		if ( ( dir > 0 ) && ( y > height() ) )
			break;
		if ( ( dir < 0 ) && ( y < 0 ) )
			break;
	}
}

void WidgetRuler::mouseMoveEvent( QMouseEvent* event )
{
	if ( !drag )
		return;

	int pos = 0;
	if ( isVertRuler )
		pos = event->pos().x() - width();
	else
		pos = event->pos().y() - height();
	emit dragRuler( pos );
}

void WidgetRuler::mousePressEvent( QMouseEvent* event )
{
	drag = true;
}

void WidgetRuler::mouseReleaseEvent( QMouseEvent* event )
{
	drag = false;

	int pos = 0;
	if ( isVertRuler )
		pos = event->pos().x() - width();
	else
		pos = event->pos().y() - height();
		

	dropRuler( pos );
}
