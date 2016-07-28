#include "stdafx.h"
#include "Project.h"
#include "IntervalSorter.h"

Project::Project()
{
	projectData.spriteRoot = new SpriteFolder( "root" );
	projectData.spriteRoot->lockParent();
}

Project::~Project()
{
	//delete projectData.spriteRoot;
}

void Project::load( QDomElement& node, const QString& rootPath )
{
	this->rootPath = rootPath;
	projectData.spriteRoot->delChildren();
	QDomElement sprites = node.firstChildElement( "sprites" );
	loadSpriteNodeRecursive( sprites, projectData.spriteRoot );
}

void Project::save(QDomElement& node, const QString& rootPath)
{
	//TODO: probably remove rootPath from Prokect
	this->rootPath = rootPath;

	QDomElement sprites = node.ownerDocument().createElement("sprites");
	saveSpriteNodeRecursive(sprites, projectData.spriteRoot);
	node.appendChild(sprites);
}

void Project::loadSpriteNodeRecursive(QDomElement& baseXmlNode, TreeNode* node)
{
	QDomElement xmlNode = baseXmlNode.firstChildElement();
	while ( !xmlNode.isNull() ) 
	{
		if ( xmlNode.tagName() == "folder" )
		{
			SpriteFolder* folder = new SpriteFolder( xmlNode.attribute( "name" ) );
			node->appendChild( folder );
			loadSpriteNodeRecursive( xmlNode, folder );
		}

		if ( xmlNode.tagName() == "sprite" )
		{
			Sprite* sprite = new Sprite( xmlNode.attribute( "name" ) );
			loadSprite( xmlNode, sprite );
			node->appendChild( sprite );
		}

		xmlNode = xmlNode.nextSiblingElement(); 
	} 
}

void Project::saveSpriteNodeRecursive( QDomElement& xmlNode, TreeNode* spriteNode )
{
	for ( int row = 0; row < spriteNode->childCount(); ++row )
	{
		TreeNode* item = spriteNode->child( row );

		if ( item->isFolder())
		{
			QDomElement node = xmlNode.ownerDocument().createElement( "folder" );
			node.setAttribute( "name", item->text );
			saveSpriteNodeRecursive( node, item );
			xmlNode.appendChild( node );
		}

		if ( item->getType() == TreeNode::NodeType::Sprite)
		{
			QDomElement node = xmlNode.ownerDocument().createElement( "sprite" );
			node.setAttribute( "name", item->text );
			Sprite* sprite = reinterpret_cast<Sprite*>(item);
			saveSprite( node, sprite );
			xmlNode.appendChild( node );
		}
	}
}

void Project::saveSprite( QDomElement& node, Sprite* sprite )
{
	foreach( Frame* frame, sprite->frames )
	{
		QDomElement frameNode = node.ownerDocument().createElement( "frame" );
		frameNode.setAttribute( "duration", frame->getDuration() );
		frameNode.setAttribute( "tag", frame->getTag() );
		saveFrame( frameNode, frame );
		node.appendChild( frameNode );
	}
}

void Project::saveFrame( QDomElement& node, Frame* frame )
{
	foreach( auto picture, frame->pictures )
	{
		QDomElement pictureNode = node.ownerDocument().createElement( "picture" );
		QString fileName = picture->getFileName();
		fileName = getRelativePath( fileName );
		pictureNode.setAttribute( "fileName", fileName );
		pictureNode.setAttribute( "x", picture->getPos().x() );
		pictureNode.setAttribute( "y", picture->getPos().y() );
		pictureNode.setAttribute( "visible", picture->getVisible() ? 1 : 0 );
		node.appendChild( pictureNode );
	}
}

void Project::loadSprite( QDomElement& node, Sprite* sprite )
{
	QDomElement xmlNode = node.firstChildElement( "frame" );
	while ( !xmlNode.isNull() ) 
	{
		Frame* frame = new Frame;
		frame->setDuration( xmlNode.attribute( "duration", "10" ).toInt() );
		frame->setTag( xmlNode.attribute( "tag", "" ) );
		sprite->frames.append( frame );
		loadFrame( xmlNode, frame );
		xmlNode = xmlNode.nextSiblingElement(); 
	}
}

