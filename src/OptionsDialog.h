#pragma once

#include "ui_options.h"

class OptionsDialog : public QDialog
{
public:

	OptionsDialog( QWidget* parent ) :
	  QDialog( parent )
	  {
		  ui.setupUi( this );
	  }

	  public slots:
		  virtual void accept();

protected:

	void showEvent( QShowEvent *e );


private:

	Ui::DialogOptions ui;
};
