#include "stdafx.h"
#include "GraphicsView.h"
#include "GraphicsItemPic.h"
#include "GraphicsScene.h"

GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget* parent /*= 0 */ ) : QGraphicsView(scene, parent )
{
	setDragMode( QGraphicsView::RubberBandDrag );
	//setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );
	setContextMenuPolicy( Qt::ActionsContextMenu );

	//setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setViewportUpdateMode( QGraphicsView::MinimalViewportUpdate );
	//setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
	

	//setDragEnabled( true );
	setAcceptDrops( true );
	//setDropIndicatorShown( true );

	//setDragDropMode( QAbstractItemView::DragDrop );
	//setDefaultDropAction( Qt::MoveAction );

	//setOptimizationFlags(QGraphicsView::DontClipPainter);
	//setOptimizationFlags(QGraphicsView::DontSavePainterState);
	//setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
	//setCacheMode(QGraphicsView::CacheBackground);

	comboBoxZoom = 0;
	zoomPreset = {100, 200, 300, 400, 500, 600, 800, 1000, 1200, 1600, 3200};

	rulerState = RulerState::Still;
	handScrolling = false;
	dragGuideIndexIsHor = false;

	chessboardPixmap.load( ":/misc/Resources/misc/chessboard.png" );
}

GraphicsView::~GraphicsView()
{

}

void GraphicsView::wheelEvent( QWheelEvent* event )
{
	double numDegress = event->delta() / 8.0;
	double numSteps = numDegress / 15.0;

	int newZoomIndex = curZoomIndex;
	if ( ( numSteps > 0 ) && ( ( newZoomIndex + 1 ) < zoomPreset.size() ) )
		newZoomIndex++;
	if ( ( numSteps < 0 ) && ( newZoomIndex > 0 ) )
		newZoomIndex--;

	if ( newZoomIndex == curZoomIndex )
		return;


	curZoomIndex = newZoomIndex;

	QPointF scenePointBefore = mapToScene( event->pos() );
	zoom();

	QPoint scenePointAfter = mapFromScene( scenePointBefore );
	QPoint delta = event->pos() - scenePointAfter;
	QScrollBar* hBar = horizontalScrollBar();
	QScrollBar* vBar = verticalScrollBar();
	hBar->setValue( hBar->value() + ( isRightToLeft() ? delta.x() : -delta.x() ) );
	vBar->setValue( vBar->value() - delta.y() );

	if ( comboBoxZoom )
		comboBoxZoom->setCurrentIndex( curZoomIndex );
}

void GraphicsView::drawForeground( QPainter* painter, const QRectF& rect )
{
	QPen redPen( Qt::red );
	redPen.setStyle( Qt::DashLine );
	redPen.setWidth(0);

	QPen whitePen( Qt::white );
	whitePen.setWidth(0);

	QPen bluePen( Qt::blue );
	bluePen.setStyle( Qt::DashLine );
	bluePen.setWidth(0);

	QPen gridPen( QColor( 128, 128, 128, 255 ) );
	gridPen.setStyle( Qt::DotLine );
	gridPen.setWidth(0);

	QRect r = rect.toRect();
	int left = r.left() - 1;
	int right = r.right() + 1;
	int top = r.top() - 1;
	int bottom = r.bottom() + 1;

	foreach( int y, horGuides )
	{
		painter->setPen( ( ( dragGuideIndex == y ) && ( dragGuideIndexIsHor ) ) ? redPen : whitePen );
		painter->drawLine( left, y, right, y );
		painter->setPen( bluePen );
		painter->drawLine( left, y, right, y );
	}

	foreach( int x, verGuides )
	{
		painter->setPen( ( ( dragGuideIndex == x ) && ( !dragGuideIndexIsHor ) ) ? redPen : whitePen );
		painter->drawLine( x, top, x, bottom );
		painter->setPen( bluePen );
		painter->drawLine( x, top, x, bottom );
	}

	if ( rulerState == RulerState::IntroduceHor )
	{
		qreal guidePos = mapToScene( 0, introduceGuideCoord ).y();
		painter->setPen( whitePen );
		painter->drawLine( left, guidePos, right, guidePos );
		painter->setPen( redPen );
		painter->drawLine( left, guidePos, right, guidePos );
	}

	if ( rulerState == RulerState::IntroduceVer )
	{
		qreal guidePos = mapToScene( introduceGuideCoord, 0 ).x();
		painter->setPen( whitePen );
		painter->drawLine( guidePos, top, guidePos, bottom );
		painter->setPen( redPen );
		painter->drawLine( guidePos, top, guidePos, bottom );
	}

	if ( getCurZoom() >= 1600 )
	{
		painter->setPen( gridPen );
		for ( int x = left; x < right; ++x )
			painter->drawLine( x, top, x, bottom );

		painter->setPen( gridPen );
		for ( int y = 0; y < bottom; ++y )
			painter->drawLine( left, y, right, y );
	}


	QList< QGraphicsItem* > selectedItems = scene()->selectedItems();
	
	foreach( QGraphicsItem* item, selectedItems )
	{
		if ( item->type() == QGraphicsItem::UserType + 1 )
		{
			GraphicsItemPic* i = static_cast< GraphicsItemPic* >( item );

			qreal x = i->pos().x();
			qreal y = i->pos().y();
			qreal w = i->outlineRect().width();
			qreal h = i->outlineRect().height();

			QPen blackPen( Qt::black );
			blackPen.setStyle( Qt::DashLine );
			blackPen.setWidth(0);

			QRectF objectRect = rect;
			QRectF mappedRect = painter->transform().mapRect( objectRect );
			qreal widthRatio = objectRect.width() / mappedRect.width();
			QRectF selectionRec( x, y, ( double ) w - widthRatio, ( double ) h - widthRatio );

			painter->setPen( whitePen );
			painter->drawRect(selectionRec);

			painter->setPen( blackPen );
			painter->drawRect(selectionRec);
		}

	}
}

