#pragma once

class ScopedBool
{
public:
	ScopedBool(bool& b) : b(b)
	{
		b = true;
	}
	~ScopedBool()
	{
		b = false;
	}
private:
	bool& b;
};
