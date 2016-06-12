#pragma once

class SpinBox : public QSpinBox
{
public:
	
protected:

	virtual QString SpinBox::textFromValue(int value) const;
	virtual StepEnabled stepEnabled() const;

private:

};
