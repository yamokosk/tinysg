#include "OBJImport.h"
#include <cstring>
#include <fstream>
#include <iostream>

//#define BUG_importOBJ

int importOBJ(const char* filename, POLYHEDRON* mesh){

	// Open the file
	std::fstream file;
	file.open(filename, std::ios_base::in);

	if( !file.good() )
	{
		file.close();
		std::cerr << "File \"" << filename << "\" not found." << std::endl;
		return -1;
	}

	//collect number of features of the polyhedron
	mesh->vertex_count = 0;
	mesh->face_count = 0;
	mesh->edge_count = 0;
	char buf[256];
	while( !file.eof() )
	{
		file.getline(buf,sizeof(buf));
		switch(buf[0])
		{
			case 'v': {
				if(buf[1] == ' ') mesh->vertex_count++;
				break;
			}
			case 'f': {
				if(buf[1] == ' ') mesh->face_count++;
				break;
			}
			default:
				break;
		}
	}
	file.clear();              // forget we hit the end of file
	file.seekg(0, std::ios::beg);   // move to the start of the file

	//fill feature arrays
	mesh->vertices = new VERTEX3D*[mesh->vertex_count];
	mesh->faces = new FACE*[mesh->face_count];
	int edge_count = mesh->face_count + mesh->vertex_count - 2; //Euler's formula
	mesh->edges = new EDGE*[edge_count];

#ifdef BUG_importOBJ
	std::cout << "Faces: " << mesh->face_count << ", Vertices: " << mesh->vertex_count << std::endl;
#endif

	int v = 0;
	int f = 0;
	double vbuf[3] = {0.0, 0.0, 0.0};
	while( !file.eof() )
	{
		file.getline(buf,sizeof(buf));
		switch (buf[0])
		{
			case 'v':
			{
				if(buf[1] == ' ')
				{
					sscanf( (buf+2), "%lf %lf %lf", vbuf, vbuf+1, vbuf+2);
#ifdef BUG_importOBJ
					std::cout << "Case 'v': " << vbuf[0] << ", " << vbuf[1] << ", " << vbuf[2] << std::endl;
#endif
					mesh->vertices[v] = new VERTEX3D(vbuf[0],vbuf[1],vbuf[2]);
					mesh->vertices[v]->index = v;
					v++;
				}
				break;
			}
			case 'f':
			{
				if(buf[1] == ' ')
				{
					unsigned int ids[4] = {0, 0, 0};
					char *tok; unsigned int nverts = 0;
					tok = strtok((buf+2)," ");
					while (tok != NULL)
					{
						if ( nverts > 3 )
						{
							file.close();
							std::cerr << "There is an problem with the faces in the obj file. please use quads or triangles only: exiting import routine" << std::endl;
							return -1;
						}

						char *tmp = NULL, index[10];
						if ( (tmp = strstr(tok, "/")) != NULL )
						{
							// A slash was found. Just need to extract the first digit
							memset(index, '\0', sizeof(index));
							memcpy(index, tok, (tmp - tok)*sizeof(char));
							ids[nverts] = atoi(index)-1;
							nverts++;
						} else {
							// No slash was found. Just process as if it is a number
							ids[nverts] = atoi(index)-1;
							nverts++;
						}
						tok = strtok(NULL, " ");
					}

#ifdef BUG_importOBJ
					std::cout << "Case 'f' (" << nverts << "): " << ids[0] << ", " << ids[1] << ", " << ids[2] << ", " << ids[4] << std::endl;
#endif
					int v1 = ids[0];
					int v2 = ids[1];
					int v3 = ids[2];
					int v4 = ids[3];

					if( nverts == 3 )
					{
						//triangle
						EDGE* e1 = addEdge(mesh, v1, v2);
						EDGE* e2 = addEdge(mesh, v2, v3);
						EDGE* e3 = addEdge(mesh, v3, v1);

						mesh->vertices[v1]->addToAdjacency(e1);
						mesh->vertices[v1]->addToAdjacency(e3);

						mesh->vertices[v2]->addToAdjacency(e1);
						mesh->vertices[v2]->addToAdjacency(e2);

						mesh->vertices[v3]->addToAdjacency(e2);
						mesh->vertices[v3]->addToAdjacency(e3);

						mesh->faces[f] = new FACE(mesh->vertices[v1],mesh->vertices[v2],mesh->vertices[v3], e1, e2, e3);
					}
					else
					{
						//quad
						EDGE* e1 = addEdge(mesh, v1, v2);
						EDGE* e2 = addEdge(mesh, v2, v3);
						EDGE* e3 = addEdge(mesh, v3, v4);
						EDGE* e4 = addEdge(mesh, v4, v1);

						mesh->vertices[v1]->addToAdjacency(e1);
						mesh->vertices[v1]->addToAdjacency(e4);

						mesh->vertices[v2]->addToAdjacency(e1);
						mesh->vertices[v2]->addToAdjacency(e2);

						mesh->vertices[v3]->addToAdjacency(e2);
						mesh->vertices[v3]->addToAdjacency(e3);

						mesh->vertices[v4]->addToAdjacency(e3);
						mesh->vertices[v4]->addToAdjacency(e4);


						mesh->faces[f] = new FACE(mesh->vertices[v1],mesh->vertices[v2],mesh->vertices[v3],mesh->vertices[v4], e1, e2, e3, e4);
					}
					mesh->faces[f]->index = f;
					f++;
				}
				break;
			}
			default:
				break;
		}
	}

	//close file
	file.close();
	return 0;
}


EDGE* addEdge(POLYHEDRON* mesh,int head_index,int tail_index){
	for(int i=0;i<mesh->edge_count;i++){
		if((mesh->edges[i]->head->index == head_index) && (mesh->edges[i]->tail->index == tail_index) ){
			return mesh->edges[i];
		}
		else if((mesh->edges[i]->head->index == tail_index) && (mesh->edges[i]->tail->index == head_index)){
			return mesh->edges[i];
		}
	}
	EDGE* e = new EDGE(mesh->vertices[head_index], mesh->vertices[tail_index]);
	mesh->edges[mesh->edge_count] = e;
	mesh->edges[mesh->edge_count]->index = mesh->edge_count++;
	return e;
}
