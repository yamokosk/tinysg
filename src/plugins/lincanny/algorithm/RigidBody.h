#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Primitive.h"
#include "BoundingMesh.h"

class RigidBody
{
public:
	
	int mesh_count;
	int max_meshes;
	BoundingMesh** meshes;
	double transform[16];
	
	RigidBody();
	~RigidBody();
	
	void addBoundingMesh(BoundingMesh* mesh);
	void setTransform(double x, double y, double z, double rot_x, double rot_y, double rot_z);
	void setTransform(const double* T);
	void resetTransform();
	BoundingMesh** getMeshArray();

};

#endif
