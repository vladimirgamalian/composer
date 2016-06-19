#pragma once

class WidgetRuler : public QWidget
{
	Q_OBJECT
public:
	WidgetRuler( QWidget *parent, QGraphicsView* graphicsView, bool vertRuler );

public slots:
	void repaintRulers();

signals:
	void dragRuler( int coord );
	void dropRuler( int coord );

protected:
	virtual void paintEvent( QPaintEvent* event );
	virtual void mouseMoveEvent( QMouseEvent* event );
	virtual void mousePressEvent( QMouseEvent* event );
	virtual void mouseReleaseEvent( QMouseEvent* event );

private:
	int next125( int v );
	void drawRulerStrokesHor( QPainter* painter, int pixelPerSmallStroke,
		int delta,
		int dir );
	void drawRulerStrokesVer( QPainter* painter, int pixelPerSmallStroke,
		int delta,
		int dir );
	QGraphicsView* graphicsView;
	bool drag;
	bool isVertRuler;
};
