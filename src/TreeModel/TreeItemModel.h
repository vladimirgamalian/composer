#pragma once

#include "TreeNode.h"

class TreeItemModel : public QAbstractItemModel
{
	Q_OBJECT
public:

	TreeItemModel( TreeNode* root, QObject *parent = 0 );
	virtual ~TreeItemModel();

	QModelIndexList getPersistentIndexList();
	static QString pathFromIndex( const QModelIndex& index );
	QModelIndex indexFromPath( const QString& path ) const;
	TreeNode* nodeFromIndex( const QModelIndex& index ) const;
	QStringList pathListFromIndexes( const QModelIndexList& indexList );
	QModelIndexList indexesFromPathList( const QStringList& pathList );

signals:
	void modified();

protected:

	virtual QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex & index ) const;
	virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;
	virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	

	virtual Qt::ItemFlags flags( const QModelIndex & index ) const;
	virtual Qt::DropActions supportedDropActions () const;
	virtual QStringList mimeTypes() const;
	virtual QMimeData* mimeData( const QModelIndexList& indexes ) const;
	virtual bool dropMimeData( const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent );



	
	virtual bool itemIsDropEnabled( const QModelIndex& index ) const;

	TreeNode* root;

private:

	
	TreeNode* nodeClone( TreeNode* node ) const;
};
