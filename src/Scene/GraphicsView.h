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
	virtual void paintEvent( QPaintEvent *event ) override;
	virtual void drawForeground( QPainter* painter, const QRectF& rect ) override;
	virtual void wheelEvent( QWheelEvent* event ) override;
	virtual void mousePressEvent( QMouseEvent *event ) override;
	virtual void mouseMoveEvent( QMouseEvent * event ) override;
	virtual void mouseReleaseEvent( QMouseEvent* event ) override;
	virtual void keyPressEvent( QKeyEvent* event ) override;
	virtual void keyReleaseEvent(QKeyEvent *event) override;

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

	void changeCursor( const QPoint& p );
	int getHorGuideIndex( int y );
	int getVerGuideIndex( int x );
	void updateDragGuidePos( QPoint p );
	void removeOutsidedGuide( QPoint p );
	void zoom( int zoomIndex );
	int getCurZoom() const;

	int lastMovingKey = -1;
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
	QList<int> horGuides;
	QList<int> verGuides;
};
