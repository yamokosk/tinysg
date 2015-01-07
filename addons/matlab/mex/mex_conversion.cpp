/*
 * mex_conversion.cpp
 *
 *  Created on: Oct 27, 2009
 *      Author: yamokosk
 */

#include "mex_conversion.h"

/* ----------------------------------------------------------------------------
 * StringMx
 *
 */
std::string StringMx::convert(const mxArray* str)
{
	char* cstr = mxArrayToString( str );
	std::string sstr( cstr );
	mxFree(cstr);
	return sstr;
}

/* ----------------------------------------------------------------------------
 * MxString
 *
 */
mxArray* MxString::convert(const std::string& str)
{
	return mxCreateString( str.c_str() );
}

/* ----------------------------------------------------------------------------
 * MxAny
 *
 */
mxArray* MxAny::convert(const boost::any& obj)
{
	if ( is_int(obj) )
	{
		double val = (double)( boost::any_cast<int>(obj) );
		return mxCreateDoubleScalar( val );
	}

	if ( is_real(obj) )
	{
		double val = (double)( boost::any_cast<tinysg::Real>(obj) );
		return mxCreateDoubleScalar( val );
	}

	if ( is_unsigned_long(obj) )
	{
		double val = (double)( boost::any_cast<unsigned long>(obj) );
		return mxCreateDoubleScalar( val );
	}

	if ( is_char_ptr(obj) )
	{
		char* msg = boost::any_cast<char*>(obj);
		return MxString::convert(msg);
	}

	if ( contains<std::string>(obj) )
	{
		std::string str = *( boost::unsafe_any_cast<std::string>(&obj) );
		return MxString::convert( str );
	}

	//if ( is_vector_3(obj) )
	if ( contains<Vector3>(obj) )
	{
		//Vector3 v( boost::any_cast<Vector3>(obj) );
		Vector3 v = *( boost::unsafe_any_cast<Vector3>(&obj) );
		return MxVector3::convert(v);
	}

	if ( is_quaternion(obj) )
	{
		Quaternion q( boost::any_cast<Quaternion>(obj) );
		return MxQuaternion::convert(q);
	}

	if ( is_scene_object_ptr(obj) )
	{
		SceneObject* sceneObj = boost::any_cast<SceneObject*>(obj);
		ObjectInfo info;
		sceneObj->getInfo(info);
		return MxObjectInfo::convert(info);
	}

	if ( is_obj_empty(obj) )
	{
		return MxString::convert( Archive::EmptyValue );
	}

	return MxString::convert(Archive::UnknownValue);
};

/* ----------------------------------------------------------------------------
 * MxPropertyContainer
 *
 */
mxArray* MxPropertyContainer::convert(const PropertyContainer& props)
{
	std::vector< const char* > fieldNames;
	int numFilenames = 0;

	BOOST_FOREACH(Property prop, props)
	{
		// Check for filename property - need to handle as a special case
		if ( ( prop.name_str() == "filename" ) )
		{
			if ( numFilenames == 0 )
			{
				fieldNames.push_back( "filenames" );
			}

			numFilenames++;
		}
		else
		{
			fieldNames.push_back( prop.name_str().c_str() );
		}
	}

	mxArray* properties = mxCreateStructMatrix(1, 1, fieldNames.size(), &fieldNames[0]);

	// Handle filenames
	if ( numFilenames > 0 )
	{
		// Create a cell array to store filenames
		/*mwSize size[2];
		size[0] = numFilenames; size[1] = 1;
		mxArray* filenames = mxCreateCellArray(2, size);
		int i = 0;

		BOOST_FOREACH(Property prop, props)
		{
			if ( prop.name_str() == "filename" )
			{
				mwSize subs[2]; subs[0] = i++; subs[1] = 0;
				mwIndex index = mxCalcSingleSubscript(filenames, 2, subs);
				mxSetCell(filenames, index, MxAny::convert( prop.value() ));
			}
		}*/

		// Create a struct to store filenames
		const char* filenameFieldNames[] = {"name", "color", "alpha"};
		mxArray* filenames = mxCreateStructMatrix(numFilenames, 1, 3, filenameFieldNames);
		int i = 0;

		BOOST_FOREACH(Property prop, props)
		{
			if ( prop.name_str() == "filename" )
			{
				mxSetField(filenames, i, "name", MxAny::convert(prop.value()) );

				Property color_property = prop.get_parameter("color");
				mxSetField(filenames, i, "color", MxAny::convert(color_property.value()) );
				//mxSetField(filenames, i, "color", mxCreateDoubleScalar(1.0) );

				Property alpha_property = prop.get_parameter("alpha");
				mxSetField(filenames, i, "alpha", MxAny::convert(alpha_property.value()) );
				//mxSetField(filenames, i, "alpha", mxCreateDoubleScalar(1.0) );

				i++;
			}
		}

		mxSetField(properties, 0, "filenames", filenames);
	}

	int j=0;
	if (numFilenames > 0) j = 1;
	BOOST_FOREACH(Property prop, props)
	{
		if ( prop.name_str() != "filename" )
			mxSetField(properties, 0, fieldNames[j++], MxAny::convert( prop.value() ));
	}

	return properties;
};

