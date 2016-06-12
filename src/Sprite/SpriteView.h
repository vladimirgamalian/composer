#pragma once
#include "Project.h"

class SpriteView : public QTreeView
{
Q_OBJECT
public:
	SpriteView( QWidget *parent = 0 );
	void setCurrentNode(QString nodePath);
	QString getCurrentNode() const;
	QString getCurrentNodeOrRoot() const;
	void save(QDomElement& node);
	void load(QDomElement& node);
	QStringList getExpandedNodeList() const;
	void setExpandedNodeList(const QStringList& expandedNodeList);

signals:
	void resetCurrentNode();

protected:
	//virtual void mousePressEvent( QMouseEvent* event );
	virtual void currentChanged(const QModelIndex& current, const QModelIndex& previous);

private:
	void recursiveScan(QModelIndex node, QList<QModelIndex>& result, bool onlyWithChildren) const;
	QModelIndex getNodeFromPath(QString path) const;
};
