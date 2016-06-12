#pragma once
#include "Frame.h"
#include "TreeModel/TreeNode.h"

class Sprite : public TreeNode
{
public:
	Sprite(QString name);
	virtual ~Sprite();

	virtual NodeType getType() const
	{
		return NodeType::Sprite;
	}

	virtual Sprite* clone();

	void setFrom(const Sprite* sprite);

	QList< Frame* > frames;

private:
	Sprite( const Sprite& ) = delete;
	Sprite& operator= ( Sprite const & ) = delete; 
};