/* ----------------------------------------------------------------------------
 * MxObjectInfo
 *
 */
const int MxObjectInfo::NumFields = 4;
const char* MxObjectInfo::FieldNames[] = {
	"name", "type", "parameters", "meshes"
};

mxArray* MxObjectInfo::convert(const ObjectInfo& info)
{
	mxArray* object = mxCreateStructMatrix(1, 1, NumFields, FieldNames);

	mxSetField(object, 0, FieldNames[0], MxString::convert( info.name ));
	mxSetField(object, 0, FieldNames[1], MxString::convert( info.type ));
	mxSetField(object, 0, FieldNames[2], MxPropertyContainer::convert( info.parameters ));

	return object;
};

/* ----------------------------------------------------------------------------
 * MxSceneObject
 *
 */
const int MxSceneObject::NumFields = 3;
const char* MxSceneObject::FieldNames[] = {
	"name", "type", "parameters"
};

mxArray* MxSceneObject::convert(const SceneObject *obj)
{
	ObjectInfo info;
	obj->getInfo( info );

	mxArray* mxObject = mxCreateStructMatrix(1,1,NumFields,FieldNames);

	// Name
	mxSetField( mxObject, 0, FieldNames[0], MxString::convert(info.name) );

	// Type
	mxSetField( mxObject, 0, FieldNames[1], MxString::convert(info.type) );

	// Properties
	mxSetField( mxObject, 0, FieldNames[2], MxPropertyContainer::convert(info.parameters) );

	// Meshes


	/*array[Object_Properties] = mxCreateStructMatrix(info.parameters.size(), 1, NUM_PROPERTY_FIELDS, fnames_property);
	int counter = 0;
	BOOST_FOREACH(Property prop, info.parameters)
	{
		mxSetField(	array[Object_Properties], counter, fnames_property[Property_Name], mxCreateString( prop.getName().c_str() ) );
		StringTuple tuple = stringify( prop.const_value() );
		mxSetField(	array[Object_Properties], counter, fnames_property[Property_Class], mxCreateString( tuple.first.c_str() ) );
		mxSetField(	array[Object_Properties], counter, fnames_property[Property_Value], mxCreateString( tuple.second.c_str() ) );
		counter++;
	}

	// World Pose
	array[Object_WorldPose] = matrix4_to_mxArray( node->getTransform(SceneNode::TS_WORLD) );

	// Graphics
	array[Object_Graphics] = mxCreateStructMatrix(1, 1, NUM_GRAPHIC_FIELDS, fnames_graphic);
	mxSetField(array[Object_Graphics], 0, fnames_graphic[Graphic_Handle], mxCreateDoubleScalar(0.0) );
	mxSetField(array[Object_Graphics], 0, fnames_graphic[Graphic_Mesh], mxCreateDoubleScalar(0.0) );
	mxSetField(array[Object_Graphics], 0, fnames_graphic[Graphic_Color], mxCreateDoubleScalar(0.0) );
	mxSetField(array[Object_Graphics], 0, fnames_graphic[Graphic_Alpha], mxCreateDoubleScalar(0.0) );*/

	return mxObject;
};

/* ----------------------------------------------------------------------------
 * MxMeshes
 *
 */
