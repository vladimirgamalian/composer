#include "stdafx.h"
#include "SpriteView.h"
#include "SpriteModel.h"

SpriteView::SpriteView( QWidget *parent /*= 0 */ ) :
	QTreeView( parent )
{
	setDragEnabled( true );
	setDragDropMode( QAbstractItemView::DragDrop );
	setDefaultDropAction( Qt::MoveAction );
	setAutoExpandDelay( 1000 );
	setDropIndicatorShown( true );
	//setRootIsDecorated(false);

	header()->setSectionResizeMode( QHeaderView::ResizeToContents );
	header()->setStretchLastSection( false );
	header()->hide();
}

void SpriteView::keyPressEvent(QKeyEvent* event)
{
	QTreeView::keyPressEvent(event);
	if (event->key() == Qt::Key_Delete)
		emit deleteSelectedItem();
}

void SpriteView::currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	qDebug() << "currentChanged " << current << ", " << previous;
	emit resetCurrentNode();
}

void SpriteView::recursiveScan(QModelIndex node, QList<QModelIndex>& result, bool onlyWithChildren) const
{
	//Q_ASSERT(node.isValid());

	int row = 0;
	for (;;)
	{
		QModelIndex i = node.child(row, 0);
		if (!i.isValid())
			break;
		recursiveScan(i, result, onlyWithChildren);
		row++;
	}

	if ((row > 0) || (!onlyWithChildren))
		result << node;
}

QModelIndex SpriteView::getNodeFromPath(QString path) const
{
	QList<QModelIndex> nodeList;
	QModelIndex root = model()->index(0, 0);
	recursiveScan(root, nodeList, false);

	for (auto node : nodeList)
		if (node.data(Qt::UserRole).toString() == path)
			return node;
	return QModelIndex();
}

void SpriteView::setCurrentNode(QString nodePath)
{
	//qDebug() << "setCurrentNode  " << nodePath;

	QModelIndex node = getNodeFromPath(nodePath);
	//Q_ASSERT(node.isValid());

	if (node != currentIndex())
		setCurrentIndex(node);
	else
		emit resetCurrentNode();

	scrollTo(node);
}

QStringList SpriteView::getExpandedNodeList() const
{
	QList<QModelIndex> nodeList;
	QModelIndex root = model()->index(0, 0);
	recursiveScan(root, nodeList, true);

	QStringList result;
	for (auto node : nodeList)
	{
		if (isExpanded(node))
			result << node.data(Qt::UserRole).toString();
	}

	return result;
}

void SpriteView::setExpandedNodeList(const QStringList& expandedNodeList)
{
	//collapseAll();

	QList<QModelIndex> nodeList;
	QModelIndex root = model()->index(0, 0);
	recursiveScan(root, nodeList, true);

	for (auto node : nodeList)
	{
		QString path = node.data(Qt::UserRole).toString();
		if (expandedNodeList.contains(path))
			setExpanded(node, true);
	}
}

QString SpriteView::getCurrentNode() const
{
	QModelIndex i = currentIndex();
	QVariant d = model()->data(i, Qt::UserRole);
	if (d.type() == QVariant::Invalid)
		return QString();
	Q_ASSERT(d.type() == QVariant::String);
	return d.toString();
}

QString SpriteView::getCurrentNodeOrRoot() const
{
	QString result = getCurrentNode();
	if (result.isEmpty())
		result = "/";
	return result;
}

void SpriteView::save(QDomElement& node)
{
	QDomElement domSpriteView = node.ownerDocument().createElement("spriteView");
	QDomElement expandedSpriteView = node.ownerDocument().createElement("expanded");
	QStringList expandedNodes = getExpandedNodeList();

	foreach(QString s, expandedNodes)
	{
		QDomElement expandedItem = node.ownerDocument().createElement("item");
		expandedItem.setAttribute("path", s);
		expandedSpriteView.appendChild(expandedItem);
	}

	QDomElement currentNode = node.ownerDocument().createElement("current");
	currentNode.setAttribute("path", getCurrentNode());

	domSpriteView.appendChild(currentNode);
	domSpriteView.appendChild(expandedSpriteView);
	node.appendChild(domSpriteView);
}

void SpriteView::load(QDomElement& node)
{
	QStringList expandedList;

	QDomElement domSpriteView = node.firstChildElement("spriteView");
	QDomElement expandedSpriteView = domSpriteView.firstChildElement("expanded");

	QDomElement expandedItem = expandedSpriteView.firstChildElement("item");
	while (!expandedItem.isNull())
	{
		QString path = expandedItem.attribute("path");
		expandedList << path;
		expandedItem = expandedItem.nextSiblingElement();
	}

	setExpandedNodeList(expandedList);

	QDomElement currentNode = domSpriteView.firstChildElement("current");
	QString path = currentNode.attribute("path");
	setCurrentNode(path);
}
