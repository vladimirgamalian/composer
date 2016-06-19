#pragma once
#include "QMainWindowEx.h"
#include "ui_composer.h"
#include "Scene/GraphicsScene.h"
#include "Scene/GraphicsView.h"
#include "Project.h"
#include "Sprite/SpriteView.h"
#include "Composition/CompositionView.h"
#include "OptionsDialog.h"
#include "SpriteMoveDialog.h"
#include "Animation/AnimationView.h"
#include "DialogAbout.h"
#include "Scene/WidgetRuler.h"
#include "SpinBox.h"
#include "Sprite/SpriteModel.h"
#include "Animation/AnimationModel.h"
#include "Composition/CompositionModel.h"
#include "CommandEnvFabric.h"

class MainWindow : public QMainWindowEx
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	virtual ~MainWindow();

public slots:
	void actionSpritesNewFolder();
	void actionSpritesNewSprite();
	void actionSpritesRemoveItem();
	void actionSpritesMoveSprite();
	void actionOptions();
	void actionAnimationInsertFrameBefore();
	void actionAnimationInsertFrameAfter();
	void actionAnimationCopyFrameBefore();
	void actionAnimationCopyFrameAfter();
	void actionAnimationDeleteFrame();
	void actionSpritesCompress();
	void actionAnimationReverseFrames();
	void actionAbout();
	void actionCompositionOpenPicture();
	void actionUndo();
	void actionRedo();
	void actionRemoveRulers();

	void modified( QString description );

	void graphicsSceneSelectionChanged();
	void compositionViewSelectionChanged(const QList< int >& selectedIndexes);

	void moveSpriteDialogAccepted();

	void renameSpriteNode(QString path, QString newName);

	void dragDropSpriteNode(QString oldPath, QString newPath, bool copy);

	void dropPictures(int compositionIndex, QPoint pos, QStringList fileList);

	// animation view drag-n-drop
	//TODO: remove QString spritePath
	void animDragDrop(QString spritePath, const QList<int>& indexes, int row, bool copyAction);


	void compositionDeleteSelectedItem();

	void compositionDragDrop(const QList<int>& indexes, int row, bool copyAction);

	void sceneMovePictures(QString spritePath, int frameIndex, QList<Project::MovePicData> moveData);
	void sceneTogglePicsVisible(QString spritePath, int frameIndex, const QList<int>& pics);

private:
	Ui::composerClass ui;
	Project project;
	OptionsDialog* optionsDialog = nullptr;
	SpriteMoveDialog* spriteMoveDialog = nullptr;
	GraphicsScene* graphicsScene = nullptr;
	GraphicsView* graphicsView = nullptr;
	SpriteView* spriteView = nullptr;
	CompositionView* compositionView = nullptr;
	AnimationView* animationView = nullptr;
	DialogAbout* dialogAbout = nullptr;
	WidgetRuler* widgetRulerH = nullptr;
	WidgetRuler* widgetRulerV = nullptr;
	SpinBox* spinBoxFrameDuration = nullptr;
	QLabel* labelTotalDurationValue = nullptr;
	QLineEdit* lineEditFrameTag = nullptr;
	QListView* listViewHistory = nullptr;
	SpriteModel* spriteModel = nullptr;
	AnimationModel* animationModel = nullptr;
	CompositionModel* compositionModel = nullptr;
	QUndoStack *undoStack = nullptr;
	QUndoView *undoView = nullptr;
	QAction *undoAction = nullptr;
	QAction *redoAction = nullptr;
	CommandEnvFabric* commandEnvFabric = nullptr;
	bool graphicsScenePreventUpdateSelection = false;

	void loadSettings();
	void saveSettings();

	virtual void newProject();
	virtual bool saveProject( QString fileName );
	virtual bool loadProject( QString fileName );


	virtual void onClose();
	void saveSprites( QDomElement& node );
	void saveSprite( QDomElement& xmlNode, QStandardItem* spriteNode );
	bool isLoadLastProjectAtStartup();
	void setHistoryCheckpoint( QString description );
	void updateSpriteAction();
	void createSpriteView(SpriteModel* spriteModel);
	void createAnimationView(AnimationModel* animationModel);
	void createCompositionView(CompositionModel* compositionModel);
	void createScene();
	void createRecentActions();
	void createHistoryWidget();
	void createAboutDialog();
	void createUndoView();
	void setConnections();
	void onResetCurrentSprite();
	void uiSetupUndoRedoAction();
};
