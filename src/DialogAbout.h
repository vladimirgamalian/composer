#pragma once

#include "ui_DialogAbout.h"
#include "gitver.h"

class DialogAbout : public QDialog
{
public:

	DialogAbout( QWidget* parent ) :
	  QDialog( parent )
	  {
		  ui.setupUi( this );
		  setModal( true );
		  QString gitVer = "GIT Version: ";
		  gitVer += GIT_VER;
		  ui.labelGitVersion->setText( gitVer );
		  ui.labelGitVersion->adjustSize();
	  }



private:

	Ui::DialogAbout ui;
};
