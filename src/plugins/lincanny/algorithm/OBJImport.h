#ifndef OBJIMPORT_H
#define OBJIMPORT_H

#include <stdio.h>
#include "Primitive.h"

int importOBJ(const char* filename, POLYHEDRON* mesh);
EDGE* addEdge(POLYHEDRON* mesh, int head_index, int tail_index);

#endif