void Project::loadFrame( QDomElement& node, Frame* sprite )
{
	QDomElement xmlNode = node.firstChildElement( "picture" );
	while ( !xmlNode.isNull() ) 
	{
		Picture* picture = new Picture;
		QString fileName = xmlNode.attribute( "fileName" );
		fileName = getAbsolutePath( fileName );
		picture->setFileName( fileName );
		if ( !picture->getFileName().isEmpty() )
		{
			int x = xmlNode.attribute( "x" ).toInt();
			int y = xmlNode.attribute( "y" ).toInt();
			bool visbile = xmlNode.attribute( "visible", "0" ).toInt();
			picture->setPos( QPoint( x, y ) );
			picture->setVisible( visbile );
			sprite->pictures.append( picture );
		}
		xmlNode = xmlNode.nextSiblingElement(); 
	}
}

//TreeNode* Project::getChild( TreeNode* parent, int row ) const
//{
//	if ( !parent )
//		parent = spriteRoot;
//
//	return parent->child( row );
//}
//
//TreeNode* Project::getParent( TreeNode* node ) const
//{
//	if ( !node )
//		return 0;
//
//	return node->getParent();
//}
//
//int Project::getChildrenCount( TreeNode* node ) const
//{
//	if ( !node )
//		node = spriteRoot;
//
//	return node->childCount();
//}

bool Project::isValidNodeName( QString nodeName ) const
{
	QRegExp rx( "([a-zA-Z_][a-zA-Z0-9_]*)" );
	return rx.exactMatch( nodeName );
}

TreeNode* Project::getRootNode() const
{
	return projectData.spriteRoot;
}

QString Project::spriteNodeRename(QString path, QString name)
{
	Q_ASSERT(isValidNodeName(name));
	TreeNode* node = projectData.spriteRoot->getNodeByPath(path);
	Q_ASSERT(node != nullptr);

	QString oldName = node->text;
	Q_ASSERT(name != oldName);
	node->text = name;

	return oldName;
}

void Project::spriteDragDrop(QString& oldPath, QString& newPath, bool copy )
{
	TreeNode* nodeFrom = getNodeByPath(oldPath);
	const TreeNode* fromParent = nodeFrom->getParent();
	Q_ASSERT(fromParent);
	int fromRow = nodeFrom->row();

	int row;
	TreeNode* nodeTo = getRootNode()->splitChild(newPath, &row);
	Q_ASSERT(nodeTo);

	if ( copy )
		nodeFrom = nodeFrom->clone();
	else
	{
		emit beginRemoveSpriteNode( nodeFrom );
		nodeFrom->unlinkFromParent();
		emit endRemoveSpriteNode();
	}

	emit beginInsertSpriteNode( nodeTo, row );
	nodeTo->insertChild( row, nodeFrom );
	emit endInsertSpriteNode();


	oldPath = fromParent->toString();
	if (oldPath != "/")
		oldPath.append("/");
	oldPath.append(QString::number(fromRow));

	newPath = nodeFrom->toString();
}

QString Project::spriteAddNode(QString path, QString name, TreeNode::NodeType nodeType)
{
	TreeNode* node = projectData.spriteRoot->getNodeByPath(path);
	Q_ASSERT(node);
	Q_ASSERT(node->isFolder());

	int row = node->childCount();
	TreeNode* newNode;
	switch (nodeType)
	{
	case TreeNode::NodeType::Folder:
		newNode = new SpriteFolder(name);
		break;
	case TreeNode::NodeType::Sprite:
	{
		Sprite* newSpriteNode = new Sprite(name);
		newSpriteNode->frames.insert(0, new Frame);
		newNode = newSpriteNode;
		break;
	}
	default:
		Q_ASSERT(false);
	}

	emit beginInsertSpriteNode(node, row);
	node->insertChild(row, newNode);
	emit endInsertSpriteNode();

	return newNode->toString();
}

