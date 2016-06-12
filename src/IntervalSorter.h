#pragma once

class IntervalSorter
{
public:
	struct Item
	{
		int first;
		int last;

		bool operator == (const Item& other) const
		{
			return ((first == other.first) && (last == other.last));
		}

		int size() const
		{
			return std::abs(last - first) + 1;
		}
	};

	static QList<Item> sort(QList<int> l);
};
