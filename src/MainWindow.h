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
	void actionAnimationReverseFrames();
	void actionAnimationCompress();
	void actionAbout();
	void actionCompositionOpenPicture();
	void actionRemoveRulers();
	void graphicsSceneSelectionChanged();
	void compositionViewSelectionChanged();
	void moveSpriteDialogAccepted();
	void renameSpriteNode(QString path, QString newName);
	void dragDropSpriteNode(QString oldPath, QString newPath, bool copy);
	void dropPictures(int compositionIndex, QPoint pos, QStringList fileList);
	//TODO: remove spritePath
	void animDragDrop(QString spritePath, const QList<int>& indexes, int row, bool copyAction);
	void compositionDeleteSelectedItem();
	void compositionDragDrop(const QList<int>& indexes, int row, bool copyAction);
	void sceneMovePictures(QString spritePath, int frameIndex, QList<Project::MovePicData> moveData);
	void sceneTogglePicsVisible(QString spritePath, int frameIndex, const QList<int>& pics);
	void frameDurationSpinnerChanged(int value);
	void frameSelectChanged();
	void labelTagChanged();
	void invalidateAll();
	void updatePictureInfo();

private:
	void loadSettings();
	void saveSettings();
	virtual void newProject() override;
	virtual bool saveProject(QString fileName) override;
	virtual bool loadProject(QString fileName) override;
	virtual void onClose() override;
	virtual void setProjectModified(bool changed = true) override;
	bool isLoadLastProjectAtStartup();
	void updateSpriteActions();
	void updateAnimationActions();
	void updateFrameDurationSpinBox();
	void updateFrameTagLineEdit();
	void createSpriteView(SpriteModel* model);
	void createAnimationView(AnimationModel* model);
	void createCompositionView(CompositionModel* model);
	void createScene();
	void createRecentActions();
	void createAboutDialog();
	void createUndoView();
	void setConnections();
	void onResetCurrentSprite();
	void uiSetupUndoRedoAction();
	void updateFrameTotalDuration();
	void undoStackCleanChanged(bool clean);
	void clearUndoHistory();
	void updateCompositionActions();
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
	QLabel* labelTotalDuration = nullptr;
	QLabel* labelTotalDurationValue = nullptr;
	QLineEdit* lineEditFrameTag = nullptr;
	SpriteModel* spriteModel = nullptr;
	AnimationModel* animationModel = nullptr;
	CompositionModel* compositionModel = nullptr;
	QUndoStack *undoStack = nullptr;
	QUndoView *undoView = nullptr;
	QAction *undoAction = nullptr;
	QAction *redoAction = nullptr;
	CommandEnvFabric* commandEnvFabric = nullptr;
	bool graphicsScenePreventUpdateSelection = false;
	bool preventFrameDurationChange = false;
};