void Project::spriteMoveSprite(QString path, int shiftX, int shiftY )
{
	for (auto f: getSprite(path)->frames)
		for (auto p: f->pictures)
			p->shiftPos(shiftX, shiftY);
}

TreeNode* Project::spriteUnlinkNode(QString path)
{
	TreeNode* node = projectData.spriteRoot->getNodeByPath(path);
	Q_ASSERT(node);
	Q_ASSERT(node != getRootNode());
	const TreeNode* parent = node->getParent();
	Q_ASSERT(parent);

	emit beginRemoveSpriteNode(node);
	node->unlinkFromParent();
	emit endRemoveSpriteNode();

	return node;
}

void Project::spriteLinkNode(QString path, TreeNode* node)
{
	int row;
	TreeNode* parentNode = getRootNode()->splitChild(path, &row);
	Q_ASSERT(parentNode);

 	emit beginInsertSpriteNode(parentNode, row);
	parentNode->insertChild(row, node);
 	emit endInsertSpriteNode();
}

//////////////////////////////////////////////////////////////////////////

int Project::animGetRowCount(QString spritePath)
{
	Sprite* sprite = getSpriteOrNull(spritePath);
	if (!sprite)
		return 0;
	return sprite->frames.size();
}

int Project::animGetDuration(QString spritePath, int frameIndex)
{
	return getFrame(spritePath, frameIndex )->getDuration();
}

int Project::animGetDurations(QString spritePath, const QList<int>& frames, bool& different)
{
	Sprite* s = getSprite(spritePath);
	int frameCount = s->frames.size();
	Q_ASSERT(frameCount > 0);

	bool durationFound = false;
	different = false;
	int duration = 0;

	for (int i: frames)
	{
		Q_ASSERT((i >= 0) && (i < frameCount));
		Frame* f = s->frames[i];
		Q_ASSERT(f);

		int d = f->getDuration();
		if ((durationFound) && (duration != d))
		{
			different = true;
			break;
		}

		duration = d;
		durationFound = true;
	}

	return duration;
}

QString Project::animGetTags(QString spritePath, const QList<int>& frames, bool& different)
{
	Sprite* s = getSprite(spritePath);
	int frameCount = s->frames.size();
	Q_ASSERT(frameCount > 0);

	bool tagFound = false;
	different = false;
	QString tag;

	for (int i : frames)
	{
		Q_ASSERT((i >= 0) && (i < frameCount));
		Frame* f = s->frames[i];
		Q_ASSERT(f);

		QString t = f->getTag();
		if ((tagFound) && (tag != t))
		{
			different = true;
			break;
		}

		tag = t;
		tagFound = true;
	}

	return tag;
}

int Project::animGetTotalDuration(QString spritePath)
{
	int ret = 0;
	int s = animGetRowCount(spritePath);
	for ( int i = 0; i < s; ++i )
		ret += animGetDuration(spritePath, i);
	return ret;
}

QString Project::animGetTag(QString spritePath, int frameIndex)
{
	Frame* frame = getFrame(spritePath, frameIndex);
	return frame->getTag();
}

void Project::animSetDuration(QString spritePath, const QList<int>& frames, int value)
{
	for (int i: frames)
		getFrame(spritePath, i)->setDuration(value);
}

void Project::animSetTag(QString spritePath, const QList<int>& frames, QString value)
{
	for (int i : frames)
	{
		qDebug() << "Project::animSetTag " << i << value;
		getFrame(spritePath, i)->setTag(value);
	}
}

Sprite* Project::getSprite(QString spritePath)
{
	Q_ASSERT(!spritePath.isEmpty());
	TreeNode* node = getNodeByPath(spritePath);
	Q_ASSERT(node->getType() == TreeNode::NodeType::Sprite);
	return reinterpret_cast<Sprite*>(node);
}

Sprite* Project::getSpriteOrNull(QString spritePath)
{
	TreeNode* node = getNodeByPath(spritePath);
	if (node->getType() != TreeNode::NodeType::Sprite)
		return nullptr;
	return reinterpret_cast<Sprite*>(node);
}

