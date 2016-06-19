#include "stdafx.h"
#include "MainWindow.h"
#include "Scene/GraphicsItemPic.h"
#include "gitver.h"
#include "Commands/Sprite/SpriteAddNodeCommand.h"
#include "Commands/Sprite/SpritesDeleteNodeCommand.h"
#include "Commands/Sprite/SpriteRenameCommand.h"
#include "Commands/Sprite/SpriteDragDropCommand.h"
#include "Commands/Animation/AnimationMoveAllCommand.h"
#include "Commands/Animation/AnimationCompressCommand.h"
#include "Commands/Animation/AnimationInsertFrameCommand.h"
#include "Commands/Animation/AnimationDelFramesCommand.h"
#include "Commands/Animation/AnimationDragDropCommand.h"
#include "Commands/Animation/AnimationReverseCommand.h"
#include "Commands/Composition/CompositionDropPicturesCommand.h"
#include "Commands/Composition/CompositionDeletePicturesCommand.h"
#include "Commands/Composition/CompositionDragDropCommand.h"
#include "Commands/Composition/CompositionMovePictures.h"
#include "Commands/Composition/CompositionTriggerPicsVisible.h"

void MainWindow::createCompositionView(CompositionModel* compositionModel)
{
	QMainWindow* window = new QMainWindow;

	QToolBar* bar = new QToolBar( window );
	bar->addAction( ui.actionCompositionOpenPicture );
	bar->setMovable( false );

	window->addToolBar( bar );
	window->setParent( ui.dockWidgetComposition );

	compositionView = new CompositionView( this );
	compositionView->setModel(compositionModel);
	compositionView->addAction( ui.actionCompositionOpenPicture );
	compositionView->setContextMenuPolicy( Qt::ActionsContextMenu );

	window->setCentralWidget( compositionView );
	ui.dockWidgetComposition->setWidget( window );

	connect( ui.actionCompositionOpenPicture, SIGNAL( triggered() ), this, SLOT( actionCompositionOpenPicture() ) );
}

void MainWindow::createAnimationView(AnimationModel* animationModel)
{
	QMainWindow* window = new QMainWindow;

	QToolBar* bar = new QToolBar( window );
	bar->addAction( ui.actionAnimationInsertBefore );
	bar->addAction( ui.actionAnimationCopyFrameBefore );
	bar->addAction( ui.actionAnimationCopyFrameAfter );
	bar->addAction( ui.actionAnimationInsertAfter );
	bar->addAction( ui.actionAnimationDeleteFrame );
	bar->addAction( ui.actionAnimationReverseFrames );
	bar->setMovable( false );

	QLabel* spinBoxFrameDurationLabel = new QLabel( "    D: " );
	spinBoxFrameDuration = new SpinBox;
	spinBoxFrameDuration->setMinimum( 0 );
	spinBoxFrameDuration->setMaximum( 9999 );
	spinBoxFrameDuration->setAccelerated( true );
	spinBoxFrameDuration->setKeyboardTracking(false);
	bar->addSeparator();
	bar->addWidget( spinBoxFrameDurationLabel );
	bar->addWidget( spinBoxFrameDuration );

	QLabel* labelFrameTag = new QLabel( "   TAG: " );
	bar->addWidget( labelFrameTag );
	lineEditFrameTag = new QLineEdit();
	lineEditFrameTag->setFixedWidth( 128 );
	lineEditFrameTag->setMaxLength( 32 );
	bar->addWidget( lineEditFrameTag );
	bar->addSeparator();

	QLabel* labelTotalDuration = new QLabel( "   Total Duration: " );
	bar->addWidget( labelTotalDuration );
	labelTotalDurationValue = new QLabel( "" );
	bar->addWidget( labelTotalDurationValue );

	window->addToolBar( bar );
	window->setParent( ui.dockWidgetAnimation );

	animationView = new AnimationView( &project, this );
	animationView->setModel(animationModel);

	animationView->addAction( ui.actionAnimationInsertBefore );
	animationView->addAction( ui.actionAnimationCopyFrameBefore );
	animationView->addAction( ui.actionAnimationCopyFrameAfter );
	animationView->addAction( ui.actionAnimationInsertAfter );
	animationView->addAction( ui.actionAnimationDeleteFrame );
	animationView->addAction( ui.actionAnimationReverseFrames );
	animationView->setContextMenuPolicy( Qt::ActionsContextMenu );

	window->setCentralWidget( animationView );
	ui.dockWidgetAnimation->setWidget( window );

	connect( ui.actionAnimationInsertBefore, SIGNAL( triggered() ), this, SLOT( actionAnimationInsertFrameBefore() ) );
	connect( ui.actionAnimationCopyFrameBefore, SIGNAL( triggered() ), this, SLOT( actionAnimationCopyFrameBefore() ) );
	connect( ui.actionAnimationCopyFrameAfter, SIGNAL( triggered() ), this, SLOT( actionAnimationCopyFrameAfter() ) );
	connect( ui.actionAnimationInsertAfter, SIGNAL( triggered() ), this, SLOT( actionAnimationInsertFrameAfter() ) );
	connect( ui.actionAnimationDeleteFrame, SIGNAL( triggered() ), this, SLOT( actionAnimationDeleteFrame() ) );
	connect( ui.actionAnimationReverseFrames, SIGNAL( triggered() ), this, SLOT( actionAnimationReverseFrames() ) );
}

