#include "IDC.h"

double IDENITY[] = {1.0,0,0,0,0,1.0,0,0,0,0,1.0,0,0,0,0,1.0};
VERTEX3D ZERO_VECTOR;
double closestFeaturesInit(POLYHEDRON* poly1, POLYHEDRON* poly2, double* poly1_trans, double* poly2_trans, FEATURE* &f1, FEATURE* &f2, VERTEX3D &pos1, VERTEX3D &pos2, int debug){

	if(poly1->vertex_count < 1 || poly2->vertex_count < 1){
		return -0.0;
	}
	
	VERTEX3D* v1 = (poly1->vertices[0]);
	VERTEX3D* v2 = (poly2->vertices[0]);

	f1 = static_cast<FEATURE*>(v1);
	f2 = static_cast<FEATURE*>(v2); 

	return closestFeatures(poly1, poly2, poly1_trans, poly2_trans, f1, f2, pos1, pos2, debug, 400);
}
	
double closestFeatures(POLYHEDRON* poly1, POLYHEDRON* poly2, double* poly1_trans, double* poly2_trans,FEATURE* &f1, FEATURE* &f2, VERTEX3D &pos1, VERTEX3D &pos2, int debug, int depth){
	
	if(poly1->vertex_count < 1 || poly2->vertex_count < 1){
		return -0.0;
	}

	VERTEX3D* v1 = (poly1->vertices[0]);
	VERTEX3D* v2 = (poly2->vertices[0]);
	EDGE *edge1, *edge2; 
	FACE *face1, *face2;
	
	f1 = static_cast<FEATURE*>(f1);
	f2 = static_cast<FEATURE*>(f2); 
	
	int z = 0;	
	int dp = 0;
	int stop = depth;

	VERTEX3D edge_point;
	VERTEX3D edge_point1, edge_point2;
							
	bool search, search_r1, search_r2;
	search = search_r1 = search_r2 = true;
	while(search){
		
		switch(f1->id){
			case VERTEX_FEATURE:
				v1 = static_cast<VERTEX3D*>(f1);
				break;
			case EDGE_FEATURE:
				edge1 = static_cast<EDGE*>(f1);
				break;
			case FACE_FEATURE:
				face1 = static_cast<FACE*>(f1);
				break;
		}
		while(search_r2){	
			dp ++;
		//	printf("2, %i:\n", debug);
			switch(f2->id){
				case VERTEX_FEATURE:
					//printf("vertex %i - ",v2->index);
					v2 = static_cast<VERTEX3D*>(f2);
					switch(f1->id){
						case VERTEX_FEATURE:	
						//	printf("vertex %i\n",v1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}
							if(point_vertex(v1, v2, poly1_trans, poly2_trans, f2)){
								search_r2 = false;
							}
							
							break;
						case EDGE_FEATURE:
							//printf("edge %i\n", edge1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}
							closestPointEdge(v2, edge1, poly2_trans, poly1_trans, edge_point);
							if(point_vertex(&edge_point, v2, IDENITY, poly2_trans, f2)){
								search_r2 = false;
							}
							
							break;
						
						case FACE_FEATURE:
							//printf("face %i\n",face1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}
							
							VERTEX3D face_point; 
							
							closestPointFace(v2, face1, poly2_trans, poly1_trans, face_point);
							if(point_vertex(&face_point, v2, IDENITY, poly2_trans, f2)){
								search_r2 = false;
							}
							
							
							break;
					}				
					break;	
				case EDGE_FEATURE:			
					edge2 = static_cast<EDGE*>(f2);
					//printf("edge %i - ",edge2->index);
					switch(f1->id){			
						case VERTEX_FEATURE:		
							//printf("vertex %i\n",v1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}
							if(point_edge(v1, edge2, poly1_trans, poly2_trans, f2)){
								search_r2 = false;
							}
						
							break;
						case EDGE_FEATURE:
							//printf("edge %i\n",edge1->index);
							if(z++ == stop){
								search = search_r2 = search_r1 = false;
								break;
							}
							if(edge_edge(edge1, edge2, poly1_trans, poly2_trans, f2)){
								search_r2 = false;
							}
							break;
						
						case FACE_FEATURE:
							//printf("face %i\n",face1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}
							if(face_edge(face1,edge2,poly1_trans,poly2_trans,f2)){
								search_r2 = false;
							}
							
							break;
					}
					break;				
				case FACE_FEATURE:
					face2 = static_cast<FACE*>(f2);
					//printf("face %i - ",face2->index);
					switch(f1->id){
						case VERTEX_FEATURE:
							//printf("vertex %i\n",v1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}
							if(point_face(v1, face2, poly1_trans, poly2_trans, f2)){
								search_r2 = false;
							}
							break;
						case EDGE_FEATURE:
							//printf("edge %i\n",edge1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}
							if(edge_face(edge1, face2, poly1_trans, poly2_trans, f2)){
								search_r2 = false;
							}
							break;
						case FACE_FEATURE:	
							//printf("face %i\n",face1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}					
							if(face_face(face1, face2, poly1_trans, poly2_trans, f2)){
								search_r2 = false;
							}
							break;
					}
					
					break;
			}
		}
		switch(f2->id){
			case VERTEX_FEATURE:
				v2 = static_cast<VERTEX3D*>(f2);
				break;
			case EDGE_FEATURE:
				edge2 = static_cast<EDGE*>(f2);
				break;
			case FACE_FEATURE:
				face2 = static_cast<FACE*>(f2);
				break;
		}

		while(search_r1){
				dp ++;
				//printf("1, %i:", debug);
				switch(f1->id){
					case VERTEX_FEATURE:
						v1 = static_cast<VERTEX3D*>(f1);	
						//printf("vertex %i - ",v1->index);
						switch(f2->id){				
							case VERTEX_FEATURE:
								//printf("vertex\n", v2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}
								if(point_vertex(v2, v1, poly2_trans, poly1_trans, f1)){
									search_r1 = false;
								}
								
								break;
							case EDGE_FEATURE:
								//printf("edge %i\n",edge2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}
								closestPointEdge(v1, edge2, poly1_trans, poly2_trans,edge_point);
							if(point_vertex(&edge_point, v1, IDENITY, poly1_trans, f1)){
									search_r1 = false;
								}	
								
								break;
							case FACE_FEATURE:
								//printf("face %i\n", face2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}
								VERTEX3D face_point; 
								closestPointFace(v1, face2, poly1_trans, poly2_trans, face_point);
								//printf("closest point/face:\n");
								//printf("%f\n%f\n%f\n",face_point.x,face_point.y,face_point.z);
								if(point_vertex(&face_point, v1, IDENITY, poly1_trans, f1)){
									search_r1 = false;
								}
								break;
						}
						break;
					case EDGE_FEATURE:
						edge1 = static_cast<EDGE*>(f1);
						//printf("edge %i- ",edge1->index);
						switch(f2->id){
							case VERTEX_FEATURE:
								//printf("vertex\n");
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}
								if(point_edge(v2, edge1, poly2_trans, poly1_trans, f1)){
									search_r1 = false;
								}
								break;
							case EDGE_FEATURE:
								//printf("edge %i\n", edge2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}
								if(edge_edge(edge2, edge1, poly2_trans,poly1_trans, f1)){
									search_r1 = false;
								}
								break;
							case FACE_FEATURE:
								//printf("face %i\n", face2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}	
								if(face_edge(face2, edge1, poly2_trans, poly1_trans, f1)){
									search_r1 = false;
								}
								break;
						}
						break;
					case FACE_FEATURE:
						face1 = static_cast<FACE*>(f1);
					//	printf("face %i- ", face1->index);
						switch(f2->id){
							case VERTEX_FEATURE:
								//printf("vertex %i\n", v2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}	
								if(point_face(v2, face1, poly2_trans, poly1_trans, f1)){
									search_r1 = false;
								}
								break;
							case EDGE_FEATURE:
								//printf("edge %i\n",edge2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}
								if(edge_face(edge2, face1, poly2_trans, poly1_trans, f1)){
									search_r1 = false;
								}
								break;
							case FACE_FEATURE:	
								//printf("face %i\n", face2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}	
								if(face_face(face2, face1, poly2_trans, poly1_trans, f1)){
									search_r1 = false;
								}
								break;
						}
						break;
			}
		}
		if(confirmClosest(f1, f2, poly1_trans, poly2_trans)){
			//printf("closest confirmed!!\n");
			search = false;
		}
		else{
			search_r1 = search_r2 = true;
		}
	}

	double d;
	
	switch(f1->id){
		
		case VERTEX_FEATURE:
			v1->transform(poly1_trans, pos1);
			switch(f2->id){
				case VERTEX_FEATURE:
					(static_cast<VERTEX3D*>(f2))->transform(poly2_trans, pos2);	
					break;
				case EDGE_FEATURE:
					d = closestPointEdge(v1, static_cast<EDGE*>(f2), poly1_trans, poly2_trans, pos2);
					break;
				case FACE_FEATURE:
					d = closestPointFace(v1, static_cast<FACE*>(f2), poly1_trans, poly2_trans, pos2);		
					break;
			}
			break;
		case EDGE_FEATURE:
			switch(f2->id){
				case VERTEX_FEATURE:
					closestPointEdge(static_cast<VERTEX3D*>(f2), edge1, poly2_trans, poly1_trans, pos1);
					(static_cast<VERTEX3D*>(f2))->transform(poly2_trans, pos2);
					break;
				case EDGE_FEATURE:
					closestEdgeEdge(edge1, edge2, poly1_trans, poly2_trans, pos1, pos2);
					break;
				case FACE_FEATURE:
					//hack so zero is not returned - needs fixin'
					((*edge1->head + *edge1->tail)/2.0).transform(poly1_trans, pos1);
					face2->d.transform(poly2_trans, pos2);
					break;


			}
			break;
		case FACE_FEATURE:
			switch(f2->id){
				case VERTEX_FEATURE:
					d = closestPointFace(static_cast<VERTEX3D*>(f2), face1, poly2_trans, poly1_trans, pos1);
					(static_cast<VERTEX3D*>(f2))->transform(poly2_trans, pos2);
					break;
				case EDGE_FEATURE:
					//hack so zero is not returned - needs fixin'
					((*edge2->head + *edge2->tail)/2.0).transform(poly2_trans, pos1);
					face1->d.transform(poly1_trans, pos1);
					break;
				case FACE_FEATURE:
					//hack so zero is not returned - needs fixin'
					face1->d.transform(poly1_trans, pos1);
					face2->d.transform(poly2_trans, pos2);
					break;
			}
			break;
	}

	if(dp > stop-1){
		///printf("OVERFLOW!!!\n");
		//while(true);
	}
	//if((pos1 - pos2).mag() < 0.0001){
	//	printf("f1 = %i, f2 = %i\n", f1->id, f2->id);
	//	printf("ZERO\n");
	//}

	return (pos1 - pos2).mag();
}