Frame* Project::getFrame(QString spritePath, int frameIndex)
{
	Sprite* s = getSprite(spritePath);
	Q_ASSERT((frameIndex >= 0) && (frameIndex < s->frames.size()));
	Frame* f = s->frames[frameIndex];
	Q_ASSERT(f);
	return f;
}

bool Project::checkDragEventForImage(QString spritePath, int frameIndex, const QMimeData *mimeData)
{
	if ( !compositionIsValid(spritePath, frameIndex) )
		return false;

	if ( !mimeData->hasUrls() )
		return false;

	bool goodFileExist = false;

	QList<QUrl> urls = mimeData->urls();
	foreach( QUrl url, urls )
	{
		if ( !url.isLocalFile() )
			continue;

		QString fileName = url.toLocalFile();
		QString fileExt = fileName.mid( fileName.lastIndexOf( "." ) );
		if ( ( fileExt != ".bmp" )
			&& ( fileExt != ".png" )
			&& ( fileExt != ".jpg" )
			&& ( fileExt != ".jpeg" )
			)
		{
			continue;
		}

		//QImageReader reader( fileName );

		//if ( reader.format().isNull() )
		//{
		//	event->ignore();
		//	return;
		//}

		goodFileExist = true;
	}

	return goodFileExist;
}

int Project::animInsertFrame(QString spritePath, int row, bool before, bool copyContent )
{
	Sprite* s = getSprite(spritePath);

	int rowCount = s->frames.size();
	Q_ASSERT(row < rowCount);
	//if ( row >= rowCount )
	//	return -1;


	Frame* srcFrame = 0;
	if ( copyContent && ( row >= 0 ) )
		srcFrame = s->frames.at( row );


	if ( before )
	{
		if ( row < 0 )
			row = 0;
	}
	else
	{
		if ( row < 0 )
			row = rowCount;
		
		if ( row < rowCount )
			row += 1;
	}

	emit animBeginInsertRows( row, row );

	Frame* frame = 0;
	if ( srcFrame )
		frame = new Frame( *srcFrame );
	else
		frame = new Frame;
		
	s->frames.insert( row, frame );

	emit animEndInsertRows();

	return s->frames.indexOf( frame );
}

bool Project::animDragDrop(QString spritePath, const QList<int>& indexes, int row, bool copyAction)
{
	Sprite* sprite = getSprite(spritePath);

	int rowCount = sprite->frames.size();

	QList<int> validRowList;
	foreach ( int i, indexes )
		if ( ( i >= 0 ) || ( i < rowCount ) )
			validRowList.append( i );

	if ( validRowList.isEmpty() )
		return false;

	qSort( validRowList.begin(), validRowList.end() );

	int rowShift = 0;
	if ( !copyAction )
	{
		foreach ( int i, validRowList )
			if ( row > i )
				rowShift++;
	}
	

	QList< Frame* > framesForMove;

	if ( copyAction )
	{
		foreach ( int i, validRowList )
		{
			Frame* f = new Frame( *sprite->frames.at( i ) );
			framesForMove.append( f );
		}
	}
	else
	{
		foreach ( int i, validRowList )
			framesForMove.append(sprite->frames.at( i ) );

		qSort( validRowList.begin(), validRowList.end(), qGreater<int>() );
		foreach( int i, validRowList )
		{
			emit animBeginRemoveRows( i, i );
			sprite->frames.removeAt( i );
			emit animEndRemoveRows();
		}
	}

	int r = row - rowShift;
	foreach( Frame* p, framesForMove )
	{
		emit animBeginInsertRows( r, r );
		sprite->frames.insert( r, p );
		emit animEndInsertRows();
		r++;
	}

	return true;
}

void Project::animRemoveFrames(QString spritePath, const QList<int>& frames)
{
	Sprite* sprite = getSprite(spritePath);
	Q_ASSERT(!frames.isEmpty());

	int min = *std::min_element(frames.begin(), frames.end());
	int max = *std::max_element(frames.begin(), frames.end());

	Q_ASSERT(min >= 0);
	Q_ASSERT(max < sprite->frames.size());

	QList<IntervalSorter::Item> intervals = IntervalSorter::sort(frames);
	QList<IntervalSorter::Item>::const_iterator i = intervals.constEnd();
	while (i != intervals.constBegin())
	{
		--i;
		emit animBeginRemoveRows(i->first, i->last);
		int count = i->size();
		while (count--)
			sprite->frames.removeAt(i->first);
		emit animEndRemoveRows();
	}
}