void MainWindow::createSpriteView(SpriteModel* spriteModel)
{
	QMainWindow* window = new QMainWindow;

	QToolBar* bar = new QToolBar( window );
	bar->addAction( ui.actionSpritesNewFolder );
	bar->addAction( ui.actionSpritesNewSprite );
	bar->addAction( ui.actionSpritesMoveSprite );
	bar->addAction( ui.actionSpritesCompress );
	bar->addAction( ui.actionSpritesRemoveItem );
	bar->setMovable( false );

	window->addToolBar( bar );
	window->setParent( ui.dockWidgetComposition );

	spriteView = new SpriteView( this );
	spriteView->setModel(spriteModel);
	spriteView->setRootIndex(spriteModel->getRootIndex());

	spriteView->addAction( ui.actionSpritesNewFolder );
	spriteView->addAction( ui.actionSpritesNewSprite );
	spriteView->addAction( ui.actionSpritesMoveSprite );
	spriteView->addAction( ui.actionSpritesRemoveItem );
	spriteView->addAction( ui.actionSpritesCompress );
	spriteView->setContextMenuPolicy( Qt::ActionsContextMenu );

	window->setCentralWidget( spriteView );
	ui.dockWidgetSprites->setWidget( window );

	connect( ui.actionSpritesNewFolder, SIGNAL( triggered() ), this, SLOT( actionSpritesNewFolder() ) );
	connect( ui.actionSpritesNewSprite, SIGNAL( triggered() ), this, SLOT( actionSpritesNewSprite() ) );
	connect( ui.actionSpritesMoveSprite, SIGNAL( triggered() ), this, SLOT( actionSpritesMoveSprite() ) );
	connect( ui.actionSpritesRemoveItem, SIGNAL( triggered() ), this, SLOT( actionSpritesRemoveItem() ) );
	connect( ui.actionSpritesCompress, SIGNAL( triggered() ), this, SLOT( actionSpritesCompress() ) );
}

void MainWindow::createScene()
{
	graphicsScene = new GraphicsScene( &project, spriteView, animationView, 0, 0, 640, 480 );
	graphicsView = new GraphicsView(graphicsScene);
	graphicsView->scale( 1, 1 );
	graphicsView->rotate( 0 );
	graphicsView->setStyleSheet( "QGraphicsView { border-style: none; }" ); 
	graphicsScene->setGraphicsView( graphicsView );

	widgetRulerH = new WidgetRuler( this, graphicsView, false );
	widgetRulerV = new WidgetRuler( this, graphicsView, true );


	//graphicsView->setViewport( new QGLWidget );
	//centralWidget()->addWidget();
	QGridLayout* layout = new QGridLayout( this );
	layout->setSpacing( 0 );
	layout->setMargin( 0 );
	QWidget* fake = new QWidget();
	fake->setBackgroundRole( QPalette::Dark );
	fake->setFixedSize( 20, 20 );
	layout->addWidget( fake, 0, 0 );
	layout->addWidget( widgetRulerV, 1, 0 );
	layout->addWidget( widgetRulerH, 0, 1 );
	layout->addWidget( graphicsView, 1, 1 );
	QFrame* f = new QFrame( this );
	f->setLayout( layout );
	setCentralWidget( f );

	connect( graphicsView, SIGNAL( repaintRulers() ), widgetRulerH, SLOT( repaintRulers() ) );
	connect( graphicsView, SIGNAL( repaintRulers() ), widgetRulerV, SLOT( repaintRulers() ) );

	connect( widgetRulerH, SIGNAL( dragRuler( int ) ), graphicsView, SLOT( dragHorizontalRuler( int ) ) );
	connect( widgetRulerV, SIGNAL( dragRuler( int ) ), graphicsView, SLOT( dragVerticalRuler( int ) ) );

	connect( widgetRulerH, SIGNAL( dropRuler( int ) ), graphicsView, SLOT( dropHorizontalRuler( int ) ) );
	connect( widgetRulerV, SIGNAL( dropRuler( int ) ), graphicsView, SLOT( dropVerticalRuler( int ) ) );
}

