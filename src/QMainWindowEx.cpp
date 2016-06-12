#include "stdafx.h"
#include "QMainWindowEx.h"

QMainWindowEx::QMainWindowEx( QWidget *parent /*= 0*/, Qt::WindowFlags flags /*= 0 */ )
	: QMainWindow( parent, flags )
{
	projectModified = false;
}

void QMainWindowEx::actionProjectNew()
{
	if ( !saveChanges() )
		return;

	newProjectInt();
	//onRenewProject();
}

void QMainWindowEx::actionProjectOpen()
{
	if ( !saveChanges() )
		return;

	QString fileName = QFileDialog::getOpenFileName( this, tr( "Open Project" ), "", tr( "Project Files (*.composer)" ) );
	if ( fileName.isNull() )
	{
		return;
	}

	openProjectInt( fileName );
}

bool QMainWindowEx::actionProjectSave()
{
	if ( projectFileName.isEmpty() )
		return actionProjectSaveAs();

	return saveProjectToFile( projectFileName );
}

bool QMainWindowEx::actionProjectSaveAs()
{
	QString fileName = QFileDialog::getSaveFileName( this, tr( "Save Project As" ), "", tr( "Project Files (*.composer)" ) );
	if ( fileName.isNull() )
		return false;

	return saveProjectToFile( fileName );
}


bool QMainWindowEx::saveProjectToFile( QString fileName )
{
	bool ok = saveProject( fileName );

	if ( ok )
	{
		setCurrentProjectFile( fileName );
		setProjectModified( false );
	}
	else
	{
		QMessageBox::critical( this, tr("save project error"), "save project error" );
	}

	return ok;
}

bool QMainWindowEx::saveChanges()
{
	if ( !projectModified )
		return true;

	QMessageBox msgBox;
	msgBox.setText( "The Project has been modified." );
	msgBox.setInformativeText( "Do you want to save your changes?" );
	msgBox.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
	msgBox.setDefaultButton( QMessageBox::Save );
	int confirm = msgBox.exec();

	if ( confirm == QMessageBox::Cancel )
		return false;

	if ( confirm == QMessageBox::Save )
	{
		if ( !actionProjectSave() )
			return false;
	}

	return true;
}

void QMainWindowEx::openProjectInt( QString fileName )
{
	if ( !loadProjectFromFile( fileName ) )
		newProjectInt();

	//onRenewProject();
}


void QMainWindowEx::actionProjectOpenRecent()
{
	QAction *action = qobject_cast< QAction* >( sender() );
	if ( action )
	{
		if ( !saveChanges() )
			return;

		openProjectInt( action->data().toString() );
	}
}


void QMainWindowEx::prependRecentFile( QString fileName )
{
	QSettings settings;
	QStringList files = settings.value( "recentFileList" ).toStringList();
	files.removeAll( fileName );
	files.prepend( fileName );
	while ( files.size() > MaxRecentFiles )
		files.removeLast();

	settings.setValue( "recentFileList", files );
}

void QMainWindowEx::setCurrentProjectFile( QString fileName )
{
	projectFileName = fileName;
	updateTitle();
	if ( fileName.isEmpty() )
		return;

	prependRecentFile( fileName );

	readRecentFileList();
}


void QMainWindowEx::readRecentFileList()
{
	QSettings settings;
	QStringList files = settings.value( "recentFileList" ).toStringList();

	int numRecentFiles = qMin( files.size(), ( int ) MaxRecentFiles );

	for ( int i = 0; i < numRecentFiles; ++i )
	{
		QString text = tr( "&%1: %2" ).arg( i + 1 ).arg( strippedName( files[ i ] ) );
		recentFileActs[ i ]->setText( text );
		recentFileActs[ i ]->setData( files[ i ] );
		recentFileActs[ i ]->setVisible( true );
	}

	for ( int j = numRecentFiles; j < MaxRecentFiles; ++j )
		recentFileActs[ j ]->setVisible( false );
}


void QMainWindowEx::delRecentFiles( QString fileName )
{
	QSettings settings;
	QStringList files = settings.value( "recentFileList" ).toStringList();
	files.removeAll( fileName );
	settings.setValue( "recentFileList", files );
	readRecentFileList();
}

QString QMainWindowEx::strippedName(const QString &fullFileName)
{
	return fullFileName;
	//return QFileInfo(fullFileName).fileName();
}

void QMainWindowEx::newProjectInt()
{
	newProject();

	setProjectModified( false );
	setCurrentProjectFile( "" );
}

void QMainWindowEx::closeEvent( QCloseEvent *event )
{
	if ( !saveChanges() )
		event->ignore();
	else
		onClose();
}

void QMainWindowEx::updateTitle()
{
	QString title = "";
	if ( projectModified )
		title += "*";
	if ( projectFileName.isEmpty() )
		title += "New Project";
	else
		title += projectFileName;
	title += " - Composer";
	this->setWindowTitle( title );
}

void QMainWindowEx::setProjectModified( bool changed )
{
	if ( projectModified == changed )
		return;

	projectModified = changed;
	updateTitle();
}

void QMainWindowEx::loadLastProject()
{
	if ( !recentFileActs[ 0 ]->isVisible() )
		return;

	QString fileName = recentFileActs[ 0 ]->data().toString();
	openProjectInt( fileName );
}

bool QMainWindowEx::loadProjectFromFile( QString fileName )
{
	if ( !QFile::exists( fileName ) )
	{
		delRecentFiles( fileName );
		QMessageBox::critical( this, tr("open project error"), "file not found" );
		return false;
	}

	if ( !loadProject( fileName ) )
	{
		delRecentFiles( fileName );
		QMessageBox::critical( this, tr("open project error"), "open project error" );
		return false;
	}

	setCurrentProjectFile( fileName );
	setProjectModified( false );
	return true;
}