//////////////////////////////////////////////////////////////////////////

Picture* Project::compostionGetPicture(QString spritePath, int frameIndex, int row)
{
	Frame* frame = getFrame(spritePath, frameIndex);
	Q_ASSERT((row >= 0) && (row < frame->pictures.size()));
	Picture* result = frame->pictures[row];
	Q_ASSERT(result);
	return result;
}

QString Project::compositionGetFileName(QString spritePath, int frameIndex, int row)
{
	return compostionGetPicture(spritePath, frameIndex, row)->getFileName();
}

QString Project::compositionGetFileNameRelative(QString spritePath, int frameIndex, int row)
{
	QString fileName = compositionGetFileName(spritePath, frameIndex, row);
	return getRelativePath(fileName);
}

bool Project::compositionGetVisible(QString spritePath, int frameIndex, int row)
{
	return compostionGetPicture(spritePath, frameIndex, row)->getVisible();
}

QPoint Project::compositionGetPos(QString spritePath, int frameIndex, int row)
{
	return compostionGetPicture(spritePath, frameIndex, row)->getPos();
}

int Project::compositionGetRowCount(QString spritePath, int frameIndex)
{
	if (!isValidFrame(spritePath, frameIndex))
		return 0;

	Frame* frame = getFrame(spritePath, frameIndex);
	return frame->pictures.size();
}

void Project::compositionSetFileName(QString spritePath, int frameIndex, int row, QString fileName)
{
	compostionGetPicture(spritePath, frameIndex, row)->setFileName(fileName);
}

void Project::compositionSetVisible(QString spritePath, int frameIndex, int row, bool visible)
{
	compostionGetPicture(spritePath, frameIndex, row)->setVisible(visible);
}

bool Project::compositionDragDrop(QString spritePath, int frameIndex, const QList<int>& indexes, int row, bool copyAction)
{
	Frame* frame = getFrame(spritePath, frameIndex); 

	int rowCount = frame->pictures.count();
	QList<int> validRowList;
	foreach ( int i, indexes )
		if ( ( i >= 0 ) || ( i < rowCount ) )
			validRowList.append( i );

	if ( validRowList.isEmpty() )
		return false;


	qSort( validRowList.begin(), validRowList.end() );

	
	int rowShift = 0;
	if ( !copyAction )
	{
		foreach ( int i, validRowList )
			if ( row > i )
				rowShift++;
	}

	QList< Picture* > picturesForMove;


	if ( copyAction )
	{
		foreach ( int i, validRowList )
		{
			Picture* p = new Picture;
			p = frame->pictures.at( i );
			picturesForMove.append( p );
		}

	}
	else
	{
		foreach ( int i, validRowList )
			picturesForMove.append( frame->pictures.at( i ) );

		qSort( validRowList.begin(), validRowList.end(), qGreater<int>() );
		foreach( int i, validRowList )
		{
			emit compositionBeginRemoveRows( i, i );
			frame->pictures.removeAt( i );
			emit compositionEndRemoveRows();
		}
	}

	int r = row - rowShift;
	foreach( Picture* p, picturesForMove )
	{
		emit compositionBeginInsertRows( r, r );
		frame->pictures.insert( r, p );
		emit compositionEndInsertRows();
		r++;
	}

	emit sceneModelReset();

	return true;
}

void Project::compositionDelItems(QString spritePath, int frameIndex, QList< int > rows)
{
	Frame* frame = getFrame(spritePath, frameIndex);

	int count = rows.count();

	if ( !count )
		return;

	qSort( rows.begin(), rows.end() );
	
	for ( int i = count - 1; i > -1; --i )
	{
		int row = rows.at( i );
		emit compositionBeginRemoveRows( row, row );
		frame->pictures.removeAt( rows.at( i ) );
		emit compositionEndRemoveRows();
	}

	emit sceneModelReset();
}