const int MxMeshes::NumFields = 2;
const char* MxMeshes::FieldNames[] = {
	"faces", "vertices"
};

mxArray* MxMeshes::convert(const std::vector<TriSurfaceMesh>& meshes)
{
	//mexPrintf("number of meshes: %d\n", meshes.size());
	int numMeshes = (int)meshes.size();

	mxArray* mxMeshes = mxCreateStructMatrix(numMeshes, 1, NumFields, FieldNames);

	for (int m=0; m < numMeshes; ++m)
	{
		// Copy face data
		int numFaces = meshes[m].faces.size();
		mxArray* mxFaces = mxCreateDoubleMatrix(numFaces, 3, mxREAL); // Face index storage
		double *pFace = mxGetPr(mxFaces);

		for (int n=0; n < numFaces; ++n)
		{
			// Remember, need to add one to the face index because matlab is
			// 1-based index.
			ACCESS_MX_ARRAY(pFace,n,0,numFaces) = (double)meshes[m].faces[n].v1 + 1.0;
			ACCESS_MX_ARRAY(pFace,n,1,numFaces) = (double)meshes[m].faces[n].v2 + 1.0;
			ACCESS_MX_ARRAY(pFace,n,2,numFaces) = (double)meshes[m].faces[n].v3 + 1.0;
		}

		mxSetField(mxMeshes, m, FieldNames[0], mxFaces);

		// Copy vertex data
		int numVerts = meshes[m].vertices.size();
		mxArray* mxVerts = mxCreateDoubleMatrix(numVerts, 3, mxREAL); // Vertex storage
		double *pVertex = mxGetPr(mxVerts);

		for (int n=0; n < numVerts; ++n)
		{
			ACCESS_MX_ARRAY(pVertex,n,0,numVerts) = (double)meshes[m].vertices[n].x;
			ACCESS_MX_ARRAY(pVertex,n,1,numVerts) = (double)meshes[m].vertices[n].y;
			ACCESS_MX_ARRAY(pVertex,n,2,numVerts) = (double)meshes[m].vertices[n].z;
		}

		mxSetField(mxMeshes, m, FieldNames[1], mxVerts);
	}

	return mxMeshes;
}

/* ----------------------------------------------------------------------------
 * MxObjectIterator
 *
 */
mxArray* MxObjectIterator::convert(SceneGraph::SceneObjectIterator& iter, int size)
{
	mxArray* mxObjects = mxCreateStructMatrix(size, 1,
			MxObjectInfo::NumFields, MxObjectInfo::FieldNames);

	int counter = 0;

	while ( iter.hasMoreElements() )
	{
		SceneObject* object = iter.getNext();

		ObjectInfo info;
		object->getInfo(info);

		mxSetField(mxObjects, counter, MxObjectInfo::FieldNames[0], MxString::convert( info.name ));
		mxSetField(mxObjects, counter, MxObjectInfo::FieldNames[1], MxString::convert( info.type ));
		mxSetField(mxObjects, counter, MxObjectInfo::FieldNames[2], MxPropertyContainer::convert( info.parameters ));
		mxSetField(mxObjects, counter, MxObjectInfo::FieldNames[3], MxMeshes::convert( info.meshes ) );

		counter++;
	}

	return mxObjects;
};

/* ----------------------------------------------------------------------------
 * MxSceneNode
 *
 */
const int MxSceneNode::NumFields = 4;
const char* MxSceneNode::FieldNames[] =
{
	"name", "parent", "T_world_to_node", "objects"
};

MxSceneNode::MxSceneNode(SceneNode* n) :
	node(n)
{

};

MxSceneNodePtr MxSceneNode::getParent() const
{
	return MxSceneNodePtr( new MxSceneNode( node->getParent() ) );
};

bool MxSceneNode::hasParent() const
{
	return node->hasParent();
};

mxArray* MxSceneNode::getName() const
{
	return MxString::convert( node->getName() );
};

void MxSceneNode::setPosition( const mxArray* p )
{
	node->setPosition( Vector3Mx::convert(p) );
};

void MxSceneNode::setOrientation( const mxArray* q )
{
	node->setOrientation( QuaternionMx::convert(q) );
};

