#pragma once

class AnimationViewDelegate : public QStyledItemDelegate
{
public:
	AnimationViewDelegate();
	virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
	virtual QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const;

private:
	QPixmap framePixmap;
};
