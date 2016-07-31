#pragma once
#include "Sprite/SpriteRoot.h"
#include "Sprite/SpriteFolder.h"
#include "Sprite/Sprite.h"
#include "ProjectData.h"

class Project : public QObject
{
Q_OBJECT
public:
	struct MovePicData
	{
		int index;
		QPoint oldPos;
		QPoint newPos;
	};

	Project();
	~Project();

	void load(QDomElement& node, const QString& rootPath);
	void save(QDomElement& node, const QString& rootPath);

	ProjectData projectData;

	void newProject();
	bool checkDragEventForImage(QString spritePath, int frameIndex, const QMimeData *mimeData);

	//////////////////////////////////////////////////////////////////////////

	bool isNodeDeletable(QString nodePath);
	bool isNodeInheritable(QString nodePath);

	//////////////////////////////////////////////////////////////////////////

	TreeNode* getNodeByPath(QString nodePath);
	bool isSprite(QString nodePath);
	Sprite* getSprite(QString spritePath);
	Sprite* getSpriteOrNull(QString spritePath);

	//////////////////////////////////////////////////////////////////////////
	// sprite model

	TreeNode* getRootNode() const;
	bool isValidNodeName(QString nodeName) const;
	QString spriteAddNode(QString path, QString name, TreeNode::NodeType nodeType);
	QString spriteNodeRename(QString path, QString name );
	TreeNode* spriteUnlinkNode(QString path);
	void spriteLinkNode(QString path, TreeNode* node);
	void spriteDragDrop(QString& oldPath, QString& newPath, bool copy);
	void spriteMoveSprite(QString path, int shiftX, int shiftY);

	//////////////////////////////////////////////////////////////////////////
	// animation model

	int animGetRowCount(QString spritePath);
	int animGetDuration(QString spritePath, int frameIndex);
	int animGetDurations(QString spritePath, const QList<int>& frames, bool& different);
	int animGetTotalDuration(QString spritePath);
	QString animGetTag(QString spritePath, int frameIndex);
	QString animGetTags(QString spritePath, const QList<int>& frames, bool& different);
	void animSetDuration(QString spritePath, const QList<int>& frames, int value);
	void animSetTag(QString spritePath, const QList<int>& frames, QString value);

	int animInsertFrame( QString spritePath, int row, bool before, bool copyContent );
	void animRemoveFrames(QString spritePath, const QList<int>& frames);
	bool animDragDrop(QString spritePath, const QList<int>& indexes, int row, bool copyAction);
	void animReverse(QString spritePath, const QList<int>& indexes);

	Sprite* animCloneNode(QString spritePath);
	void replaceSprite(QString spritePath, Sprite* sprite);

	bool isValidFrame(QString spritePath, int frameIndex);
	Frame* cloneFrame(QString spritePath, int frameIndex);
	Frame* getFrame(QString spritePath, int frameIndex);
	void replaceFrame(QString spritePath, int frameIndex, Frame* frame);

	bool spritesCompress(QString spritePath);

	//////////////////////////////////////////////////////////////////////////
	// composition model

	int compositionGetRowCount(QString spritePath, int frameIndex);
	Picture* compostionGetPicture(QString spritePath, int frameIndex, int row);
	QString compositionGetFileName(QString spritePath, int frameIndex, int row);
	QString compositionGetFileNameRelative(QString spritePath, int frameIndex, int row);
	bool compositionGetVisible(QString spritePath, int frameIndex, int row);
	void compositionSetVisible(QString spritePath, int frameIndex, int row, bool visible);
	QPoint compositionGetPos(QString spritePath, int frameIndex, int row);
	void compositionSetPos(QString spritePath, int frameIndex, int row, QPoint pos);
	void compositionSetFileName(QString spritePath, int frameIndex, int row, QString fileName);
	void compositionDropPictures( QString spritePath, int frameIndex, int compositionIndex, const QPoint& pos, const QStringList& fileList );
	bool compositionIsValid(QString spritePath, int frameIndex);
	void compositionPicturesToggleVisible(QString spritePath, int frameIndex, const QList<int>& pics);
	void compositionPicturesShift(QString spritePath, int frameIndex, const QList<int>& pics, int shiftX, int shiftY );
	void compositionDelItems(QString spritePath, int frameIndex, QList< int > rows);
	bool compositionDragDrop(QString spritePath, int frameIndex, const QList<int>& indexes, int row, bool copyAction);

	//////////////////////////////////////////////////////////////////////////

	void scenePictureMove(QString spritePath, int frameIndex, int index, QPoint pos);
	void scenePicturesMove(QString spritePath, int frameIndex, const QList<Project::MovePicData>& moveData, bool newPos);

	//////////////////////////////////////////////////////////////////////////

public slots:
	void lineEditFrameTagTextChanged( const QString& value );

signals:
	void beginInsertSpriteNode( TreeNode* parent, int row );
	void endInsertSpriteNode();
	void beginRemoveSpriteNode( TreeNode* node );
	void endRemoveSpriteNode();

	void animBeginRemoveRows( int beg, int end );
	void animEndRemoveRows();
	void animBeginInsertRows( int beg, int end );
	void animEndInsertRows();
	void animDataChanged();

	void compositionBeginRemoveRows( int beg, int end );
	void compositionEndRemoveRows();
	void compositionBeginInsertRows( int beg, int end );
	void compositionEndInsertRows();

	
	void animModelReset();
	void compositionModelReset();
	void sceneModelReset();

	void animSetSelect( const QList< int >& selected );
	void compositionSetSelect( const QList< int >& selected );
	
	void setLineEditFrameTag( const QString & );

private:
	QString rootPath;
	void loadSpriteNodeRecursive( QDomElement& baseXmlNode, TreeNode* node );
	void saveSpriteNodeRecursive( QDomElement& xmlNode, TreeNode* spriteNode );
	void saveSprite( QDomElement& node, Sprite* sprite );
	void saveFrame( QDomElement& node, Frame* frame );
	void loadSprite( QDomElement& node, Sprite* sprite );
	void loadFrame( QDomElement& node, Frame* sprite );
	QString getRelativePath( QString path );
	QString getAbsolutePath( QString path );
};
