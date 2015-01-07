#ifndef IDC_H
#define IDC_H

/* 
IDC(Incremental Distance Calculation): Based on Lin-Canny 
closest features. See M Lin J Canny, "A fast algorithm for 
incremental distance calculation," 1991. 
*/

#include "RigidBody.h"

#define EPS 0.000001

double closestFeaturesInit(POLYHEDRON* poly1, POLYHEDRON* poly2, double* poly1_trans, double* poly2_trans, 
						   FEATURE* &f1, FEATURE* &f2, VERTEX3D &pos1, VERTEX3D &pos2, int debug);

/*
Call this first time two features are checked

closestFeaturesInit:  Find the closest features between polyhedron 
rigid bodies. returns: distance between closest features of poly1 
and poly2.

parameters:
b1, b2: rigid bodies whose distance between closest features 
is to be computed. 
f1, f2: address of closest features for b1 and b2, respectively.
pos1, pos2: the 3D position of the two closest features on b1 
and b2, respectively. 
*/

double closestFeatures(POLYHEDRON* poly1, POLYHEDRON* poly2, double* poly1_trans, double* poly2_trans, 
				       FEATURE* &f1, FEATURE* &f2, VERTEX3D &pos1, VERTEX3D &pos2, int debug, int depth);






bool point_vertex(VERTEX3D* p, VERTEX3D* v, double* p_trans, double* e_trans, FEATURE* &f);
bool point_edge(VERTEX3D* p, EDGE* e, double* p_trans, double* e_trans, FEATURE* &f);
bool point_face(VERTEX3D* p, FACE* face, double* p_trans, double* face_trans, FEATURE* &f);
bool edge_edge(EDGE* e1, EDGE* e2, double* e1_trans, double* e2_trans, FEATURE* &f);
bool edge_face(EDGE* edge, FACE* face, double* edge_trans, double* face_trans, FEATURE* &f);
bool face_edge(FACE* face, EDGE* edge, double* face_trans, double* edge_trans, FEATURE* &f);
bool face_face(FACE* face1, FACE* face2, double* face1_trans, double* face2_trans, FEATURE* &f);

bool confirmClosest(FEATURE* f1, FEATURE* f2, double* f1_trans, double* f2_trans);

bool pointInFrontOfPlane(VECTOR3D pos, VECTOR3D n, VERTEX3D p);
bool parallelEdgeFace(EDGE* e, FACE* f , double* e_trans, double* f_trans);
bool parallelFaceFace(FACE* face1, FACE* face2, double* face1_trans, double* face2_trans);
bool overlapTest(FACE* f1, FACE* f2, double* f1_trans, double* f2_trans, FEATURE* &f);
bool edgeVisible(EDGE* e, VERTEX3D* p, double* e_trans, double* p_trans,int &face_id);

double closestPointEdge(VERTEX3D* p, EDGE* f, double* p_trans, double* e_trans, VERTEX3D &v);
double closestPointFace(VERTEX3D* p, FACE* f, double* p_trans, double* f_trans, VERTEX3D &v);
double closestEdgeEdge(EDGE* e1, EDGE* e2, double* e1_trans, double* e2_trans,  VERTEX3D &v1, VERTEX3D &v2);
double closestEdgeEdgeDraw(EDGE* e1, EDGE* e2, double* e1_trans, double* e2_trans,  VERTEX3D &v1, VERTEX3D &v2, int& segment_id);
double closestFaceFace(FACE* f1, FACE* f2, double* f1_trans, double* f2_trans,  VERTEX3D &v1, VERTEX3D &v2);

VERTEX3D closestEdgeFace(EDGE* e, FACE* f, double* e_trans, double* f_trans);
VERTEX3D closestFaceFace(FACE* f1, FACE* f2, double* f1_trans, double* f2_trans);

#endif