bool confirmClosest(FEATURE* f1, FEATURE* f2, double* f1_trans, double* f2_trans){
	VERTEX3D* v1, *v2;
	EDGE* edge1, *edge2;
	FACE* face1,* face2;
	FEATURE* f_nil;
	switch(f1->id){
		case VERTEX_FEATURE:
			v1 = static_cast<VERTEX3D*>(f1);
			break;
		case EDGE_FEATURE:
			edge1 = static_cast<EDGE*>(f1);
			break;
		case FACE_FEATURE:
			face1 = static_cast<FACE*>(f1);
			break;
	}

	VERTEX3D face_point, edge_point1, edge_point2;				
	switch(f2->id){
		case VERTEX_FEATURE:
			v2 = static_cast<VERTEX3D*>(f2);
			switch(f1->id){
				case VERTEX_FEATURE:	
					return point_vertex(v1, v2, f1_trans, f2_trans, f_nil);		
				case EDGE_FEATURE:
					closestPointEdge(v2, edge1, f2_trans, f1_trans, edge_point1);
					return point_vertex(&edge_point1, v2, IDENITY, f2_trans, f_nil);
				case FACE_FEATURE:
					closestPointFace(v2, face1, f2_trans, f1_trans, face_point);
					return point_vertex(&face_point, v2, IDENITY, f2_trans, f2);
			}					
			break;
		case EDGE_FEATURE:
			edge2 = static_cast<EDGE*>(f2);
			switch(f1->id){			
				case VERTEX_FEATURE:		
					return point_edge(v1, edge2, f1_trans, f2_trans, f_nil);
				case EDGE_FEATURE:
					return edge_edge(edge1, edge2, f1_trans, f2_trans, f_nil);
				case FACE_FEATURE:
					return face_edge(face1, edge2, f1_trans, f2_trans,f_nil);	
			}
			break;
		case FACE_FEATURE:
			face2 = static_cast<FACE*>(f2);
			switch(f1->id){
				case VERTEX_FEATURE:
					return point_face(v1, face2, f1_trans, f2_trans, f_nil);
				case EDGE_FEATURE:
					return edge_face(edge1, face2, f1_trans, f2_trans, f_nil);
				case FACE_FEATURE:					
					return face_face(face1, face2, f1_trans, f2_trans, f_nil);
							
			}					
			break;
	}
	return true;
}

