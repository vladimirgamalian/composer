#include "stdafx.h"
#include "OptionsDialog.h"

void OptionsDialog::showEvent( QShowEvent *e )
{
	QSettings settings;
	bool loadLastProjectAtStartup = settings.value( "loadLastProjectAtStartup", false ).toBool();

	ui.checkBoxLoadLastProjectAtStartup->setChecked( loadLastProjectAtStartup );

	QDialog::showEvent( e );
}

void OptionsDialog::accept()
{
	bool loadLastProjectAtStartup = ui.checkBoxLoadLastProjectAtStartup->isChecked();
	QSettings settings;
	settings.setValue( "loadLastProjectAtStartup", loadLastProjectAtStartup );
	QDialog::accept();
}