void MainWindow::createRecentActions()
{
	recentFileActs[ 0 ] = ui.actionRecently1;
	recentFileActs[ 1 ] = ui.actionRecently2;
	recentFileActs[ 2 ] = ui.actionRecently3;
	recentFileActs[ 3 ] = ui.actionRecently4;
	recentFileActs[ 4 ] = ui.actionRecently5;
	recentFileActs[ 5 ] = ui.actionRecently6;
	recentFileActs[ 6 ] = ui.actionRecently7;
	recentFileActs[ 7 ] = ui.actionRecently8;
	recentFileActs[ 8 ] = ui.actionRecently9;

	for ( int i = 0; i < MaxRecentFiles; ++i )
	{
		recentFileActs[ i ]->setVisible( false );
		connect( recentFileActs[ i ], SIGNAL( triggered() ), this, SLOT( actionProjectOpenRecent() ) );
	}
}

void MainWindow::createHistoryWidget()
{
	QMainWindow* window = new QMainWindow;

	QToolBar* bar = new QToolBar( window );
	bar->addAction( ui.actionCompositionOpenPicture );
	bar->setMovable( false );

	window->addToolBar( bar );
	//window->setParent( ui.dockWidgetHistory );

	listViewHistory = new QListView;
	listViewHistory->addAction( ui.actionCompositionOpenPicture );
	listViewHistory->setContextMenuPolicy( Qt::ActionsContextMenu );

	window->setCentralWidget( listViewHistory );
	//ui.dockWidgetHistory->setWidget( window );
}

void MainWindow::createAboutDialog()
{
	dialogAbout = new DialogAbout( this );
	connect( ui.actionAbout, SIGNAL( triggered() ), this, SLOT( actionAbout() ) );
}

