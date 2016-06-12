#include "stdafx.h"
#include "AnimationModel.h"

AnimationModel::AnimationModel( Project* project, SpriteView* spriteView, QObject *parent /*= 0 */ )
	: QAbstractListModel( parent ), project(project), spriteView(spriteView)
{
}

int AnimationModel::rowCount( const QModelIndex & /* parent = QModelIndex() */ ) const
{
	QString spritePath = spriteView->getCurrentNode();
	if (spritePath.isEmpty())
		return 0;
	return project->animGetRowCount(spritePath);
}

QVariant AnimationModel::data( const QModelIndex &index, int role /*= Qt::DisplayRole */ ) const
{
	if ( !index.isValid() )
		return QVariant();

	QString spritePath = spriteView->getCurrentNode();
	if (spritePath.isEmpty())
		return QVariant();

	int row = index.row();

	if ( role == Qt::UserRole + 3 )
		return project->animGetDuration(spritePath, row);

	if ( role == Qt::UserRole + 4 )
		return project->animGetTag(spritePath, row);

	return QVariant();
}

Qt::ItemFlags AnimationModel::flags( const QModelIndex & index ) const
{
	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	if ( index.isValid() )
		flags |= Qt::ItemIsDragEnabled;
	else
		flags |= Qt::ItemIsDropEnabled;

	return flags;
}

bool AnimationModel::setData( const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole */ )
{
	if ( !index.isValid() )
		return false;

	//if ( role == Qt::UserRole + 3 )
	//{
	//	return project->animSetDuration( index.row(), value.toInt() );
	//}

	return false;
}

void AnimationModel::resetModel()
{
	//TODO: reset(); // deprecated
	beginResetModel();
	endResetModel();
}

Qt::DropActions AnimationModel::supportedDropActions() const
{
	return ( Qt::MoveAction | Qt::CopyAction );
}

QStringList AnimationModel::mimeTypes() const
{
	QStringList types;
	types << QLatin1String( "application/x-modelanimationitem" );
	return types;
}

QMimeData* AnimationModel::mimeData( const QModelIndexList& indexes ) const
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
		stream << ( *it ).row();

	data->setData( format, encoded );
	return data;
}

bool AnimationModel::dropMimeData( const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent )
{
	if ( !data || !( action == Qt::CopyAction || action == Qt::MoveAction ) )
		return false;

	QStringList types = mimeTypes();
	if ( types.isEmpty() )
		return false;

	int rowcount = rowCount( parent );
	if ( ( row > rowcount ) || ( row == -1 ) )
		row = rowcount;

	bool copyAction = ( action == Qt::CopyAction );

	QString format = types.at( 0 );
	if ( !data->hasFormat( format ) )
		return false;

	QByteArray encoded = data->data( format );
	QDataStream stream( &encoded, QIODevice::ReadOnly );

	QList<int> indexes;

	while ( !stream.atEnd() )
	{
		int s;
		stream >> s;
		indexes << s;
	}

	QString spritePath = spriteView->getCurrentNode();
	emit dragDrop(spritePath, indexes, row, copyAction);

	return true;
}

void AnimationModel::animBeginRemoveRows( int beg, int end )
{
	beginRemoveRows( QModelIndex(), beg, end );
}

void AnimationModel::animEndRemoveRows()
{
	endRemoveRows();
}

void AnimationModel::animBeginInsertRows( int beg, int end )
{
	beginInsertRows( QModelIndex(), beg, end );
}

void AnimationModel::animEndInsertRows()
{
	endInsertRows();
}

void AnimationModel::animDataChanged( int row )
{
	QModelIndex i = index( row, 0 );
	emit dataChanged( i, i );
}
