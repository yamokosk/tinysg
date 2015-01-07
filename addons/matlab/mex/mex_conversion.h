/*
 * mex_conversion.h
 *
 *  Created on: Oct 26, 2009
 *      Author: yamokosk
 */

#ifndef MEX_CONVERSION_H_
#define MEX_CONVERSION_H_

#include "mex_common.h"

#include <NodeUtils.h>
#include <Stringify.h>
#include <linalg/matlab/MxConversion.h>

using namespace tinysg;

//typedef std::auto_ptr<SceneNode*> SceneNodeSPtr;

/*
 * NOTE: AdapterTSG class never owns the pointers passed to it.
 *
 * 		AdapterTsg: "Don't blame me for memory leaks."
 */

struct StringMx;
struct MxString;
struct MxNodeUtils;
struct MxAny;
struct MxPropertyContainer;
struct MxObjectInfo;
struct MxSceneObject;
struct MxObjectIterator;
struct MxSceneNode;
struct MxQueryArguments;
struct MxSceneGraph;

struct StringMx
{
	static std::string convert(const mxArray* str);
};

struct MxString
{
	static mxArray* convert(const std::string& str);
};

struct MxNodeUtils
{
	static mxArray* get_transform_matrix(const tinysg::SceneNode* n, tinysg::TransformSpace tspace = tinysg::TS_WORLD)
	{
		Matrix4 T = tinysg::get_transform_matrix(n, tspace);
		return MxMatrix4::convert( T );
	};
};

struct MxAny
{
	static mxArray* convert(const boost::any& obj);
};

struct MxPropertyContainer
{
	static mxArray* convert(const PropertyContainer& props);
};

struct MxObjectInfo
{
	static const int NumFields;
	static const char* FieldNames[];

	static mxArray* convert(const ObjectInfo& info);
};

struct MxSceneObject
{
	static const int NumFields;
	static const char* FieldNames[];

	static mxArray* convert(const SceneObject *obj);
};

struct MxMeshes
{
	static const int NumFields;
	static const char* FieldNames[];

	static mxArray* convert(const std::vector<TriSurfaceMesh>& meshes);
};

struct MxObjectIterator
{
	static mxArray* convert(SceneGraph::SceneObjectIterator& iter, int size);
};

typedef std::auto_ptr<MxSceneNode> MxSceneNodePtr;

struct MxSceneNode
{
	static const int NumFields;
	static const char* FieldNames[];

	MxSceneNode(SceneNode* n);

	/*
	 * Would make some interesting functions to adapt, but don't have the time
	 * right now.
	// Child management
	SceneNode* createChild();
	SceneNode* createChild(const std::string& name);
	void addChild(SceneNode* n);
	SceneNode* removeChild(SceneNode* child);
	SceneNode* removeChild(const std::string& name);
	void removeAllChildren();
	SceneNode* getChild(const std::string& name) const;
	ChildIterator getChildren();

	// Accessors
	void setParent(SceneNode* n);
	SceneNode* getParent() const {return parent;}
	bool hasParent() const {return (parent != NULL);}

	// Object functions
	void attach(SceneObject* obj)
	SceneObjectIterator getAttachedObjects();
	unsigned int getNumAttachedObjects() const;

	// WOW! Now this could have ultimate potential
	// Generator functions
	void registerPoseGenerator(PoseGenerator*);
	const PoseGenerator* getPoseGenerator() {return mpPoseGenerator;}
	 */

	/*inline mxArray* getAttachedObjects() const
	{
		SceneNode::SceneObjectIterator iter = node->getAttachedObjects();
		return MxObjectIterator::convert(iter, (int)node->getNumAttachedObjects());
	};*/

	/*
	 * Absolutely MUST be adapted.
	*/
	inline MxSceneNodePtr getParent() const;
	inline bool hasParent() const;
	inline mxArray* getName() const;
	inline void setPosition( const mxArray* p );
	inline void setOrientation( const mxArray* q );
	inline mxArray* getPosition(TransformSpace space=TS_PARENT) const;
	inline mxArray* getOrientation(TransformSpace space=TS_PARENT) const;
	mxArray* convert() const;

	SceneNode* node;
};

struct MxQueryArguments
{
	static mxArray* convert(const QueryArguments& args);
};

struct MxSceneGraph
{
	MxSceneGraph(SceneGraph& g);

	/*
	 * Would make some interesting functions to adapt, but don't have the time
	 * right now.
	// Node management
	SceneNode* createNode(const std::string& name);
	void destroyAllNodes();

	// Scene management
	void clearScene();
	void update();
	void __deprecated_update();

	// Object management
	SceneObject* createObject(const ObjectInfo& info);
	SceneObject* createObject(const std::string& name, const std::string& type);
	SceneObject* createObject(const std::string& name, const std::string& type, const PropertyContainer& properties);

	// Query management
	Query* createQuery(const std::string& type);
	*/

	mxArray* getNode(const mxArray* name) const;
	mxArray* getObject(const mxArray* name) const;
	mxArray* getAllObjects(void) const;

	SceneGraph& graph;
};


#endif /* MEX_CONVERSION_H_ */
