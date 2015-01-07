#ifndef BOUNDINGMESH_H
#define BOUNDINGMESH_H

#include "Primitive.h"
#include "OBJImport.h"

#include <memory>
#include <string>

// convex bounding geometery
// supports only quad/tri faces
// for now

class BoundingMesh
{
public:
	typedef std::auto_ptr<POLYHEDRON> PolyhedronPtr;
	PolyhedronPtr mesh;
	std::string filename;
	BoundingMesh();
	BoundingMesh(const char* filename);
	~BoundingMesh();
	bool load();
	bool load(const char* filename);
	POLYHEDRON* getPoly();
};

#endif