void MainWindow::createUndoView()
{
	undoView = new QUndoView(undoStack);
	undoView->setWindowTitle(tr("Command List"));
	undoView->show();
	undoView->setAttribute(Qt::WA_QuitOnClose, false);

	ui.dockWidgetHistory->setWidget(undoView);
}

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindowEx( parent, flags )
{
	QCoreApplication::setOrganizationName( "dell" );
	QCoreApplication::setOrganizationDomain( "dell.com" );
	QCoreApplication::setApplicationName( "dell" );

	ui.setupUi(this);

	

	connect( ui.actionSaveProject, SIGNAL( triggered() ), this, SLOT( actionProjectSave() ) );
	connect( ui.actionSaveProjectAs, SIGNAL( triggered() ), this, SLOT( actionProjectSaveAs() ) );
	connect( ui.actionOpenProject, SIGNAL( triggered() ), this, SLOT( actionProjectOpen() ) );
	connect( ui.actionNewProject, SIGNAL( triggered() ), this, SLOT( actionProjectNew() ) );

	connect( ui.actionRemoveRulers, SIGNAL( triggered() ), this, SLOT( actionRemoveRulers() ) );
	

	undoStack = new QUndoStack(this);
	createUndoView();
	undoAction = undoStack->createUndoAction(this, tr("&Undo"));
	undoAction->setShortcuts(QKeySequence::Undo);
	redoAction = undoStack->createRedoAction(this, tr("&Redo"));
	redoAction->setShortcuts(QKeySequence::Redo);
	uiSetupUndoRedoAction();


	createHistoryWidget();
	createAboutDialog();
	

	spriteModel = new SpriteModel( &project, this );
	createSpriteView(spriteModel);

	animationModel = new AnimationModel( &project, spriteView, this );
	createAnimationView(animationModel);

	compositionModel = new CompositionModel( &project, spriteView, animationView, this );
	createCompositionView(compositionModel);

	commandEnvFabric = new CommandEnvFabric(&project, spriteView, animationView, compositionView);

	createScene();

	optionsDialog = new OptionsDialog( this );
	connect( ui.actionOptions, SIGNAL( triggered() ), this, SLOT( actionOptions() ) );

	spriteMoveDialog = new SpriteMoveDialog( this );


	connect( spriteMoveDialog, SIGNAL( accepted() ), this, SLOT( moveSpriteDialogAccepted() ) );


	

	createRecentActions();
	readRecentFileList();








//	connect( compositionModel, SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( modelCompositionDataChanged( const QModelIndex&, const QModelIndex& ) ) );
	

	loadSettings();

	graphicsScene->triggerStickToBorder( ui.actionSnapToBorder->isChecked() );
	graphicsScene->triggerStickToPictures( ui.actionSnapToPictures->isChecked() );
	graphicsScene->triggerStickToGuides( ui.actionSnapToGuides->isChecked() );

	connect( ui.actionSnapToBorder, SIGNAL( triggered( bool ) ), graphicsScene, SLOT( triggerStickToBorder( bool ) ) );
	connect( ui.actionSnapToPictures, SIGNAL( triggered( bool ) ), graphicsScene, SLOT( triggerStickToPictures( bool ) ) );
	connect( ui.actionSnapToGuides, SIGNAL( triggered( bool ) ), graphicsScene, SLOT( triggerStickToGuides( bool ) ) );



	QComboBox* comboBoxZoom = new QComboBox;
	ui.mainToolBar->addSeparator();
	ui.mainToolBar->addWidget( new QLabel( "    " ) );
	ui.mainToolBar->addWidget( comboBoxZoom );
	ui.mainToolBar->addWidget( new QLabel( "   " ) );
	graphicsView->setComboBoxZoom( comboBoxZoom );


	//QLabel* spinBoxPicXLabel = new QLabel( "    X: " );
	//QLabel* spinBoxPicYLabel = new QLabel( "    Y: " );
	//QSpinBox* spinBoxPicX = new QSpinBox;
	//QSpinBox* spinBoxPicY = new QSpinBox;
	//ui.mainToolBar->addSeparator();
	//ui.mainToolBar->addWidget( spinBoxPicXLabel );
	//ui.mainToolBar->addWidget( spinBoxPicX );
	//ui.mainToolBar->addWidget( spinBoxPicYLabel );
	//ui.mainToolBar->addWidget( spinBoxPicY );


	
	//compositionView->setGraphicsScene( &project, graphicsScene );

	//connect( graphicsView, SIGNAL( delPressed() ), compositionView, SLOT( delPressed() ) );




	
	//connect( ui.actionUndo, SIGNAL( triggered() ), this, SLOT( actionUndo() ) );
	//connect( ui.actionRedo, SIGNAL( triggered() ), this, SLOT( actionRedo() ) );

	//connect(&project, &Project::setActiveNode, this, &MainWindow::setActiveNode);

	connect( &project, SIGNAL( animModelReset() ), animationModel, SLOT( animModelReset() ) );
	connect( &project, SIGNAL( animSetSelect( const QList< int >& ) ), animationView, SLOT( animSetSelect( const QList< int >& ) ) );

	connect( &project, SIGNAL( compositionModelReset() ), compositionModel, SLOT( resetModel() ) );
	connect( &project, SIGNAL( sceneModelReset() ), graphicsScene, SLOT( resetModel() ) );
	

	connect( &project, SIGNAL( animBeginRemoveRows( int, int ) ), animationModel, SLOT( animBeginRemoveRows( int, int ) ) );
	connect( &project, SIGNAL( animEndRemoveRows() ), animationModel, SLOT( animEndRemoveRows() ) );

	connect( &project, SIGNAL( animBeginInsertRows( int, int ) ), animationModel, SLOT( animBeginInsertRows( int, int ) ) );
	connect( &project, SIGNAL( animEndInsertRows() ), animationModel, SLOT( animEndInsertRows() ) );



	connect( &project, SIGNAL( compositionBeginRemoveRows( int, int ) ), compositionModel, SLOT( compositionBeginRemoveRows( int, int )  ) );
	connect( &project, SIGNAL( compositionEndRemoveRows() ), compositionModel, SLOT( compositionEndRemoveRows() ) );

	connect( &project, SIGNAL( compositionBeginInsertRows( int, int ) ), compositionModel, SLOT( compositionBeginInsertRows( int, int )  ) );
	connect( &project, SIGNAL( compositionEndInsertRows() ), compositionModel, SLOT( compositionEndInsertRows() ) );


	connect( &project, SIGNAL( spriteModelReset() ), spriteModel, SLOT( resetModel() ) );

	connect( &project, SIGNAL( sceneSetSelect( const QList< int >& ) ), graphicsScene, SLOT( setSelectedIndexes( const QList< int >& ) ) );

	
	connect( &project, SIGNAL( compositionSetSelect( const QList< int >& ) ), compositionView, SLOT( setSelect( const QList< int >& ) ) );

	connect( &project, SIGNAL( setFrameDurationSpinner( int ) ), spinBoxFrameDuration, SLOT( setValue( int ) ) );
	connect( spinBoxFrameDuration, SIGNAL( valueChanged( int ) ), &project, SLOT( frameDurationSpinnerChanged( int ) ) );
	
	connect( &project, SIGNAL( setSpriteTotalDuration( int ) ), labelTotalDurationValue, SLOT( setNum( int ) ) );


	connect( &project, SIGNAL( setLineEditFrameTag( const QString & ) ), lineEditFrameTag, SLOT( setText( const QString & ) ) );
	connect( lineEditFrameTag, SIGNAL( textChanged( const QString & ) ), &project, SLOT( lineEditFrameTagTextChanged( const QString & ) ) );
	

	connect( &project, SIGNAL( animDataChanged( int ) ), animationModel, SLOT( animDataChanged( int ) ) );

	connect( &project, SIGNAL( sceneModelRedraw() ), graphicsView, SLOT( redrawAll() ) );

	connect( &project, SIGNAL( compositionModelRedraw() ), compositionView, SLOT( redrawAll() ) );

	
	connect(spriteModel, &SpriteModel::renameNode, this, &MainWindow::renameSpriteNode);
	connect(spriteModel, &SpriteModel::dragDropNode, this, &MainWindow::dragDropSpriteNode);
	

	connect(animationView, &AnimationView::deleteSelectedItem, this, &MainWindow::actionAnimationDeleteFrame);
	
	connect(compositionModel, &CompositionModel::dropPictures, this, &MainWindow::dropPictures);
	connect(graphicsScene, &GraphicsScene::dropPictures, this, &MainWindow::dropPictures);


	setConnections();


	if ( isLoadLastProjectAtStartup() )
		loadLastProject();

	//updateSpriteAction();
}

