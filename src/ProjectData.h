#pragma once
#include "TreeModel/TreeNode.h"

class ProjectData
{
public:
	ProjectData() {};
	ProjectData( const ProjectData& );
	virtual ~ProjectData();
	ProjectData& operator= ( ProjectData const & );
	TreeNode* spriteRoot = nullptr;
};