bool point_vertex(VERTEX3D* p, VERTEX3D* v, double*p_trans, double* v_trans, FEATURE* &f){
	VERTEX3D p_global;
	VERTEX3D v_global;
	p->transform(p_trans, p_global);
	v->transform(v_trans, v_global);
	for(int i=0;i<v->adjacency_size;i++){
		EDGE* e = v->adjacency[i];
		VECTOR3D n;	
		VECTOR3D head_global, tail_global;
		e->head->transform(v_trans, head_global);
		e->tail->transform(v_trans, tail_global);
		if(e->head->index == v->index){
			n = head_global - tail_global;
		}
		else{
			n = tail_global - head_global;
		}
		if(!pointInFrontOfPlane(v_global, n, p_global)){	
			f = static_cast<FEATURE*>(e);
			return false;			
		}
	}
	return true;
}


bool point_edge(VERTEX3D* p, EDGE* e, double* p_trans, double* e_trans, FEATURE* &f){	
	VERTEX3D p_global;
	VERTEX3D head_global;
	VERTEX3D tail_global;
	
	p->transform(p_trans, p_global);
	e->head->transform(e_trans, head_global);
	e->tail->transform(e_trans, tail_global);

	VECTOR3D n = tail_global - head_global;
	if(!pointInFrontOfPlane(head_global, n, p_global)){
		f = static_cast<FEATURE*>(e->head);
		return false;
	}
	
	n = head_global - tail_global;
	if(!pointInFrontOfPlane(tail_global, n, p_global)){
		f = static_cast<FEATURE*>(e->tail);
		return false;
	}
	//printf("8");
	n = tail_global - head_global;
	VECTOR3D face_n;
	e->left_face->n.rotate(e_trans, face_n);
	VECTOR3D cross = n.cross(face_n);
	if(!pointInFrontOfPlane(head_global, cross, p_global)){
		f = static_cast<FEATURE*>(e->left_face);
		return false;
	}

	n = head_global - tail_global;
//	printf("b");
//	printf("edge index = %i",e->index);
	e->right_face->n.rotate(e_trans, face_n);
//	printf("a");
	cross = n.cross(face_n);
	if(!pointInFrontOfPlane(tail_global, cross, p_global)){
		f = static_cast<FEATURE*>(e->right_face);
		return false;
	}
//	printf("true\n");
	f = static_cast<FEATURE*>(e);
	return true;
}

