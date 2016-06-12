#pragma once

#include "ui_SpriteMoveDialog.h"

class SpriteMoveDialog : public QDialog
{
public:

	SpriteMoveDialog( QWidget* parent ) :
	  QDialog( parent )
	  {
		  ui.setupUi( this );
		  //ui.spinBoxShiftX->setValue( 0 );
		  //ui.spinBoxShiftY->setValue( 0 );
		  setModal( true );
	  }

	  int getShiftX() const
	  {
		  return ui.spinBoxShiftX->value();
	  }

	  int getShiftY() const
	  {
		  return ui.spinBoxShiftY->value();
	  }

protected:


private:

	Ui::DialogSpriteMove ui;
};