void Project::compositionDropPictures(QString spritePath, int frameIndex, int compositionIndex, const QPoint& pos, const QStringList& fileList)
{
	Frame* frame = getFrame(spritePath, frameIndex);

	foreach( QString fileName, fileList )
	{
		QPixmap* pixmap = new QPixmap( fileName );
		bool validPixmap = !pixmap->isNull();
		delete pixmap;

		if ( !validPixmap )
			continue;

		Picture* picture = new Picture;
		picture->setFileName( fileName );
		picture->setPos( pos );

		emit compositionBeginInsertRows( compositionIndex, compositionIndex );
		frame->pictures.insert( compositionIndex, picture );
		emit compositionEndInsertRows();
	}

	emit sceneModelReset();
}

bool Project::compositionIsValid(QString spritePath, int frameIndex)
{
	Q_ASSERT(frameIndex >= 0);
	if (!isSprite(spritePath))
		return false;
	Sprite* sprite = getSprite(spritePath);
	return (sprite->frames.size() > frameIndex);
}

void Project::newProject()
{
	projectData.spriteRoot->delChildren();
	rootPath = "";
}

bool Project::isNodeDeletable(QString nodePath)
{
	return !getNodeByPath(nodePath)->isRoot();
}

bool Project::isNodeInheritable(QString nodePath)
{
	return getNodeByPath(nodePath)->isFolder();
}

TreeNode* Project::getNodeByPath(QString nodePath)
{
	Q_ASSERT(!nodePath.isEmpty());
	TreeNode* node = getRootNode()->getNodeByPath(nodePath);
	Q_ASSERT(node != nullptr);
	return node;
}

bool Project::isSprite(QString nodePath)
{
	const TreeNode* node = getRootNode()->getNodeByPath(nodePath);
	Q_ASSERT(node != nullptr);
	return node->isSprite();
}

QString Project::getRelativePath(QString path)
{
	QDir dir( rootPath );
	return QDir::cleanPath( dir.relativeFilePath( path ) );
}

QString Project::getAbsolutePath( QString path )
{
	QDir dir( rootPath );
	return QDir::cleanPath( dir.absoluteFilePath( path ) );
}

//TODO: restore functional
//void Project::updateFrameTag()
//{
//	QString tag = "";
//	bool cathed = false;
//	bool different = false;
//
//	foreach( int i, selection.animationSelect )
//	{
//		QString newTag = animGetTag( i );
//		if ( !cathed )
//		{
//			tag = newTag;
//			cathed = true;
//		}
//		else
//		{
//			if ( newTag != tag )
//				different = true;
//		}
//	}
//
//	if ( !cathed )
//		different = true;
//
//	if ( different )
//		tag = "";
//
//	frameTagChangeInProgress = true;
//	emit setLineEditFrameTag( tag );
//	frameTagChangeInProgress = false;
//}

//TODO: remove
void Project::updateFrameParams()
{

}

void Project::compositionPicturesShift(QString spritePath, int frameIndex, const QList<int>& pics, int shiftX, int shiftY)
{
	foreach(int row, pics)
	{
		QPoint point = compositionGetPos(spritePath, frameIndex, row);
		point += QPoint(shiftX, shiftY);
		compositionSetPos(spritePath, frameIndex, row, point);
	}
}

void Project::compositionSetPos(QString spritePath, int frameIndex, int row, QPoint pos)
{
	compostionGetPicture(spritePath, frameIndex, row )->setPos(pos);
}

void Project::compositionPicturesToggleVisible(QString spritePath, int frameIndex, const QList<int>& pics)
{
	foreach(int row, pics)
		compositionSetVisible(spritePath, frameIndex, row, !compositionGetVisible(spritePath, frameIndex, row));
}

template <typename T>
QList<T> reversedQList( const QList<T> & in )
{
	QList<T> result;
	result.reserve( in.size() ); // reserve is new in Qt 4.7
	std::reverse_copy( in.begin(), in.end(), std::back_inserter( result ) );
	return result;
}