bool point_face(VERTEX3D* p, FACE* face, double* p_trans, double* face_trans, FEATURE* &f){	
	VERTEX3D p_global;
	p->transform(p_trans, p_global);
	VECTOR3D n;
	//printf("5");
	face->n.rotate(face_trans, n);
	
	for(int i=0;i<face->vertex_count;i++){
		VERTEX3D head_global;
		VERTEX3D tail_global;
		face->edges[i]->head->transform(face_trans, head_global);
		face->edges[i]->tail->transform(face_trans, tail_global);
		
		VECTOR3D cross;
		int hid = face->getVertexIndex(face->edges[i]->head) + 1;
		int tid = face->getVertexIndex(face->edges[i]->tail) + 1;
		if((tid)%face->vertex_count == (hid +1)%face->vertex_count){
			cross = n.cross(tail_global - head_global);
			if(!pointInFrontOfPlane(tail_global, cross, p_global)){
				f = static_cast<FEATURE*>(face->edges[i]);
				return false;
			}
		}
		else{
			cross = n.cross(head_global - tail_global);
			if(!pointInFrontOfPlane(head_global, cross, p_global)){
				f = static_cast<FEATURE*>(face->edges[i]);
				return false;
			}
		}
	}

	VECTOR3D pos; 
	face->d.transform(face_trans, pos);
	if(!pointInFrontOfPlane(pos,n,p_global)){
		//printf("BEHIND_PLANE: %i\n",face->index);
		EDGE* e = face->edges[face->idc_edge];
		face->idc_edge++;
		face->idc_edge %= face->vertex_count;
		if(face->getEdgeSide(e) == LEFT_FACE){
			f = static_cast<FEATURE*>(e->right_face);
		//	printf("R:face id = %i\n",e->right_face->index);
		}
		else{
			f = static_cast<FEATURE*>(e->left_face);
			//printf("L:face id = %i\n",e->left_face->index);
		}	
		return false;	
	}

	return true;
	
}

