#pragma once
#include "Project.h"
#include "Sprite/SpriteView.h"
#include "Animation/AnimationView.h"

class CompositionModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum Roles
	{
		RolePos = Qt::UserRole + 1,
		RoleVisible = Qt::UserRole + 2,
		RoleFileName = Qt::UserRole + 3
	};

	CompositionModel( Project* project, SpriteView* spriteView, AnimationView* animationView, QObject *parent = 0 );

	bool isInternalData( const QMimeData* mimeData );
	bool checkDragEventForImage(const QMimeData *mimeData);
	void toggleVisible(const QList<int>& pics);
	void toggleVisible(const QModelIndex& index);

signals:
	void dropPictures(int compositionIndex, QPoint pos, QStringList fileList);
	void dragDrop(const QList<int>& indexes, int row, bool copyAction);
	void togglePicsVisible(QString spritePath, int frameIndex, const QList<int>& pics);

protected:
	virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
	virtual Qt::ItemFlags flags( const QModelIndex & index ) const;
	virtual bool setData( const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole */ );

	virtual Qt::DropActions supportedDropActions () const;
	virtual QStringList mimeTypes() const;

	virtual QMimeData* mimeData( const QModelIndexList& indexes ) const;

	virtual bool dropMimeData( const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent );

public slots:
	void resetModel();
	void compositionBeginRemoveRows( int beg, int end );
	void compositionEndRemoveRows();
	void compositionBeginInsertRows( int beg, int end );
	void compositionEndInsertRows();

private:
	Project* project;
	SpriteView* spriteView;
	AnimationView* animationView;
};
