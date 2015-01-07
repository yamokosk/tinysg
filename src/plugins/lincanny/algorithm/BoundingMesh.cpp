#include "BoundingMesh.h"

BoundingMesh::BoundingMesh()
{
}

BoundingMesh::BoundingMesh(const char* f) :
	filename(f)
{
}

BoundingMesh::~BoundingMesh()
{
}

bool BoundingMesh::load(const char* f)
{
	filename = f;
	mesh.reset( new POLYHEDRON() );
	if ( importOBJ(filename.c_str(), this->mesh.get()) > 0 )
		return false;
	else
		return true;
}

bool BoundingMesh::load()
{
	mesh.reset( new POLYHEDRON() );
	if ( importOBJ(filename.c_str(), this->mesh.get()) > 0 )
		return false;
	else
		return true;
}

POLYHEDRON* BoundingMesh::getPoly()
{
	return mesh.get();
}