bool edge_edge(EDGE* e1, EDGE* e2, double* e1_trans, double* e2_trans, FEATURE* &f){
	VERTEX3D p1, p2;
	closestEdgeEdge(e1, e2, e1_trans, e2_trans, p1, p2);
//	printf("closest edge/edge:\n");
//	printf("%f, %f, %f\n",p1.x,p1.y,p1.z);
//	printf("%f, %f, %f\n",p2.x,p2.y,p2.z);
	return point_edge(&p1, e2, IDENITY, e2_trans, f);
}	

bool face_edge(FACE* face, EDGE* edge, double* face_trans, double* edge_trans, FEATURE* &f){
	if(parallelEdgeFace(edge, face, edge_trans, face_trans)){
		//printf("PARALLEL: face-edge!!! \n");
		
		VERTEX3D p1;
		int f_id;
		if(edgeVisible(edge, &face->d, edge_trans, face_trans, f_id)){
			return true;
		}
		double head_dist = closestPointFace(edge->head, face, edge_trans, face_trans, p1);	
		double tail_dist = closestPointFace(edge->tail, face, edge_trans, face_trans, p1);	

		if(head_dist < tail_dist){
			f = static_cast<FEATURE*>(edge->head);
			return false;
		}
		else{
			f = static_cast<FEATURE*>(edge->tail);
			return false;
		}
		/*	
		int face_id;
		if(edgeVisible(edge, &ZERO_VECTOR, edge_trans, face_trans, face_id)){
			return true;
		}
		*/
		/*	if(point_face(&p1, face, edge_trans, face_trans,f)){
			if(point_face(edge->head, face, edge_trans,face_trans, f)){
				printf("1\n");
				f = static_cast<FEATURE*>(edge->head);
				return false;
			}
			if(point_face(edge->tail, face, edge_trans, face_trans, f)){
				printf("2\n");
				f = static_cast<FEATURE*>(edge->tail);
				return false;
			}
			
		
			else{
				if(face_id == LEFT_FACE){
					printf("3\n");
					f = static_cast<FEATURE*>(edge->left_face);
				}
				else{
					printf("4\n");
					f = static_cast<FEATURE*>(edge->right_face);
				}
				return false;
			}
			
		}
		else{
			int face_id;
			edgeVisible(edge, &ZERO_VECTOR, edge_trans, face_trans, face_id);
			if(face_id == LEFT_FACE){
				printf("3b\n");
				f = static_cast<FEATURE*>(edge->left_face);
			}
			else{
				printf("4b\n");
				f = static_cast<FEATURE*>(edge->right_face);
			}
			return false;
		}
		*/
	}
	VERTEX3D pos;
	VERTEX3D head_global, tail_global, n_global;	
	face->d.transform(face_trans, pos);
	edge->head->transform(edge_trans, head_global);
	edge->tail->transform(edge_trans, tail_global);
	face->n.rotate(face_trans, n_global);

	bool head, tail;
	double head_dist, tail_dist;
	head = tail = false;

	if(point_face(edge->head, face, edge_trans, face_trans, f)){
		head = true;
		head_dist = closestPointFace(edge->head,face, edge_trans, face_trans, pos);
	}

	if(point_face(edge->tail, face, edge_trans, face_trans, f)){
		tail = true;
		tail_dist = closestPointFace(edge->tail,face, edge_trans, face_trans, pos);
	}

	if(head && tail){
		if(head_dist < tail_dist){
			f = static_cast<FEATURE*>(edge->head);
			return false;
		}
		else{
			f = static_cast<FEATURE*>(edge->tail);
			return false;
		}
	}
	else{
		if(head){
			if((head_global-tail_global).dot(n_global)< 0.00 ){
				f = static_cast<FEATURE*>(edge->head);
				return false;
			}
			else{
				f = static_cast<FEATURE*>(edge);
				return true;
			}
	
		}
		else if(tail){
			if((tail_global-head_global).dot(n_global)< 0.00 ){
				f = static_cast<FEATURE*>(edge->tail);
				return false;
			}
			else{
				f = static_cast<FEATURE*>(edge);
				return true;
			}
		}
	}
	f = static_cast<FEATURE*>(edge);
	return true;
}

	
bool edge_face(EDGE* edge, FACE* face, double* edge_trans, double* face_trans, FEATURE* &f){	
	if(!face_edge(face,edge,face_trans, edge_trans,f)){
		f = static_cast<FEATURE*>(face);
		return true;
	}
	
	if(parallelEdgeFace(edge, face, edge_trans, face_trans)){
		/****
		FIX THIS!!!!!
		****/
		/*
		printf("PARALLEL: edge-face\n");
		VERTEX3D p1;
		closestPointFace(edge->head, face, edge_trans, face_trans, p1);	
		if(point_face(&p1, face, edge_trans, face_trans,f)){
			//f = static_cast<FEATURE*>(edge);
			return true;
		}
		//f = static_cast<FEATURE*>(edge->head);
		return false;
	*///	printf("PARALLEL: edge-face\n");
		return true;
	}

	VERTEX3D p1, p2;
	int closest_index = 0; 
	double closest = closestEdgeEdge(edge, face->edges[0], edge_trans, face_trans, p1, p2);
	
	for(int i=1;i<face->vertex_count;i++){
		double d = closestEdgeEdge(edge, face->edges[i], edge_trans, face_trans, p1, p2);
		if(d < closest){
			closest = d;
			closest_index = i;
		}
	}
	f = static_cast<FEATURE*>(face->edges[closest_index]); 
	return false;
}