MainWindow::~MainWindow()
{
	delete commandEnvFabric;
}

bool MainWindow::isLoadLastProjectAtStartup()
{
	QSettings settings;
	return settings.value( "loadLastProjectAtStartup", false ).toBool();
}

void MainWindow::newProject()
{
//	spriteModel->clear();
	project.newProject();
//	graphicsView->newProject();
}

void MainWindow::loadSettings()
{
	QSettings settings;
	restoreGeometry( settings.value( "geometry" ).toByteArray() );
	restoreState( settings.value( "windowState" ).toByteArray() );

	ui.actionSnapToBorder->setChecked( settings.value( "snapToBorder" ).toBool() );
	ui.actionSnapToGuides->setChecked( settings.value( "snapToGuides" ).toBool() );
	ui.actionSnapToPictures->setChecked( settings.value( "snapToPictures" ).toBool() );
}

void MainWindow::onClose()
{
	saveSettings();
}

void MainWindow::saveSettings()
{
	QSettings settings;
	settings.setValue( "geometry", saveGeometry() );
	settings.setValue( "windowState", saveState() );
	settings.setValue( "snapToBorder", ui.actionSnapToBorder->isChecked() );
	settings.setValue( "snapToGuides", ui.actionSnapToGuides->isChecked() );
	settings.setValue( "snapToPictures", ui.actionSnapToPictures->isChecked() );
}

