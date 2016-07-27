#include "stdafx.h"
#include "CompositionModel.h"


CompositionModel::CompositionModel( Project* project, SpriteView* spriteView, AnimationView* animationView, QObject *parent /*= 0 */ )
	: QAbstractListModel( parent ), spriteView(spriteView), animationView(animationView)
{
	this->project = project;
}

int CompositionModel::rowCount( const QModelIndex &parent /*= QModelIndex() */ ) const
{
	QString spritePath = spriteView->getCurrentNode();
	if (spritePath.isEmpty())
		return 0;

	int frameIndex = animationView->getCurrentFrame();
	if (frameIndex < 0)
		return 0;

	return project->compositionGetRowCount(spritePath, frameIndex);
}

QVariant CompositionModel::data( const QModelIndex &index, int role /*= Qt::DisplayRole */ ) const
{
	if ( !index.isValid() )
		return QVariant();

	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrentFrame();

	int row = index.row();

	switch ( role )
	{
	case Qt::DisplayRole:
		return project->compositionGetFileNameRelative(spritePath, frameIndex, row);

	case RoleVisible:
		return project->compositionGetVisible(spritePath, frameIndex, row);

	case RolePos:
		return project->compositionGetPos(spritePath, frameIndex, row);

	case RoleFileName:
		return project->compositionGetFileName(spritePath, frameIndex, row);
	}

	return QVariant();
}

void CompositionModel::resetModel()
{
	//reset(); // deprecated
	//TODO: deprecated
	beginResetModel();
	endResetModel();
}

Qt::ItemFlags CompositionModel::flags( const QModelIndex & index ) const
{
	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	if ( index.isValid() )
		flags |= Qt::ItemIsDragEnabled;
	else
		flags |= Qt::ItemIsDropEnabled;

	return flags;
}

bool CompositionModel::setData( const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole */ )
{
	if ( !index.isValid() )
		return false;

	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrentFrame();

	int row = index.row();

	switch ( role )
	{
	case Qt::DisplayRole:
		project->compositionSetFileName(spritePath, frameIndex, row, value.toString());
		return true;

	case RoleVisible:
		project->compositionSetVisible(spritePath, frameIndex, row, value.toBool());
		return true;
	}

	return false;
}

//bool CompositionModel::addPicture( const QString& fileName )
//{
//	return project->compositionAddPicture( fileName );
//}

//void CompositionModel::delItems( QModelIndexList& indexes )
//{
//	QList< int > rows;
//	foreach( QModelIndex i, indexes )
//		rows << i.row();
//
//	QString spritePath = spriteView->getCurrentNode();
//	int frameIndex = animationView->getCurrent();
//
//	project->compositionDelItems(spritePath, frameIndex, rows );
//}

Qt::DropActions CompositionModel::supportedDropActions() const
{
	return ( Qt::MoveAction | Qt::CopyAction );
}

QStringList CompositionModel::mimeTypes() const
{
	QStringList types;
	types << QLatin1String( "application/x-modelcompositionitem" );
	types << QLatin1String( "text/uri-list" );

	return types;
}

QMimeData* CompositionModel::mimeData( const QModelIndexList& indexes ) const
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

bool CompositionModel::dropMimeData( const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent )
{
	if ( !data || !( action == Qt::CopyAction || action == Qt::MoveAction ) )
		return false;

	QStringList types = mimeTypes();
	if ( types.isEmpty() )
		return false;

	int rowcount = rowCount( parent );
	if ( ( row > rowcount ) || ( row == -1 ) )
		row = rowcount;

	if (data->hasUrls())
	{
		QStringList fileList;
		foreach(QUrl url, data->urls())
			if (url.isLocalFile())
				fileList << url.toLocalFile();

		emit dropPictures(row, QPoint(), fileList);
		return true;
	}

	bool copyAction = ( action == Qt::CopyAction );

	QString format = types.at( 0 );
	if ( !data->hasFormat( format ) )
		return false;

	QByteArray encoded = data->data( format );
	QDataStream stream( &encoded, QIODevice::ReadOnly );

	QList<int> rowList;

	while ( !stream.atEnd() )
	{
		int s;
		stream >> s;
		rowList << s;
	}

	emit dragDrop(rowList, row, copyAction);

	return true;
}

bool CompositionModel::isInternalData( const QMimeData* mimeData )
{
	return mimeData->hasFormat( mimeTypes().at( 0 ) );
}

void CompositionModel::compositionBeginRemoveRows( int beg, int end )
{
	beginRemoveRows( QModelIndex(), beg, end );
}

void CompositionModel::compositionEndRemoveRows()
{
	endRemoveRows();
}

void CompositionModel::compositionBeginInsertRows( int beg, int end )
{
	beginInsertRows( QModelIndex(), beg, end );
}

void CompositionModel::compositionEndInsertRows()
{
	endInsertRows();
}

bool CompositionModel::checkDragEventForImage(const QMimeData *mimeData)
{
	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrentFrame();

	return project->checkDragEventForImage(spritePath, frameIndex, mimeData);
}

void CompositionModel::toggleVisible(const QList<int>& pics)
{
	QString spritePath = spriteView->getCurrentNode();
	int frameIndex = animationView->getCurrentFrame();
	emit togglePicsVisible(spritePath, frameIndex, pics);
}

void CompositionModel::toggleVisible(const QModelIndex& index)
{
	int row = index.row();
	Q_ASSERT((row >= 0) && (row < rowCount()));
	toggleVisible(QList<int>({row}));
}