bool face_face(FACE* face1, FACE* face2, double* face1_trans, double* face2_trans, FEATURE* &f){
	if(parallelFaceFace(face1, face2, face1_trans, face2_trans)){
		return overlapTest(face1, face2,face1_trans,face2_trans, f);
	}
	
	VERTEX3D p_closest;
	double min_dist = closestPointFace(face1->vertices[0], face2, face1_trans, face2_trans, p_closest);

	VERTEX3D p_temp;
	for(int i=1;i<face1->vertex_count;i++){
		double temp_dist  = closestPointFace(face1->vertices[0], face2, face1_trans, face2_trans, p_temp);
		if(temp_dist < min_dist){
			min_dist = temp_dist;
			p_closest = p_temp;
		}
	}
	return point_face(&p_closest, face2, IDENITY, face2_trans, f);
}

double closestPointEdge(VERTEX3D* p, EDGE* e, double* p_trans, double* e_trans, VERTEX3D &v){
	VERTEX3D p_global, head_global, tail_global;
	p->transform(p_trans, p_global);
	e->head->transform(e_trans, head_global);
	e->tail->transform(e_trans, tail_global);
	VECTOR3D ev = tail_global - head_global;
	ev.normalize();
	VECTOR3D pv = p_global-head_global;
	double d = pv.dot(ev);	
	
	v = head_global + ev*d;

	VECTOR3D a = v - head_global;
	VECTOR3D b = tail_global - head_global;
	
	if(a.dot(b) < 0.0){
		v = head_global;	
	}
	else{
		double e_mag = (tail_global - head_global).magSquared();
		double v_mag = (v - head_global).magSquared();
		if(v_mag > e_mag){
			v = tail_global;
		}
	}
	return (p_global - v).mag();
}

