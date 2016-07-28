#pragma once
#include "Project.h"

class SpriteModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	SpriteModel( Project* project, QObject *parent = 0 );
	virtual ~SpriteModel();
	QModelIndex getRootIndex() const;
	void nodeDataChanged(QString nodePath);

protected:
	// minimal functions set for read-only model
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex()) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex()) const;
	virtual int columnCount( const QModelIndex& parent = QModelIndex()) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;

	// extended functions for drag-n-drop & edit
	virtual Qt::ItemFlags flags( const QModelIndex & index ) const;
	virtual bool setData( const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole */ );
	virtual Qt::DropActions supportedDropActions () const;
	virtual QStringList mimeTypes() const;
	virtual QMimeData* mimeData( const QModelIndexList& indexes ) const;
	virtual bool dropMimeData( const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent );

public slots:
	void beginInsertSpriteNode( TreeNode* parent, int row );
	void endInsertSpriteNode();
	void beginRemoveSpriteNode( TreeNode* node );
	void endRemoveSpriteNode();
	void resetModel();

signals:
	void renameNode(QString path, QString newName);
	void dragDropNode(QString oldPath, QString newPath, bool copy);
	
private:
	Project* project;
	QModelIndex createModelIndexFromNode(const TreeNode* node) const;
	TreeNode* getNodeFromModelIndex(const QModelIndex& index) const;
	bool itemIsDropEnabled( const QModelIndex& index ) const;
};