mxArray* MxSceneNode::getPosition(TransformSpace space) const
{
	return MxVector3::convert( node->getPosition(space) );
};

mxArray* MxSceneNode::getOrientation(TransformSpace space) const
{
	return MxQuaternion::convert( node->getOrientation(space) );
};

mxArray* MxSceneNode::convert() const
{
	mxArray* mxNode = mxCreateStructMatrix(1, 1, NumFields, FieldNames);

	// Name
	mxSetField(mxNode, 0, FieldNames[0], getName());

	// Parent
	if ( hasParent() )
		mxSetField(mxNode, 0, FieldNames[1], getParent()->getName());
	else
		mxSetField(mxNode, 0, FieldNames[1], MxString::convert("NO_PARENT") );

	// WorldPose
	mxSetField(mxNode, 0, FieldNames[2], MxNodeUtils::get_transform_matrix(node, tinysg::TS_WORLD) );

	// Attached objects
	//mxSetField(mxNode, 0, FieldNames[3], getAttachedObjects() );
	mxSetField(mxNode, 0, FieldNames[3], mxCreateDoubleScalar(0.0) );

	return mxNode;
};

/* ----------------------------------------------------------------------------
 * MxQueryArguments
 *
 */
mxArray* MxQueryArguments::convert(const QueryArguments& args)
{
	const char* fieldNames[] = {"critpoints", "regpoints", "distances"};
	mxArray* mxQueryResults = mxCreateStructMatrix(1,1,3,fieldNames);

	int numPoints = args.critpnt.size();
	mxArray* mxCritPoints = mxCreateDoubleMatrix(3, numPoints, mxREAL);
	mxArray* mxRegPoints = mxCreateDoubleMatrix(3, numPoints, mxREAL);
	mxArray* mxDistances = mxCreateDoubleMatrix(1, numPoints, mxREAL);

	double* pCritPoints = mxGetPr(mxCritPoints);
	double* pRegPoints = mxGetPr(mxRegPoints);
	double* pDistances = mxGetPr(mxDistances);

	for (int n=0; n < numPoints; ++n)
	{
		Point3D critpnt = args.critpnt[n];
		ACCESS_MX_ARRAY(pCritPoints,0,n,3) = (double)critpnt.x;
		ACCESS_MX_ARRAY(pCritPoints,1,n,3) = (double)critpnt.y;
		ACCESS_MX_ARRAY(pCritPoints,2,n,3) = (double)critpnt.z;

		Point3D regpnt = args.regpnt[n];
		ACCESS_MX_ARRAY(pRegPoints,0,n,3) = (double)regpnt.x;
		ACCESS_MX_ARRAY(pRegPoints,1,n,3) = (double)regpnt.y;
		ACCESS_MX_ARRAY(pRegPoints,2,n,3) = (double)regpnt.z;

		ACCESS_MX_ARRAY(pDistances, 0, n, 1) = (double)args.distanceMap[n];
	}

	mxSetField(mxQueryResults, 0, "critpoints", mxCritPoints);
	mxSetField(mxQueryResults, 0, "regpoints", mxRegPoints);
	mxSetField(mxQueryResults, 0, "distances", mxDistances);

	return mxQueryResults;
};

/* ----------------------------------------------------------------------------
 * MxSceneGraph
 *
 */
MxSceneGraph::MxSceneGraph(SceneGraph& g) :
	graph(g)
{

};

mxArray* MxSceneGraph::getNode(const mxArray* name) const
{
	SceneNode* node = graph.getNode( StringMx::convert(name) );
	if ( node == NULL )
	{
		std::stringstream errmsg;
		errmsg << "Node " << StringMx::convert(name) << " does not seem to exist in the scene." << std::endl;
		throw errmsg.str();
	}
	MxSceneNode adapter(node);
	return adapter.convert();
}

mxArray* MxSceneGraph::getObject(const mxArray* name) const
{
	SceneObject* object = graph.getObject( StringMx::convert(name) );
	return MxSceneObject::convert(object);
}

mxArray* MxSceneGraph::getAllObjects(void) const
{
	unsigned int numObjects = graph.getNumObjects();
	SceneGraph::SceneObjectIterator iter = graph.getAllObjects();

	return MxObjectIterator::convert( iter, (int)numObjects );
}