double closestPointFace(VERTEX3D* p, FACE* f, double* p_trans, double* f_trans, VERTEX3D &v){
	VERTEX3D p_global, d_global;
	p->transform(p_trans, p_global);
	f->d.transform(f_trans, d_global);
	VECTOR3D pv = p_global - d_global;
	VECTOR3D nv;
//	printf("1");
	f->n.rotate(f_trans, nv);
	double d = pv.dot(nv);
	v = p_global - nv*d;	
	return (p_global - v).mag();
	
	
	/*
	VERTEX3D p_global;
	p->transform(p_trans, p_global);
	VECTOR3D n;
	face->n.rotate(face_trans, n);
	
	for(int i=0;i<face->vertex_count;i++){
		VERTEX3D head_global;
		VERTEX3D tail_global;
		face->edges[i]->head->transform(face_trans, head_global);
		face->edges[i]->tail->transform(face_trans, tail_global);
		
		VECTOR3D cross;
		int hid = face->getVertexIndex(face->edges[i]->head) + 1;
		int tid = face->getVertexIndex(face->edges[i]->tail) + 1;
		if((tid)%face->vertex_count == (hid +1)%face->vertex_count){
			cross = n.cross(tail_global - head_global);
			if(!pointInFrontOfPlane(tail_global, cross, p_global)){
				f = static_cast<FEATURE*>(face->edges[i]);
				return false;
			}
		}
		else{
			cross = n.cross(head_global - tail_global);
			if(!pointInFrontOfPlane(tail_global, cross, p_global)){
				f = static_cast<FEATURE*>(face->edges[i]);
				return false;
			}
		}
	}
	*/
}


