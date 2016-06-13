#pragma once
#include "Project.h"
#include "Sprite/SpriteView.h"
#include "Animation/AnimationView.h"
#include "GraphicsView.h"

class GraphicsScene : public QGraphicsScene
{
	Q_OBJECT
public:
	GraphicsScene( Project* project, SpriteView* spriteView, AnimationView* animationView, qreal x, qreal y, qreal width, qreal height, QObject *parent = 0 );
	virtual ~GraphicsScene();
	void setGraphicsView( GraphicsView* graphicsView );
	QPoint stickItem( const QRect& rect, QGraphicsItem* item );
	//TODO: probably need to move to View
	QList< int > getSelectedItemsIndexes() const;
	bool getPictureVisible( int index );
	void  setPicturePos( int index, const QPoint& pos );
	void picturesShift(int shiftX, int shiftY);
	void picturesToggleVisible();
	void onPressLeftMouse();
	void onReleaseLeftMouse();

signals:
	void dropPictures(int compositionIndex, QPoint pos, QStringList fileList);
	void movePictures(QList<Project::MovePicData> moveData);

public slots:
	void resetModel();
	void triggerStickToBorder( bool state );
	void triggerStickToPictures( bool state );
	void triggerStickToGuides( bool state );
	void setSelectedIndexes( const QList< int >& selectedIndexes );

protected:
	virtual void dragEnterEvent( QGraphicsSceneDragDropEvent* event );
	virtual void dragLeaveEvent( QGraphicsSceneDragDropEvent* event );
	virtual void dragMoveEvent( QGraphicsSceneDragDropEvent* event );
	virtual void dropEvent( QGraphicsSceneDragDropEvent* event );

private:
	enum {
		ScreenWidth = 640,
		ScreenHeight = 480,
		MagnetThresold = 10
	};

	bool isStickPos(int pos, int stickPos);
	bool across(int a, int b, int c, int d);

	Project* project;
	SpriteView* spriteView;
	AnimationView* animationView;
	bool snapToBorder = false;
	bool snapToPictures = false;
	bool snapToGuides = false;
	GraphicsView* graphicsView = nullptr;
	Frame* frameBackup = nullptr;
};
