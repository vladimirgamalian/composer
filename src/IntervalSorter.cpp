#include "stdafx.h"
#include "IntervalSorter.h"

QList<IntervalSorter::Item> IntervalSorter::sort(const QList<int>& src)
{
	QList<Item> result;

	QList<int> l(src);
	std::sort(l.begin(), l.end());

	if (l.empty())
		return result;

	IntervalSorter::Item curItem;
	curItem.first = l[0];
	curItem.last = l[0];

	for (int i = 1; i < l.size(); ++i)
	{
		int v = l[i];
		if (v != curItem.last + 1)
		{
			result << curItem;
			curItem.first = v;
		}
		curItem.last = v;
	}
	result << curItem;

	return result;
}
