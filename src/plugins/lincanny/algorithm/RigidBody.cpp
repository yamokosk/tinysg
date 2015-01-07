#include "RigidBody.h"
#include <cstring>

RigidBody::RigidBody(){
	mesh_count = 0;
	max_meshes = 5;
	meshes = new BoundingMesh*[max_meshes];
	resetTransform();	
}

RigidBody::~RigidBody(){	
	for(int i=0;i<mesh_count;i++){
		delete meshes[i];
	}
	delete [] meshes;
}

void RigidBody::resetTransform(){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(i==j){
				transform[i*4 + j] = 1.0;
			}
			else{
				transform[i*4 + j] = 0.0;
			}
		}
	}
}

void RigidBody::addBoundingMesh(BoundingMesh* mesh){
	if(mesh_count == max_meshes){
		max_meshes *= 2;
		BoundingMesh** temp = new BoundingMesh*[max_meshes];
		for(int i=0;i<mesh_count;i++){
			temp[i] = meshes[i];
		}
		delete [] meshes;
		meshes = temp;
	}
	meshes[mesh_count++] = mesh;
}

BoundingMesh** RigidBody::getMeshArray(){
	return meshes;
}

void RigidBody::setTransform(double x, double y, double z, double x_rot, double y_rot, double z_rot)
{
	double cos_x = cos(x_rot);
	double sin_x = sin(x_rot);
	double cos_y = cos(y_rot);
	double sin_y = sin(y_rot);
	double cos_z = cos(z_rot);
	double sin_z = sin(z_rot);
	double A = cos_z*cos_y + sin_z*sin_x*sin_y;
	double B = -sin_z*cos_y+cos_z*sin_x*sin_y;
	double C = cos_x*sin_y;
	double D = sin_z*cos_x;
	double E = cos_z*cos_x;
	double F = -sin_x;
	double G = -cos_z*sin_y + sin_z*sin_x*cos_y;
	double H = sin_z*sin_y + cos_z*sin_x*cos_y;
	double I = cos_x*cos_y;

	transform[0] = A;
	transform[1] = B;
	transform[2] = C;
	transform[3] = 0;

	transform[4] = D;
	transform[5] = E;
	transform[6] = F;
	transform[7] = 0;
	
	transform[8] = G;
	transform[9] = H;
	transform[10] = I;
	transform[11] = 0;

	transform[12] = x;
	transform[13] = y;
	transform[14] = z;
	transform[15] = 1;
}

void RigidBody::setTransform(const double* T)
{
	memcpy(transform, T, sizeof(double)*16);
}