bool Project::spritesCompress(QString spritePath)
{
	Sprite* sprite = getSprite(spritePath);
	
	bool spriteChanged = false;
	for (;;)
	{
		bool changes = false;

		int size = sprite->frames.size();
		for ( int i = 1; i < size; ++i )
		{
			if ( sprite->frames[ i - 1 ]->isEqual( sprite->frames[ i ] ) )
			{
				int totalDuration = sprite->frames[ i - 1 ]->getDuration() + sprite->frames[ i ]->getDuration();
				sprite->frames[ i - 1 ]->setDuration( totalDuration );

				//emit animBeginRemoveRows( i, i );
				sprite->frames.removeAt( i );
				//emit animEndRemoveRows();

				spriteChanged = true;
				changes = true;
				break;
			}
		}

		if ( !changes )
			break;
	}

	return spriteChanged;
}

Sprite* Project::animCloneNode(QString spritePath)
{
	return getSprite(spritePath)->clone();
}

Frame* Project::cloneFrame(QString spritePath, int frameIndex)
{
	return getFrame(spritePath, frameIndex)->clone();
}

void Project::replaceFrame(QString spritePath, int frameIndex, Frame* frame)
{
	Sprite* sprite = getSprite(spritePath);
	Q_ASSERT((frameIndex >= 0) && (frameIndex < sprite->frames.size()));
	delete sprite->frames[frameIndex];
	sprite->frames[frameIndex] = frame;
}

void Project::replaceSprite(QString spritePath, Sprite* sprite)
{
	TreeNode* node = getNodeByPath(spritePath);
	Q_ASSERT(node);
	Q_ASSERT(node->getType() == TreeNode::NodeType::Sprite);
	Sprite* s = reinterpret_cast<Sprite*>(node);

	s->setFrom(sprite);

	emit animModelReset();
	emit compositionModelReset();
	emit sceneModelReset();
}

bool Project::isValidFrame(QString spritePath, int frameIndex)
{
	if (frameIndex < 0)
		return false;
	if (spritePath.isEmpty())
		return false;
	TreeNode* node = getRootNode()->getNodeByPath(spritePath);
	if (node->getType() != TreeNode::NodeType::Sprite)
		return false;
	Sprite* s = reinterpret_cast<Sprite*>(node);
	if (frameIndex >= s->frames.size())
		return false;
	return true;
}

void Project::animReverse(QString spritePath, const QList<int>& indexes)
{
	Sprite* sprite = getSprite(spritePath);

	QList< Frame* > selectedFrames;
	QList< int > selectedFramesIndexes;
	foreach( int i, indexes)
	{
		Frame* f = getFrame(spritePath, i);
		if ( f )
		{
			selectedFrames << f;
			selectedFramesIndexes << i;
		}
	}

	Q_ASSERT(selectedFrames.size() > 0);

	QList< Frame* > reversedFrames = reversedQList( selectedFrames );
	Q_ASSERT( selectedFrames.size() == reversedFrames.size() );

	for ( int i = 0; i < selectedFramesIndexes.size(); ++i )
		sprite->frames[ selectedFramesIndexes[ i ] ] = reversedFrames[ i ];

	emit animModelReset();
	emit animSetSelect( selectedFramesIndexes );
	emit compositionModelReset();
	emit sceneModelReset();
}

void Project::scenePictureMove(QString spritePath, int frameIndex, int index, QPoint pos)
{
	compostionGetPicture(spritePath, frameIndex, index)->setPos(pos);
}

void Project::scenePicturesMove(QString spritePath, int frameIndex, const QList<MovePicData>& moveData, bool newPos)
{
	Q_ASSERT(!moveData.isEmpty());

	for (auto i: moveData)
		compostionGetPicture(spritePath, frameIndex, i.index)->setPos(newPos ? i.newPos : i.oldPos);
}

void Project::lineEditFrameTagTextChanged(const QString& value)
{
	//TODO: restore functional
// 	if ( frameTagChangeInProgress )
// 		return;
// 
// 	foreach( int i, selection.animationSelect )
// 	{
// 		animSetFrameTag( i, value );
// 	}
}