bool MainWindow::saveProject( QString fileName )
{
	if ( fileName.isEmpty() )
		return false;

	try
	{
		QDir d = QFileInfo( fileName ).absoluteDir();
		QString projectDir = QDir::cleanPath( d.absolutePath() );

		QDomDocument doc;
		QDomProcessingInstruction xmlHeaderPI = doc.createProcessingInstruction( "xml", "version=\"1.0\" " );
		doc.appendChild( xmlHeaderPI );

		QDomElement root = doc.createElement( "project" );
		root.setAttribute( "version", 1 );
		doc.appendChild( root );

		project.save( root, projectDir );
		spriteView->save(root);
		graphicsView->save( root );

		QFile outFile( fileName );
		if ( !outFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
			throw std::runtime_error( "cant open project file" );

		QTextStream stream( &outFile );
		stream << doc.toString();
	}
	catch ( ... )
	{
		return false;
	}


	return true;
}

bool MainWindow::loadProject( QString fileName )
{
	try
	{
		QDir d = QFileInfo( fileName ).absoluteDir();
		QString projectDir = QDir::cleanPath( d.absolutePath() );

		QFile file( fileName );
		if ( !file.open( QIODevice::ReadOnly ) )
			throw std::runtime_error( "cant open project file" );

		QDomDocument doc;
		if ( !doc.setContent( &file ) ) 
			throw std::runtime_error( "cant load project file" );

		QDomElement root = doc.documentElement();
		if ( root.tagName() != "project" )
			throw std::runtime_error( "invalid project format" );

		project.load( root, projectDir );
		spriteView->load(root);
		graphicsView->load( root );
	}
	catch (...)
	{
		return false;		
	}

	return true;
}

void MainWindow::actionSpritesNewFolder()
{
	QString nodePath = spriteView->getCurrentNodeOrRoot();
	SpriteAddNodeCommand *undoCommand = new SpriteAddNodeCommand(commandEnvFabric->getCommandEnv(), nodePath, TreeNode::NodeType::Folder);
	undoStack->push(undoCommand);
}

void MainWindow::actionSpritesRemoveItem()
{
	QString spritePath = spriteView->getCurrentNode();
	SpritesDeleteNodeCommand *undoCommand = new SpritesDeleteNodeCommand(commandEnvFabric->getCommandEnv(), spritePath);

	undoStack->push(undoCommand);
}

void MainWindow::actionSpritesNewSprite()
{
	QString nodePath = spriteView->getCurrentNodeOrRoot();
	SpriteAddNodeCommand *undoCommand = new SpriteAddNodeCommand(commandEnvFabric->getCommandEnv(), nodePath, TreeNode::NodeType::Sprite);
	undoStack->push(undoCommand);
}

void MainWindow::actionSpritesMoveSprite()
{
	spriteMoveDialog->show();
}

void MainWindow::moveSpriteDialogAccepted()
{
	QString spritePath = spriteView->getCurrentNodeOrRoot();
	AnimationMoveAllCommand *undoCommand = new AnimationMoveAllCommand(commandEnvFabric->getCommandEnv(), spritePath, spriteMoveDialog->getShiftX(), spriteMoveDialog->getShiftY());
	undoStack->push(undoCommand);
}

void MainWindow::renameSpriteNode(QString path, QString newName)
{
	SpriteRenameCommand *undoCommand = new SpriteRenameCommand(commandEnvFabric->getCommandEnv(), spriteModel, path, newName);
	undoStack->push(undoCommand);
}

void MainWindow::dragDropSpriteNode(QString oldPath, QString newPath, bool copy)
{
	SpriteDragDropCommand *spriteDragDropCommand = new SpriteDragDropCommand(commandEnvFabric->getCommandEnv(), oldPath, newPath, copy);
	undoStack->push(spriteDragDropCommand);
}

void MainWindow::dropPictures(int compositionIndex, QPoint pos, QStringList fileList)
{
	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrent();
	
	CompositionDropPicturesCommand* compositionDropPicturesCommand
		= new CompositionDropPicturesCommand(commandEnvFabric->getCommandEnv(),
			spritePath, frameIndex, compositionIndex, pos, fileList);
	undoStack->push(compositionDropPicturesCommand);
}

void MainWindow::animDragDrop(QString spritePath, const QList<int>& indexes, int row, bool copyAction)
{
	QString path = spriteView->getCurrentNode();
	AnimationDragDropCommand *undoCommand = new AnimationDragDropCommand(commandEnvFabric->getCommandEnv(), path, indexes, row, copyAction);
	undoStack->push(undoCommand);
}

void MainWindow::compositionDeleteSelectedItem()
{
	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrent();
	QList<int> pictures = compositionView->getSelected();

	CompositionDeletePicturesCommand *undoCommand = new CompositionDeletePicturesCommand(commandEnvFabric->getCommandEnv(),
		spritePath, frameIndex, pictures);
	undoStack->push(undoCommand);
}

void MainWindow::compositionDragDrop(const QList<int>& indexes, int row, bool copyAction)
{
	int index = animationView->getCurrent();
	QString path = spriteView->getCurrentNode();
	CompositionDragDropCommand *undoCommand = new CompositionDragDropCommand(commandEnvFabric->getCommandEnv(),
		path, index, indexes, row, copyAction);
	undoStack->push(undoCommand);
}

void MainWindow::sceneMovePictures(QString spritePath, int frameIndex, QList<Project::MovePicData> moveData)
{
	CompositionMovePictures *undoCommand = new CompositionMovePictures(commandEnvFabric->getCommandEnv(),
		spritePath, frameIndex, moveData);
	undoStack->push(undoCommand);
}

void MainWindow::sceneTogglePicsVisible(QString spritePath, int frameIndex, const QList<int>& pics)
{
	CompositionTriggerPicsVisible *undoCommand = new CompositionTriggerPicsVisible(commandEnvFabric->getCommandEnv(),
		spritePath, frameIndex, pics);
	undoStack->push(undoCommand);
}

void MainWindow::actionOptions()
{
	optionsDialog->show();
}

void MainWindow::updateSpriteAction()
{
	QString nodePath = spriteView->getCurrentNodeOrRoot();

	bool isDeletable = project.isNodeDeletable(nodePath);
	bool isInheritable = project.isNodeInheritable(nodePath);
	bool isSprite = project.isSprite(nodePath);

	ui.actionSpritesNewFolder->setEnabled( isInheritable );
	ui.actionSpritesNewSprite->setEnabled( isInheritable );
	ui.actionSpritesMoveSprite->setEnabled( isSprite );
	ui.actionSpritesRemoveItem->setEnabled( isDeletable );
	ui.actionSpritesCompress->setEnabled( isSprite );
}

void MainWindow::actionAnimationInsertFrameBefore()
{
	int index = animationView->getCurrent();
	QString path = spriteView->getCurrentNode();
	AnimationInsertFrameCommand *undoCommand = new AnimationInsertFrameCommand(commandEnvFabric->getCommandEnv(), path, index, true, false);
	undoStack->push(undoCommand);
//	animationView->setSelected(undoCommand->getNewIndex());
}

void MainWindow::actionAnimationInsertFrameAfter()
{
	int index = animationView->getCurrent();
	QString path = spriteView->getCurrentNode();
	AnimationInsertFrameCommand *undoCommand = new AnimationInsertFrameCommand(commandEnvFabric->getCommandEnv(), path, index, false, false);
	undoStack->push(undoCommand);
//	animationView->setSelected(undoCommand->getNewIndex());
}

void MainWindow::actionAnimationDeleteFrame()
{
	QString spritePath = spriteView->getCurrentNode();
	QList<int> frames = animationView->getSelected();
	AnimationDelFramesCommand *undoCommand = new AnimationDelFramesCommand(commandEnvFabric->getCommandEnv(), spritePath, frames);
	undoStack->push(undoCommand);
}

void MainWindow::actionSpritesCompress()
{
	QString spritePath = spriteView->getCurrentNode();
	AnimationCompressCommand *undoCommand = new AnimationCompressCommand(commandEnvFabric->getCommandEnv(), spritePath);
	undoStack->push(undoCommand);
}

void MainWindow::actionAbout()
{
	dialogAbout->show();
}

void MainWindow::actionCompositionOpenPicture()
{
	QString fileName = QFileDialog::getOpenFileName( this, tr( "Add picture" ), "", tr( "PNG pictures (*.png);;JPG pictures (*.jpg);;BMP pictures (*.bmp)" ) );
	if ( fileName.isNull() )
		return;
	//TODO: add picture
	//compositionModel->addPicture( fileName );
}

void MainWindow::actionAnimationCopyFrameBefore()
{
	int index = animationView->getCurrent();
	QString path = spriteView->getCurrentNode();
	AnimationInsertFrameCommand *undoCommand = new AnimationInsertFrameCommand(commandEnvFabric->getCommandEnv(), path, index, true, true);
	undoStack->push(undoCommand);
} 

void MainWindow::actionAnimationCopyFrameAfter()
{
	int index = animationView->getCurrent();
	QString path = spriteView->getCurrentNode();
	AnimationInsertFrameCommand *undoCommand = new AnimationInsertFrameCommand(commandEnvFabric->getCommandEnv(), path, index, false, true);
	undoStack->push(undoCommand);
}

void MainWindow::setHistoryCheckpoint( QString description )
{
//	historyModel->checkpoint( description );
}

void MainWindow::modified( QString description )
{
	setHistoryCheckpoint( description );
	listViewHistory->reset();
	setProjectModified( true );
}

void MainWindow::actionUndo()
{
	qDebug() << "actionUndo";
//	historyModel->undo();
//	undoAction();
}

void MainWindow::actionRedo()
{
	qDebug() << "actionRedo";
//	historyModel->redo();
}

void MainWindow::graphicsSceneSelectionChanged()
{
	if (graphicsScenePreventUpdateSelection)
		return;

	QList<int> selectedItemsIndexes = graphicsScene->getSelectedItemsIndexes();
	graphicsScenePreventUpdateSelection = true;
	compositionView->setSelected(selectedItemsIndexes);
	graphicsScenePreventUpdateSelection = false;
}

void MainWindow::compositionViewSelectionChanged(const QList< int >& selectedIndexes)
{
	if (graphicsScenePreventUpdateSelection)
		return;

	graphicsScenePreventUpdateSelection = true;
	graphicsScene->setSelectedIndexes(selectedIndexes);
	graphicsScenePreventUpdateSelection = false;
}

void MainWindow::actionAnimationReverseFrames()
{
	AnimationReverseCommand *undoCommand = new AnimationReverseCommand(commandEnvFabric->getCommandEnv(),
		spriteView->getCurrentNode(), animationView->getSelected());
	undoStack->push(undoCommand);
}

void MainWindow::actionRemoveRulers()
{
	graphicsView->removeRulers();
}

void MainWindow::setConnections()
{
	//connect(spriteView, &SpriteView::resetCurrentNode, animationModel, &AnimationModel::resetModel);
	//connect(spriteView, &SpriteView::resetCurrentNode, compositionModel, &CompositionModel::resetModel);
	//connect(spriteView, &SpriteView::resetCurrentNode, graphicsScene, &GraphicsScene::resetModel);
	connect(spriteView, &SpriteView::resetCurrentNode, this, &MainWindow::onResetCurrentSprite);

	connect(animationView, &AnimationView::resetCurrentFrame, compositionModel, &CompositionModel::resetModel);
	connect(animationView, &AnimationView::resetCurrentFrame, graphicsScene, &GraphicsScene::resetModel);


	connect(animationModel, &AnimationModel::dragDrop, this, &MainWindow::animDragDrop);


	connect(compositionView, &CompositionView::deleteSelectedItem, this, &MainWindow::compositionDeleteSelectedItem);
	connect(graphicsView, &GraphicsView::delPressed, this, &MainWindow::compositionDeleteSelectedItem);

	connect(compositionModel, &CompositionModel::dragDrop, this, &MainWindow::compositionDragDrop);
	connect(compositionModel, &CompositionModel::togglePicsVisible, this, &MainWindow::sceneTogglePicsVisible);

	connect(graphicsScene, &GraphicsScene::selectionChanged, this, &MainWindow::graphicsSceneSelectionChanged);
	connect(compositionView, &CompositionView::selectChanged, this, &MainWindow::compositionViewSelectionChanged);

	connect(graphicsScene, &GraphicsScene::movePictures, this, &MainWindow::sceneMovePictures);
	connect(graphicsScene, &GraphicsScene::togglePicsVisible, this, &MainWindow::sceneTogglePicsVisible);

	connect(animationView, &AnimationView::frameDuration, this, &MainWindow::frameDuration);
}

void MainWindow::onResetCurrentSprite()
{
	animationModel->resetModel();
	animationView->setCurrentFrame(0);
	animationView->updateDurations();
	updateSpriteAction();
}

void MainWindow::uiSetupUndoRedoAction()
{
	undoAction->setIcon(QIcon(":/composer/Resources/icons/undo.png"));
	redoAction->setIcon(QIcon(":/composer/Resources/icons/redo.png"));

	ui.mainToolBar->addAction(undoAction);
	ui.mainToolBar->addAction(redoAction);

	ui.menu_Edit->addSeparator();
	ui.menu_Edit->addAction(undoAction);
	ui.menu_Edit->addAction(redoAction);
}

void MainWindow::frameDuration(bool enabled, bool different, int v)
{
	if (enabled)
	{
		spinBoxFrameDuration->setValue(different ? 0 : v);
		spinBoxFrameDuration->setEnabled(true);
	}
	else
	{
		spinBoxFrameDuration->setValue(0);
		spinBoxFrameDuration->setEnabled(false);
	}
}
