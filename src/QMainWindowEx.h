#pragma once

class QMainWindowEx : public QMainWindow
{
	Q_OBJECT
public:
	QMainWindowEx( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
	void actionProjectNew();
	bool actionProjectSaveAs();
	bool actionProjectSave();
	void actionProjectOpen();
	void actionProjectOpenRecent();
	
protected:
	enum { MaxRecentFiles = 9 };

	void loadLastProject();
	void readRecentFileList();

	// TODO: remake after undo/redo completed
	virtual void setProjectModified( bool changed = true );

	virtual void newProject() = 0;
	virtual bool saveProject( QString fileName ) = 0;
	virtual bool loadProject( QString fileName ) = 0;

	virtual void onClose() = 0;
	//virtual void onRenewProject() = 0;

	void openProjectInt( QString fileName );

	QAction *recentFileActs[MaxRecentFiles];
private:

	QString projectFileName;
	bool projectModified;

	void updateTitle();
	void setCurrentProjectFile( QString fileName );
	void delRecentFiles( QString fileName );
	QString strippedName(const QString &fullFileName);
	void newProjectInt();
	bool saveChanges();
	void closeEvent( QCloseEvent *event );
	bool saveProjectToFile( QString fileName );
	bool loadProjectFromFile( QString fileName );
	void prependRecentFile( QString fileName );
};
