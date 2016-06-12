#include "stdafx.h"
#include "SpinBox.h"

QString SpinBox::textFromValue(int value) const
{
	if ( value == minimum() )
		return QString();

	return QString::number(value);
}

SpinBox::StepEnabled SpinBox::stepEnabled() const
{
	if ( value() == minimum() + 1 )
		return ( StepUpEnabled );

	return QSpinBox::stepEnabled();
}
