#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <vector>

#define VERTEX_FEATURE	0x01
#define EDGE_FEATURE	0x02
#define FACE_FEATURE	0x04
//const int VERTEX_FEATURE = 200;
//const int EDGE_FEATURE = 201;
//const int FACE_FEATURE = 202;

const int LEFT_FACE = 300;
const int RIGHT_FACE = 301;

const int SEGMENT_INSIDE = 400;
const int SEGMENT_HEAD = 401;
const int SEGMENT_TAIL = 402;


class FEATURE
{
public:
	int id;
	FEATURE();
	FEATURE(int id);
};


//foward declarations
class EDGE;
class FACE;
class POLYHEDRON;
class RigidBody;

class VERTEX3D : public FEATURE
{
public:
	int index;
	double x,y,z;
	int adjacency_size;
	int adjacency_max;
	EDGE** adjacency;
	
	VERTEX3D();
	VERTEX3D(double x, double y, double z);	
	~VERTEX3D();
	int addToAdjacency(EDGE* v);
	void transform(const double* model_matrix, double& tx, double& ty, double& tz) const;
	void transform(const double* t, VERTEX3D& v) const;
	void rotate(const double* model_matrix, double& tx, double& ty, double& tz) const;
	void rotate(const double* model_matrix, VERTEX3D& v) const;
	void translate(const double* model_matrix, double& tx, double& ty, double& tz) const;
	double magSquared();
	double mag();
	double dot(const VERTEX3D& v) const;
	void normalize();
	VERTEX3D cross(const VERTEX3D& v) const;

	VERTEX3D operator-(const VERTEX3D& v) const
	{
		return VERTEX3D(x - v.x, y - v.y, z- v.z );
	}
	VERTEX3D operator+(const VERTEX3D& v) const
	{
		return VERTEX3D(x + v.x, y + v.y, z + v.z );
	}
	VERTEX3D operator+(double s) const
	{
		return VERTEX3D(x + s, y + s, z + s);
	}
	VERTEX3D operator+=(const VERTEX3D& v)
	{
		x+=v.x;
		y+=v.y;
		z+=v.z;
		return *this;
	}
	VERTEX3D operator*(double s) const
	{
		return VERTEX3D(x*s, y*s, z*s);
	}
	VERTEX3D operator/(double s) const
	{
		return VERTEX3D(x/s, y/s, z/s);
	}
	VERTEX3D operator=(const VERTEX3D& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
};

class EDGE : public FEATURE
{
public:
	int index;
	VERTEX3D* head;
	VERTEX3D* tail;
	FACE* right_face;
	FACE* left_face;
	
	EDGE(VERTEX3D* v1, VERTEX3D* v2);
	~EDGE();
	void setCoboundary(FACE* lf, FACE* rf);
};

class FACE : public FEATURE
{
public:
	int index;
	int vertex_count;
	
	mutable int idc_edge;
	VERTEX3D n;
	VERTEX3D d;
	EDGE** edges;
	VERTEX3D** vertices;
	POLYHEDRON* mesh;
	
	FACE(VERTEX3D* v1,VERTEX3D* v2,VERTEX3D* v3,EDGE* e1,EDGE* e2,EDGE* e3);
	FACE(VERTEX3D* v1,VERTEX3D* v2,VERTEX3D* v3,VERTEX3D* v4,EDGE* e1,EDGE* e2,EDGE* e3, EDGE* e4);
	~FACE();
	int getVertexIndex(const VERTEX3D* v) const;
	int getEdgeSide(const EDGE* e) const;
};

class POLYHEDRON
{
public:
	int vertex_count, edge_count, face_count;
	VERTEX3D** vertices;
	EDGE** edges;
	FACE** faces;
	POLYHEDRON();
	~POLYHEDRON();
	int addEdge(EDGE* e);
};

typedef VERTEX3D VECTOR3D;

#endif
