#pragma once

class TreeNode
{
public:
	enum class NodeType
	{
		Folder,
		Sprite
	};

	TreeNode();
	TreeNode(TreeNode* parent);
	virtual ~TreeNode();
	virtual NodeType getType() const = 0;
	virtual TreeNode* clone() = 0;
	TreeNode* child(int number);
	int row() const;
	int childCount() const;
	void insertChild( int pos, TreeNode* child );
	void appendChild( TreeNode* child );
	const TreeNode* getParent() const;
	void unlinkFromParent();
	void delChildren();
	void delChild( int row );

	void lockParent();

	QString text;
	QIcon icon;

	QString toString() const;

	TreeNode* getNodeByPath(const QString& path);

	TreeNode* splitChild(const QString& path, int* row);

	bool isRoot() const
	{
		return (!parent);
	}

	bool isFolder() const
	{
		return getType() == NodeType::Folder;
	}

	bool isSprite() const
	{
		return getType() == NodeType::Sprite;
	}

protected:
	void cloneChild( TreeNode* node );

private:
	bool parentLock = false;
	TreeNode* parent = nullptr;
	QList< TreeNode* > children;
	QRegularExpression re{ R"(^((\/\d{1,10})+|\/)$)" };
};
