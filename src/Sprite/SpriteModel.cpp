#include "stdafx.h"
#include "SpriteModel.h"

SpriteModel::SpriteModel( Project* project, QObject *parent /*= 0 */ )
	: QAbstractItemModel( parent ), project(project)
{
	connect( project, SIGNAL( beginInsertSpriteNode( TreeNode*, int ) ), this, SLOT( beginInsertSpriteNode( TreeNode*, int ) ) );
	connect( project, SIGNAL( endInsertSpriteNode() ), this, SLOT( endInsertSpriteNode() ) );

	connect( project, SIGNAL( beginRemoveSpriteNode( TreeNode* ) ), this, SLOT( beginRemoveSpriteNode( TreeNode* ) ) );
	connect( project, SIGNAL( endRemoveSpriteNode() ), this, SLOT( endRemoveSpriteNode() ) );
}

SpriteModel::~SpriteModel()
{

}

QModelIndex SpriteModel::index( int row, int column, const QModelIndex& parent ) const
{
	Q_ASSERT(row >= 0);
	Q_ASSERT(column == 0);

	if (!parent.isValid())
		return createModelIndexFromNode(project->projectData.spriteRoot);

	TreeNode* parentNode = getNodeFromModelIndex(parent);
	int childCount = parentNode->childCount();
	if (row >= childCount)
	{
		//qWarning() << "SpriteModel::index with out of bound row " << row;
		return QModelIndex();
	}

	return createModelIndexFromNode(parentNode->child(row));
}

QModelIndex SpriteModel::createModelIndexFromNode(const TreeNode* node ) const
{
	Q_ASSERT(node);
	return createIndex( node->row(), 0, const_cast<TreeNode*>(node) );
}

TreeNode* SpriteModel::getNodeFromModelIndex(const QModelIndex& index) const
{
	Q_ASSERT(index.isValid());
	return reinterpret_cast< TreeNode* >( index.internalPointer() );
}

QModelIndex SpriteModel::parent( const QModelIndex& index ) const
{
	Q_ASSERT(index.isValid());
	const TreeNode* parentNode = getNodeFromModelIndex(index)->getParent();
	if (!parentNode)
		return QModelIndex();
	return createModelIndexFromNode(parentNode);
}

int SpriteModel::rowCount( const QModelIndex& parent ) const
{
	return parent.isValid() ? getNodeFromModelIndex(parent)->childCount() : 1;
}

int SpriteModel::columnCount( const QModelIndex& parent ) const
{
	return 1;
}

QVariant SpriteModel::data( const QModelIndex& index, int role /*= Qt::DisplayRole */ ) const
{
	if (!index.isValid())
		return QVariant();

	TreeNode* item = getNodeFromModelIndex( index );

	if ( ( role == Qt::DisplayRole ) || ( role == Qt::EditRole ) )
		return item->text;

	if ( role == Qt::DecorationRole )
		return item->icon;

	if (role == Qt::UserRole)
		return QVariant(getNodeFromModelIndex(index)->toString());

	return QVariant();
}

Qt::ItemFlags SpriteModel::flags( const QModelIndex& index ) const
{
	Qt::ItemFlags r = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if (!itemIsRootNode(index))
		r |= Qt::ItemIsEditable | Qt::ItemIsDragEnabled;
	if ( itemIsDropEnabled( index ) )
		r |= Qt::ItemIsDropEnabled;
	return r;
}

bool SpriteModel::setData( const QModelIndex& index,
	const QVariant& value, int role /*= Qt::EditRole */ )
{
	if ( !index.isValid() )
		return false;

	if (role == Qt::EditRole)
	{
		QString newName = value.toString();
		if (!project->isValidNodeName(newName))
			return false;

		TreeNode* node = getNodeFromModelIndex(index);
		if (newName == node->text)
			return false;

		emit renameNode(node->toString(), newName);

		return true;
	}

	return false;
}

Qt::DropActions SpriteModel::supportedDropActions() const
{
	return ( Qt::CopyAction + Qt::MoveAction );
}