void GraphicsView::drawBack()
{
	QPainter* backPainter = new QPainter(viewport());

	backPainter->fillRect(viewport()->rect(), Qt::gray);

	if (!reinterpret_cast<GraphicsScene*>(scene())->isSceneValid())
		return;

	QPoint sceneTopLeft = mapFromScene(QPoint());
	QPoint sceneBottomRight = mapFromScene(QPoint(640, 480));
	QPoint sceneSize = sceneBottomRight - sceneTopLeft;

	QBrush backBrush(chessboardPixmap);
	backPainter->fillRect(sceneTopLeft.x(), sceneTopLeft.y(), sceneSize.x(), sceneSize.y(), backBrush);

	delete backPainter;
}

void GraphicsView::paintEvent( QPaintEvent *event )
{
	drawBack();
	QGraphicsView::paintEvent( event );
	emit repaintRulers();
}

void GraphicsView::mousePressEvent( QMouseEvent *event )
{
	lastMouseEventPos = ( *event ).pos();

	if ( event->button() == Qt::MiddleButton )
	{
		event->accept();
		handScrolling = true;
		return;
	}
	
	QPointF p = mapToScene( event->pos() );
	int x = qRound( p.x() );
	int y = qRound( p.y() );

	rulerState = RulerState::Still;

	int newGuideIndex = getHorGuideIndex( y );
	if ( newGuideIndex != -1 )
	{
		rulerState = RulerState::DragHor;
		dragGuideIndex = newGuideIndex;
		dragGuideIndexIsHor = true;
	}
	else
	{
		newGuideIndex = getVerGuideIndex( x );
		if ( newGuideIndex != -1 )
		{
			rulerState = RulerState::DragVer;
			dragGuideIndex = newGuideIndex;
			dragGuideIndexIsHor = false;
		}
	}

	if ( ( rulerState != RulerState::DragHor ) && ( rulerState != RulerState::DragVer ) )
	{
		if (event->button() == Qt::LeftButton)
		{
			GraphicsScene* s = reinterpret_cast<GraphicsScene*>(scene());
			s->startMoving();
		}

		QGraphicsView::mousePressEvent( event );
		return;
	}

	repaint();
}

int GraphicsView::getHorGuideIndex( int y )
{
	int result = -1;
	for ( int i = 0; i < horGuides.size(); ++i )
	{
		int y2 = horGuides[ i ];
		if ( y2 == y )
		{
			result = i;
		}
	}

	return result;
}

int GraphicsView::getVerGuideIndex( int x )
{
	int result = -1;
	for ( int i = 0; i < verGuides.size(); ++i )
	{
		int x2 = verGuides[ i ];
		if ( x2 == x )
		{
			result = i;
		}
	}

	return result;
}

