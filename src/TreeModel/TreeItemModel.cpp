#include "stdafx.h"
#include "TreeItemModel.h"

TreeItemModel::TreeItemModel( TreeNode* r, QObject *parent /*= 0 */ ) 
	: QAbstractItemModel( parent ),
	root( r )
{

}

TreeItemModel::~TreeItemModel()
{

}

QModelIndex TreeItemModel::index( int row, int column, const QModelIndex &parent /*= QModelIndex() */ ) const
{
	if ( ( row < 0 ) || ( column < 0 ) )
		return QModelIndex();
	if ( parent.isValid() && parent.column() != 0 )
		return QModelIndex();

	TreeNode* parentItem = nodeFromIndex( parent );
	TreeNode* node = parentItem->child( row );
	if ( !node )
		return QModelIndex();

	return createIndex( row, column, node );
}

QModelIndex TreeItemModel::parent( const QModelIndex & index ) const
{
 	if ( !index.isValid() )
 		return QModelIndex();

	const TreeNode* node = nodeFromIndex( index );
	const TreeNode* parentNode = node->getParent();

	if ( parentNode == root )
		return QModelIndex();

	return createIndex( parentNode->row(), 0, const_cast<TreeNode*>(parentNode) );
}

int TreeItemModel::rowCount( const QModelIndex& parent /*= QModelIndex() */ ) const
{
	TreeNode* parentItem = nodeFromIndex( parent );
	return parentItem->childCount();
}

int TreeItemModel::columnCount( const QModelIndex& /*parent*/ /*= QModelIndex() */ ) const
{
	return 1;
}

QVariant TreeItemModel::data( const QModelIndex& index, int role /*= Qt::DisplayRole */ ) const
{
	if ( index.isValid() && ( role == Qt::DisplayRole ) )
		return "Foo";

	return QVariant();
}

QModelIndexList TreeItemModel::getPersistentIndexList()
{
	return persistentIndexList();
}

TreeNode* TreeItemModel::nodeFromIndex( const QModelIndex& index ) const
{
	if ( index.isValid() )
		return static_cast< TreeNode* >( index.internalPointer() );

	return root;
}

Qt::ItemFlags TreeItemModel::flags( const QModelIndex & index ) const
{
	Qt::ItemFlags r = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled;
	if ( itemIsDropEnabled( index ) )
		r |= Qt::ItemIsDropEnabled;
	return r;
}

bool TreeItemModel::itemIsDropEnabled( const QModelIndex& /*index*/ ) const
{
	return false;
}

Qt::DropActions TreeItemModel::supportedDropActions() const
{
	return ( Qt::CopyAction + Qt::MoveAction );
}

bool TreeItemModel::dropMimeData( const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent )
{
	if ( !data || !( action == Qt::CopyAction || action == Qt::MoveAction ) )
		return false;

	bool copyAction = ( action == Qt::CopyAction );

	QStringList types = mimeTypes();
	if ( types.isEmpty() )
		return false;

	QString format = types.at( 0 );
	if ( !data->hasFormat( format ) )
		return false;

	if ( row > rowCount( parent ) )
		row = rowCount( parent );

	if ( row == -1 )
		row = rowCount( parent );

	if ( column == -1 )
		column = 0;

	QByteArray encoded = data->data( format );
	QDataStream stream( &encoded, QIODevice::ReadOnly );

	QStringList pathList;

	while ( !stream.atEnd() )
	{
		QString s;
		stream >> s;
		pathList << s;
	}

	if ( pathList.isEmpty() )
		return false;

	QModelIndex indexFrom = indexFromPath( pathList.at( 0 ) );
	if ( !indexFrom.isValid() )
		return false;

	TreeNode* node = nodeFromIndex( indexFrom );
	if ( copyAction )
	{
		node = nodeClone( node );
	}
	else
	{
		beginRemoveRows( indexFrom.parent(), indexFrom.row(), indexFrom.row() );
		node->unlinkFromParent();
		endRemoveRows();
	}


	beginInsertRows( parent, row, row );
	TreeNode* newParent = nodeFromIndex( parent );
	newParent->insertChild( row, node );
	endInsertRows();

	emit modified();

	return true;
}

TreeNode* TreeItemModel::nodeClone( TreeNode* node ) const
{
	TreeNode* newNode = node->clone();

	for ( int i = 0; i < node->childCount(); ++i )
	{
		TreeNode* c = nodeClone( node->child( i ) );
		newNode->appendChild( c );
	}

	return newNode;
}

QMimeData* TreeItemModel::mimeData( const QModelIndexList& indexes ) const
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
	for ( ; it != indexes.end(); ++it )
		stream << pathFromIndex( *it );

	data->setData( format, encoded );
	return data;
}

QStringList TreeItemModel::mimeTypes() const
{
	QStringList types;
	types << QLatin1String( "application/x-customtreeitemmodeldatalist" );
	return types;
}

QString TreeItemModel::pathFromIndex( const QModelIndex& index )
{
	QString result;
	for ( QModelIndex i = index; i.isValid(); i = i.parent() )
		result = "/" + QString::number( i.row() ) + result;
	return result;
}

QModelIndex TreeItemModel::indexFromPath( const QString& path ) const
{
	QStringList nodeList = path.split( "/", QString::SkipEmptyParts );
	if ( nodeList.isEmpty() )
		return QModelIndex();

	QModelIndex cur;
	foreach( QString node, nodeList )
	{
		bool ok = false;
		int row = node.toInt( &ok );
		if ( !ok )
			return QModelIndex();
		cur = index( row, 0, cur );
	}

	return cur;
}

QStringList TreeItemModel::pathListFromIndexes( const QModelIndexList& indexList )
{
	QStringList res;

	foreach ( QModelIndex i, indexList )
		res << pathFromIndex( i );

	return res;
}

QModelIndexList TreeItemModel::indexesFromPathList( const QStringList& pathList )
{
	QModelIndexList res;

	foreach ( QString path, pathList )
		res << indexFromPath( path );

	return res;
}