QStringList SpriteModel::mimeTypes() const
{
	QStringList types;
	types << QLatin1String( "application/x-customtreeitemmodeldatalist" );
	return types;
}

QMimeData* SpriteModel::mimeData( const QModelIndexList& indexes ) const
{
	if ( indexes.count() <= 0 )
		return 0;

	QStringList types = mimeTypes();
	if ( types.isEmpty() )
		return 0;

	QMimeData *data = new QMimeData();
	QString format = types.at( 0 );

	QByteArray encoded;
	QDataStream stream( &encoded, QIODevice::WriteOnly );

	QModelIndexList::ConstIterator it = indexes.begin();
	for (; it != indexes.end(); ++it)
		stream << getNodeFromModelIndex(*it)->toString();

	data->setData( format, encoded );
	return data;
}

bool SpriteModel::dropMimeData( const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent )
{
	if ( !data || !( action == Qt::CopyAction || action == Qt::MoveAction ) )
		return false;

	if (!parent.isValid())
		return false;

	bool copyAction = ( action == Qt::CopyAction );

	QStringList types = mimeTypes();
	if ( types.isEmpty() )
		return false;

	QString format = types.at( 0 );
	if ( !data->hasFormat( format ) )
		return false;

	if ( ( row < 0 )  || ( row > rowCount( parent ) ))
		row = rowCount( parent );

	QByteArray encoded = data->data( format );
	QDataStream stream( &encoded, QIODevice::ReadOnly );

	QString pathFrom;
	stream >> pathFrom;

	TreeNode* n = project->projectData.spriteRoot->getNodeByPath(pathFrom);
	if (!n)
		return false;

	QModelIndex indexFrom = createModelIndexFromNode(n);
	Q_ASSERT(indexFrom.isValid());

	TreeNode* nodeFrom = getNodeFromModelIndex( indexFrom );
	TreeNode* nodeTo = getNodeFromModelIndex( parent );

	if ( ( !copyAction ) && ( nodeFrom->getParent() == nodeTo ) && ( nodeFrom->row() < row ) )
		row--;

	Q_ASSERT(row >= 0);

	QString oldPath = nodeFrom->toString();
	QString newPath = nodeTo->toString();
	if (newPath != "/")
		newPath.append("/");
	newPath.append(QString::number(row));

	emit dragDropNode(oldPath, newPath, copyAction);

	return true;
}

bool SpriteModel::itemIsDropEnabled(const QModelIndex& index) const
{
	if (!index.isValid())
		return false;
	return getNodeFromModelIndex(index)->isFolder();
}

bool SpriteModel::itemIsRootNode(const QModelIndex& index) const
{
	if (!index.isValid())
		return false;
	return getNodeFromModelIndex(index)->isRoot();
}

void SpriteModel::beginInsertSpriteNode( TreeNode* parent, int row )
{
	Q_ASSERT(parent);
	Q_ASSERT(row >= 0);
	beginInsertRows( createModelIndexFromNode( parent ), row, row );
}

void SpriteModel::endInsertSpriteNode()
{
	endInsertRows();
}

void SpriteModel::beginRemoveSpriteNode( TreeNode* node )
{
	Q_ASSERT(node);
	int row = node->row();
	beginRemoveRows( createModelIndexFromNode( node ).parent(), row, row );
}

void SpriteModel::endRemoveSpriteNode()
{
	endRemoveRows();
}

void SpriteModel::nodeDataChanged(QString nodePath)
{
	TreeNode* n = project->getRootNode()->getNodeByPath(nodePath);
	Q_ASSERT(n);
	QModelIndex i = createModelIndexFromNode(n);
	emit dataChanged(i, i);
}

QModelIndex SpriteModel::getRootIndex() const
{
	return createModelIndexFromNode(project->getRootNode());
}

void SpriteModel::resetModel()
{
	//TODO: reset(); // deprecated
	beginResetModel();
	endResetModel();
}
