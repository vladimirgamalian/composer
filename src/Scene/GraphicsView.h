#pragma once
#include "Project.h"

class GraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	GraphicsView(QGraphicsScene *scene, QWidget* parent = 0 );
	virtual ~GraphicsView();

	void getHorGuides( QList< int >& list );
	void getVerGuides( QList< int >& list );

	void load( QDomElement& node );
	void save( QDomElement& node );
	void newProject();

	void setComboBoxZoom( QComboBox* comboBox );
	 
signals:
	void repaintRulers();
	void delPressed();

public slots:
	void dragHorizontalRuler( int coord );
	void dragVerticalRuler( int coord );
	void dropHorizontalRuler( int coord );
	void dropVerticalRuler( int coord );
	void removeRulers();
	void redrawAll();

private slots:
	void zoomIndexChanged( int index );

protected:
	virtual void paintEvent( QPaintEvent *event );
	virtual void drawForeground( QPainter* painter, const QRectF& rect );
	virtual void wheelEvent( QWheelEvent* event );
	virtual void mousePressEvent( QMouseEvent *event );
	virtual void mouseMoveEvent( QMouseEvent * event );
	virtual void mouseReleaseEvent( QMouseEvent* event );
	virtual void keyPressEvent( QKeyEvent* event );

private:

	struct RulerState 
	{
		enum Enum
		{
			Still,
			IntroduceHor,
			IntroduceVer,
			DragHor,
			DragVer
		};
	};

	int curZoomIndex;
	QList<int> zoomPreset;
	QComboBox* comboBoxZoom;

	QPixmap chessboardPixmap;

	bool handScrolling;
	QPoint lastMouseEventPos;

	RulerState::Enum rulerState;
	int introduceGuideCoord;
	int dragGuideIndex;
	bool dragGuideIndexIsHor;

	QList< int > horGuides;
	QList< int > verGuides;

	void changeCursor( const QPoint& p );
	int getHorGuideIndex( int y );
	int getVerGuideIndex( int x );

	void updateDragGuidePos( QPoint p );
	void removeOutsidedGuide( QPoint p );

	void zoom( int zoomIndex );

	int getCurZoom() const;
};