void GraphicsView::changeCursor( const QPoint& p )
{
	QPointF p2 = mapToScene( p );

	int horIndex = getHorGuideIndex( qRound( p2.y() ) );
	int verIndex = getVerGuideIndex( qRound( p2.x() ) );

	setCursor( Qt::ArrowCursor );
	if ( horIndex != -1 )
		setCursor( Qt::SizeVerCursor );
	if ( verIndex != -1 )
		setCursor( Qt::SizeHorCursor );

}

void GraphicsView::updateDragGuidePos( QPoint p )
{
	if ( rulerState == RulerState::DragHor )
	{
		if ( dragGuideIndex < horGuides.size() )
		{
			QPointF p2 = mapToScene( 0, p.y() );

			horGuides[ dragGuideIndex ] = qRound( p2.y() );
			repaint();
		}

		return;
	}

	if ( rulerState == RulerState::DragVer )
	{
		if ( dragGuideIndex < verGuides.size() )
		{
			QPointF p2 = mapToScene( p.x(), 0 );

			verGuides[ dragGuideIndex ] = qRound( p2.x() );
			repaint();
		}

		return;
	}
}


void GraphicsView::removeOutsidedGuide( QPoint p )
{
	if ( rulerState == RulerState::DragHor )
	{
		if ( ( p.y() < 0 ) || ( p.y() > height() ) )
		{
			horGuides.removeAt( dragGuideIndex );
		}
	}

	if ( rulerState == RulerState::DragVer )
	{
		if ( ( p.x() < 0 ) || ( p.x() > width() ) )
		{
			verGuides.removeAt( dragGuideIndex );
		}
	}
}


void GraphicsView::mouseMoveEvent( QMouseEvent * event )
{
	if ( handScrolling )
	{
		QScrollBar* hBar = horizontalScrollBar();
		QScrollBar* vBar = verticalScrollBar();
		QPoint delta = event->pos() - lastMouseEventPos;
		hBar->setValue( hBar->value() + ( isRightToLeft() ? delta.x() : -delta.x() ) );
		vBar->setValue( vBar->value() - delta.y() );

		lastMouseEventPos = ( *event ).pos();
		return;
	}

	updateDragGuidePos( event->pos() );
	QGraphicsView::mouseMoveEvent( event );
}

void GraphicsView::mouseReleaseEvent( QMouseEvent* event )
{
	handScrolling = false;
	updateDragGuidePos( event->pos() );
	removeOutsidedGuide( event->pos() );
	QGraphicsView::mouseReleaseEvent( event );
	rulerState = RulerState::Still;
	repaint();

	if (event->button() == Qt::LeftButton)
	{
		GraphicsScene* s = reinterpret_cast<GraphicsScene*>(scene());
		s->finishMoving();
	}
}

void GraphicsView::dragHorizontalRuler( int coord )
{
	QPointF p = mapToScene( 0, coord ).toPoint();
	p = mapFromScene( p );

	introduceGuideCoord = p.y();
	rulerState = RulerState::IntroduceHor;
	repaint();
}

void GraphicsView::dragVerticalRuler( int coord )
{
	QPointF p = mapToScene( coord, 0 ).toPoint();
	p = mapFromScene( p );

	introduceGuideCoord = p.x();
	rulerState = RulerState::IntroduceVer;
	repaint();
}

void GraphicsView::dropHorizontalRuler( int coord )
{
	if ( ( coord >= 0 ) && ( coord < height() ) )
	{
		QPointF p = mapToScene( 0, coord );
		horGuides.push_back( qRound( p.y() ) );
	}
	
	rulerState = RulerState::Still;
	repaint();
}

void GraphicsView::dropVerticalRuler( int coord )
{
	if ( ( coord >= 0 ) && ( coord < width() ) )
	{
		QPointF p = mapToScene( coord, 0 );
		verGuides.push_back( qRound( p.x() ) );
	}

	rulerState = RulerState::Still;
	repaint();
}

