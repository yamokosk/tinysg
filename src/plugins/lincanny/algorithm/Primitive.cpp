#include "Primitive.h"

/***************
     FEATURE 
****************/
FEATURE::FEATURE()
{
	this->id = -1;
}

FEATURE::FEATURE(int id)
{
	this->id = id;
}


/***************
    VERTEX3D 
****************/
VERTEX3D::VERTEX3D() :
	FEATURE(VERTEX_FEATURE)
{
	x = y = z = 0.0;
	adjacency_size = 0;  
	adjacency_max = 4;
}

VERTEX3D::VERTEX3D(double x, double y, double z) :
	FEATURE(VERTEX_FEATURE)
{
	this->x = x;
	this->y = y;
	this->z = z;
	adjacency_size = 0;  
	adjacency_max = 4;
}

VERTEX3D::~VERTEX3D() {}

int VERTEX3D::addToAdjacency(EDGE* e)
{
	if ( !adjacency_size )
	{
		adjacency = new EDGE*[adjacency_max];
	}
	else
	{
		for(int i=0; i < adjacency_size; i++)
		{
			if( adjacency[i]->index == e->index )
			{
				return 1;
			}
		}
		if ( adjacency_size == adjacency_max )
		{
			adjacency_max *= 2;
			EDGE** temp = new EDGE*[adjacency_max];
			for (int i=0; i< adjacency_size; i++)
			{
				temp[i] = adjacency[i];
			}
			delete [] adjacency;
			adjacency = temp;
		}	
	}
	adjacency[adjacency_size++] = e;
	return 0;	
}

double VERTEX3D::dot(const VERTEX3D& v) const
{
	return x*v.x + y*v.y + z*v.z;
}

VERTEX3D VERTEX3D::cross(const VERTEX3D& v) const
{
	return VERTEX3D(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);
}

double VERTEX3D::magSquared()
{
	return (x*x + y*y + z*z);
}

double VERTEX3D::mag()
{
	return sqrt(x*x + y*y + z*z);
}

void VERTEX3D::normalize()
{
	double m = this->mag();
	if (m == 0.000){
		x = y = z = 0.000;
	}
	else{
		x /= m;
		y /= m;
		z /= m;
	}
}

void VERTEX3D::transform(const double* t, double &tx, double &ty, double &tz) const
{
	tx = x*t[0] + y*t[4] + z*t[8] + t[12];
	ty = x*t[1] + y*t[5] + z*t[9] + t[13];
	tz = x*t[2] + y*t[6] + z*t[10] + t[14];
}

void VERTEX3D::transform(const double* t, VERTEX3D& v) const
{
	if ( t != NULL )
	{
		v.x = x*t[0] + y*t[4] + z*t[8] + t[12];
		v.y = x*t[1] + y*t[5] + z*t[9] + t[13];
		v.z = x*t[2] + y*t[6] + z*t[10] + t[14];
	}
}


void VERTEX3D::rotate(const double* t, VERTEX3D& v) const
{
	v.x = x*t[0] + y*t[4] + z*t[8];
	v.y = x*t[1] + y*t[5] + z*t[9];
	v.z = x*t[2] + y*t[6] + z*t[10];
}

void VERTEX3D::translate(const double* t, double &tx, double &ty, double &tz) const
{
	tx += t[12];
	ty += t[13];
	tz += t[14];
}




/***************
      EDGE 
****************/
EDGE::EDGE(VERTEX3D* v1, VERTEX3D* v2):FEATURE(EDGE_FEATURE){
	this->head = v1;
	this->tail = v2;	
	left_face = 0;
	right_face =0;
}

EDGE::~EDGE(){}
	
void EDGE::setCoboundary(FACE* lf, FACE* rf){
	this->left_face = lf;
	this->right_face= rf;
}


/***************
      FACE
****************/
FACE::FACE(VERTEX3D* v1,VERTEX3D* v2,VERTEX3D* v3,VERTEX3D* v4,EDGE* e1,EDGE* e2,EDGE* e3,EDGE* e4):FEATURE(FACE_FEATURE){
	this->vertex_count = 4;
	this->vertices = new VERTEX3D*[this->vertex_count];
	this->edges = new EDGE*[this->vertex_count];
	this->idc_edge = 0;
	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;
	vertices[3] = v4;
	edges[0] = e1;
	edges[1] = e2;
	edges[2] = e3;
	edges[3] = e4;

	n = (*v2-*v1).cross(*v4-*v1);
	n.normalize();
	d.x = (v1->x + v2->x + v3->x + v4->x)/4.0;
	d.y = (v1->y + v2->y + v3->y + v4->y)/4.0;
	d.z = (v1->z + v2->z + v3->z + v4->z)/4.0;

	for (int j=0; j<vertex_count; j++)
	{
		//int h = getVertexIndex(edges[j]->head)+1;
		//int t = getVertexIndex(edges[j]->tail)+1;
		
		if(edges[j]->left_face)
		{
			edges[j]->right_face = this;
		}
		else
		{
			edges[j]->left_face = this;
		}
	}
}

FACE::FACE(VERTEX3D* v1,VERTEX3D* v2,VERTEX3D* v3,EDGE* e1,EDGE* e2,EDGE* e3):FEATURE(FACE_FEATURE){
	this->vertex_count = 3;
	this->vertices = new VERTEX3D*[this->vertex_count];
	this->edges = new EDGE*[this->vertex_count];
	this->idc_edge = 0;
	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;
	edges[0] = e1;
	edges[1] = e2;
	edges[2] = e3;

	n = (*v2-*v1).cross(*v3-*v1);
	n.normalize();
	d.x = (v1->x + v2->x + v3->x)/3.0;
	d.y = (v1->y + v2->y + v3->y)/3.0;
	d.z = (v1->z + v2->z + v3->z)/3.0;

	for(int j=0; j < vertex_count; j++)
	{
		//int h = getVertexIndex(edges[j]->head)+1;
		//int t = getVertexIndex(edges[j]->tail)+1;
		
		if(edges[j]->left_face)
		{
			edges[j]->right_face = this;
		}
		else
		{
			edges[j]->left_face = this;
		}
	}
}

FACE::~FACE(){}

int FACE::getVertexIndex(const VERTEX3D* v) const
{
	for(int i=0;i<this->vertex_count;i++)
	{
		if(v->index == this->vertices[i]->index)
		{
			return i;
		}
	}
	return -1;
}

int FACE::getEdgeSide(const EDGE* e) const
{
	if(e->left_face->index == this->index)
	{
		return LEFT_FACE;
	}
	return RIGHT_FACE;

}

/***************
    POLYHEDRON
****************/
POLYHEDRON::POLYHEDRON(){
	this->vertex_count = 0;
	this->edge_count = 0;
	this->face_count = 0;
}

POLYHEDRON::~POLYHEDRON(){
	for(int i=0;i<edge_count;i++){
		delete edges[i];
	}
	for(int i=0;i<face_count;i++){
		delete faces[i];
	}

	delete [] vertices;
	delete [] edges;
	delete [] faces;
}

