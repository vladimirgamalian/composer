#pragma once

class SpinBox : public QSpinBox
{
protected:
	virtual QString SpinBox::textFromValue(int value) const override;
	virtual StepEnabled stepEnabled() const override;
};
