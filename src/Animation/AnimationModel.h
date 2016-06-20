#pragma once
#include "Project.h"
#include "Sprite/SpriteView.h"

class AnimationModel : public QAbstractListModel
{
	Q_OBJECT
public:
	AnimationModel( Project* project, SpriteView* spriteView, QObject *parent = 0 );

public slots:
	void resetModel();

	//TODO: remove with parents methods
	void animBeginRemoveRows( int beg, int end );
	void animEndRemoveRows();
	void animBeginInsertRows( int beg, int end );
	void animEndInsertRows();
	void animDataChanged( int row );

signals:
	void dragDrop(QString spritePath, const QList<int>& indexes, int row, bool copyAction);
	
protected:
	virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
	virtual Qt::ItemFlags flags( const QModelIndex & index ) const;
	virtual bool setData( const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole */ );
	virtual Qt::DropActions supportedDropActions() const;
	virtual QStringList mimeTypes() const;
	virtual QMimeData* mimeData( const QModelIndexList& indexes ) const;
	virtual bool dropMimeData( const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent );

private:
	Project* project;
	SpriteView* spriteView;
};