double closestEdgeEdge(EDGE* e1, EDGE* e2, double* e1_trans, double* e2_trans, VERTEX3D &v1, VERTEX3D &v2){ 
	
	VECTOR3D e1_head_global, e1_tail_global;
	VECTOR3D e2_head_global, e2_tail_global;
	
	e1->head->transform(e1_trans, e1_head_global);
	e1->tail->transform(e1_trans, e1_tail_global);	
	VECTOR3D u = e1_tail_global - e1_head_global;
	
	e2->head->transform(e2_trans, e2_head_global);
	e2->tail->transform(e2_trans, e2_tail_global);	
	VECTOR3D v = e2_tail_global - e2_head_global;
	
	VECTOR3D w = e1_head_global - e2_head_global;
	
	double a = u.magSquared();
	double b = u.dot(v);
	double c = v.magSquared();
	double d = u.dot(w);
	double e = v.dot(w);
	double denom = a*c - b*b;
	
	double sc, sN, sD = denom;
	double tc, tN, tD = denom;
	
	if(denom < EPS){
		sN = 0.0;
		sD = 1.0;
		tN = e;
		tD = c;
	}
	else{
		sN = (b*e - c*d);
		tN = (a*e - b*d);
		if(sN < 0.0){
			sN = 0.0;
            tN = e;
            tD = c;
		}
		else if(sN > sD){  
            sN = sD;
            tN = e + b;
            tD = c;
        }
	}
	if(tN < 0.0) {        
        tN = 0.0;
		if(-d < 0.0){
            sN = 0.0;
		}
		else if(-d > a){
            sN = sD;
		}
        else{
            sN = -d;
            sD = a;
        }
    }
	else if (tN > tD) {
       tN = tD;
	   if((-d + b) < 0.0){
           sN = 0;
	   }
	   else if((-d + b) > a){
           sN = sD;
	   }
       else{
           sN = (-d + b);
           sD = a;
       }
    }
	sc = (abs(sN) < EPS ? 0.0 : sN / sD);
    tc = (abs(tN) < EPS ? 0.0 : tN / tD);


	v1 = e1_head_global + u*sc;
	v2 = e2_head_global + v*tc;
	
	return (v2-v1).mag();
}

bool pointInFrontOfPlane(VECTOR3D pos, VECTOR3D n, VERTEX3D p){
	//printf("d = %f\n", (n).dot(p-pos));
	return ((n).dot(p-pos) > -.001);
}

bool edgeVisible(EDGE* e, VERTEX3D* p, double* e_trans, double* p_trans, int& face_id){
	VERTEX3D disp, orign, left_global, right_global, p_global, e_global;
	e->left_face->n.rotate(e_trans, left_global);
	e->right_face->n.rotate(e_trans, right_global);
	p->transform(p_trans, p_global);
	orign.transform(e_trans, e_global);
	disp = p_global - e_global;	
	double d_left = disp.dot(left_global);
	double d_right = disp.dot(right_global);
	
	if(d_left < d_right){
		face_id = LEFT_FACE;
	}
	else{
		face_id = RIGHT_FACE;
	}

	if(d_left > -EPS && d_right > -EPS){
		return true;
	}
	else{
		return false;	
	}
}

bool parallelEdgeFace(EDGE* e, FACE* f , double* e_trans, double* f_trans){
	VECTOR3D head_global, tail_global, n_global;
	e->head->transform(e_trans, head_global);
	e->tail->transform(e_trans, tail_global);
	f->n.rotate(f_trans, n_global);	
	double d = (tail_global - head_global).dot(n_global);
	return (d >-EPS && d<EPS);	
}

bool parallelFaceFace(FACE* face1, FACE* face2, double* face1_trans, double* face2_trans){
	VECTOR3D n1_global, n2_global;
	face1->d.rotate(face1_trans, n1_global);
	face2->d.rotate(face2_trans, n2_global);
	double d = n1_global.dot(n2_global);
	if((d > 1.0-EPS && d <1.0+EPS) || (d > -1.0-EPS && d <-1.0+EPS)){
		return true;
	}
	return false;
}

bool overlapTest(FACE* f1, FACE* f2, double* f1_trans, double* f2_trans, FEATURE* &f){
	for(int i=0;i<f2->vertex_count;i++){
		if(point_face(f2->vertices[i], f1, f2_trans, f1_trans, f)){
			return true;
		}
	}
	for(int i=0;i<f1->vertex_count;i++){
		if(point_face(f1->vertices[i], f2, f1_trans, f2_trans, f)){
			f = static_cast<FEATURE*>(f2);
			return true;
		}
	}
	return false;
}