void GraphicsView::load( QDomElement& node )
{
	horGuides.clear();
	verGuides.clear();

	QDomElement graphicsView = node.firstChildElement( "graphicsView" );

	QDomElement horRuler = graphicsView.firstChildElement( "horRuler" );

	QDomElement ruler = horRuler.firstChildElement( "ruler" );
	while ( !ruler.isNull() )
	{
		bool ok = false;
		int pos = ruler.attribute( "pos", "?" ).toInt( &ok );
		if ( ok )
			horGuides.push_back( pos );
		ruler = ruler.nextSiblingElement(); 
	}


	QDomElement verRuler = graphicsView.firstChildElement( "verRuler" );

	ruler = verRuler.firstChildElement( "ruler" );
	while ( !ruler.isNull() )
	{
		bool ok = false;
		int pos = ruler.attribute( "pos", "?" ).toInt( &ok );
		if ( ok )
			verGuides.push_back( pos );
		ruler = ruler.nextSiblingElement(); 
	}
}

void GraphicsView::save( QDomElement& node )
{
	QDomElement graphicsView = node.ownerDocument().createElement( "graphicsView" );

	QDomElement horRuler = node.ownerDocument().createElement( "horRuler" );
	foreach( int y, horGuides )
	{
		QDomElement ruler = node.ownerDocument().createElement( "ruler" );
		ruler.setAttribute( "pos", y );
		horRuler.appendChild( ruler );
	}

	QDomElement verRuler = node.ownerDocument().createElement( "verRuler" );
	foreach( int x, verGuides )
	{
		QDomElement ruler = node.ownerDocument().createElement( "ruler" );
		ruler.setAttribute( "pos", x );
		verRuler.appendChild( ruler );
	}

	graphicsView.appendChild( horRuler );
	graphicsView.appendChild( verRuler );

	node.appendChild( graphicsView );
}

void GraphicsView::newProject()
{
	horGuides.clear();
	verGuides.clear();
}

void GraphicsView::getHorGuides( QList< int >& list )
{
	list = horGuides;
}

void GraphicsView::getVerGuides( QList< int >& list )
{
	list = verGuides;
}

void GraphicsView::setComboBoxZoom( QComboBox* comboBox )
{
	comboBoxZoom = comboBox;

	foreach( auto i, zoomPreset )
		comboBoxZoom->addItem( QString::number( i ) + "%" );

	

	connect( comboBoxZoom, SIGNAL( currentIndexChanged( int ) ), this, SLOT( zoomIndexChanged( int ) ) );

	curZoomIndex = zoomPreset.indexOf( 100 );
	comboBoxZoom->setCurrentIndex( curZoomIndex );
}

void GraphicsView::zoomIndexChanged( int index )
{
	if ( ( index < 0 ) || ( index >= zoomPreset.size() ) )
		return;
	if ( curZoomIndex == index )
		return;

	curZoomIndex = index;
	zoom();
}

void GraphicsView::zoom()
{
	qreal zoomPercent = zoomPreset.at( curZoomIndex );
	setTransform( QTransform::fromScale( zoomPercent / 100.0, zoomPercent / 100.0 ) );
}

int GraphicsView::getCurZoom() const
{
	if ( ( curZoomIndex < 0 ) || ( curZoomIndex >= zoomPreset.size() ) )
		return -1;

	return zoomPreset.at( curZoomIndex );
}

void GraphicsView::keyPressEvent(QKeyEvent* event)
{
	GraphicsScene* s = reinterpret_cast<GraphicsScene*>(scene());

	if ( event->key() == Qt::Key_Delete )
	{
		emit delPressed();
		return;
	}

	QPoint shift;
	switch (event->key())
	{
	case Qt::Key_Up:
		shift = { 0, -1 };
		break;
	case Qt::Key_Down:
		shift = { 0, 1 };
		break;
	case Qt::Key_Left:
		shift = { -1, 0 };
		break;
	case Qt::Key_Right:
		shift = { 1, 0 };
		break;
	}

	if (!shift.isNull())
	{
		if (lastMovingKey == -1)
		{
			lastMovingKey = event->key();
			s->startMoving();
		}

		if (lastMovingKey == event->key())
		{
			s->picturesShift(shift.x(), shift.y());
			return;
		}
	}

	if ( event->key() == Qt::Key_Space )
	{
		s->picturesToggleVisible();
		return;
	}

	QGraphicsView::keyPressEvent( event );
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
	if (!event->isAutoRepeat())
	{
		if (event->key() == lastMovingKey)
		{
			lastMovingKey = -1;
			reinterpret_cast<GraphicsScene*>(scene())->finishMoving();
		}
	}
}

void GraphicsView::redrawAll()
{
	viewport()->update();
}

void GraphicsView::removeRulers()
{
	horGuides.clear();
	verGuides.clear();
	redrawAll();
}
