#include "stdafx.h"
#include "ProjectData.h"

ProjectData::ProjectData( const ProjectData& src )
{
	if ( src.spriteRoot )
		spriteRoot = src.spriteRoot->clone();
}

ProjectData::~ProjectData()
{
	delete spriteRoot;
}

ProjectData& ProjectData::operator=( ProjectData const & src )
{
	delete spriteRoot;
	spriteRoot = src.spriteRoot->clone();
	return *this;
}
