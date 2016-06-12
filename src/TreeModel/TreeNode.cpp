#include "stdafx.h"
#include "TreeNode.h"

TreeNode::TreeNode() 
	: parent( 0 ) 
{

}

TreeNode::TreeNode(TreeNode* parent) : parent(parent)
{

}

TreeNode::~TreeNode() 
{
	delChildren();
};

void TreeNode::delChildren()
{
	qDeleteAll( children );
	children.clear();
}

TreeNode* TreeNode::child( int number )
{
	TreeNode* result = children.value(number, nullptr);
	Q_ASSERT(result);
	return result;
}

int TreeNode::row() const
{
	if ( parent )
		return parent->children.indexOf( const_cast< TreeNode* > ( this ) );

	return 0;
}

int TreeNode::childCount() const
{
	return children.count();
}

void TreeNode::insertChild( int pos, TreeNode* child )
{
	Q_ASSERT(!child->parentLock);
	child->parent = this;
	children.insert( pos, child );
}

void TreeNode::appendChild( TreeNode* child )
{
	Q_ASSERT(!child->parentLock);
	child->parent = this;
	children.append( child );
}

const TreeNode* TreeNode::getParent() const
{
	return parent;
}

void TreeNode::unlinkFromParent()
{
	Q_ASSERT(!parentLock);
	Q_ASSERT(parent);
	//if ( parent )
	{
		parent->children.removeOne( this );
		parent = nullptr;
	}
}

void TreeNode::delChild( int row )
{
	TreeNode* node = child( row );
	if ( !node )
		return;
	children.removeOne( node );
	delete node;
}

void TreeNode::lockParent()
{
	parentLock = true;
}

QString TreeNode::toString() const
{
	const TreeNode* node = this;
	QStringList parts;
	while (node->parent)
	{
		parts.prepend(QString::number(node->row()));
		node = node->parent;
	}

	return QString("/") + parts.join("/");
}

TreeNode* TreeNode::getNodeByPath(const QString& path)
{
	if (!re.match(path).hasMatch())
		return nullptr;

	QStringList nodeStrList = path.split("/", QString::SkipEmptyParts);
	QList<int> nodeList;
	std::transform(nodeStrList.begin(), nodeStrList.end(), std::back_inserter(nodeList), [](QString s) { return s.toInt(); });

	TreeNode* node = this;
	for (int r : nodeList)
	{
		if (r >= node->childCount())
			return nullptr;
		node = node->child(r);
	}
	return node;
}

TreeNode* TreeNode::splitChild(const QString& path, int* row)
{
	if (!re.match(path).hasMatch())
		return nullptr;

	QStringList nodeStrList = path.split("/", QString::SkipEmptyParts);
	QList<int> nodeList;
	std::transform(nodeStrList.begin(), nodeStrList.end(), std::back_inserter(nodeList), [](QString s) { return s.toInt(); });

	*row = nodeList.back();
	nodeList.pop_back();

	TreeNode* node = this;
	for (int r : nodeList)
	{
		if (r >= node->childCount())
			return nullptr;
		node = node->child(r);
	}

	return node;
}

void TreeNode::cloneChild( TreeNode* node )
{
	int count = childCount();
	for ( int i = 0; i < count; ++i )
		node->appendChild( child( i )->clone() );
}
