/*************************************************************************
 * SceneML, Copyright (C) 2007, 2008  J.D. Yamokoski
 * All rights reserved.
 * Email: yamokosk at gmail dot com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the License,
 * or (at your option) any later version. The text of the GNU Lesser General
 * Public License is included with this library in the file LICENSE.TXT.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the file LICENSE.TXT for
 * more details.
 *
 *************************************************************************/

#ifndef _MEX_STRUCT_DEF_H_
#define _MEX_STRUCT_DEF_H_

#define GRAPHIC_DEF		FIELD(Handle)		\
						FIELD(Mesh)			\
						FIELD(Color)		\
						FIELD(Alpha)

enum graphic_indices {
#define	FIELD(name) Graphic_##name,
	GRAPHIC_DEF
#undef FIELD
    NUM_GRAPHIC_FIELDS /* The number of existing commands */
};

const char *fnames_graphic[] = {
#define	FIELD(name) #name ,
	GRAPHIC_DEF
#undef FIELD
	NULL
};

/*
 * Object field definition
 */
#define OBJECT_DEF	FIELD(Name)			\
					FIELD(Type)			\
					FIELD(Properties)	\
					FIELD(WorldPose)	\
					FIELD(GraphicsData)

enum object_indices {
#define	FIELD(name) Object_##name,
	OBJECT_DEF
#undef FIELD
    NUM_OBJECT_FIELDS /* The number of existing commands */
};

const char *fnames_object[] = {
#define	FIELD(name) #name ,
	OBJECT_DEF
#undef FIELD
	NULL
};

/*
 * Node field definition
 */
#define NODE_DEF	FIELD(Name)			\
					FIELD(Parent)		\
					FIELD(WorldPose)	\
					FIELD(Objects)

enum node_indices {
#define	FIELD(name) Node_##name,
	NODE_DEF
#undef FIELD
    NUM_NODE_FIELDS
};

const char *fnames_node[] = {
#define	FIELD(name) #name ,
	NODE_DEF
#undef FIELD
	NULL
};

/*
 * Properties field definition
 */
#define PROPERTY_DEF	FIELD(Name)			\
						FIELD(Class)		\
						FIELD(Value)

enum property_indices {
#define	FIELD(name) Property_##name,
	PROPERTY_DEF
#undef FIELD
    NUM_PROPERTY_FIELDS
};

const char *fnames_property[] = {
#define	FIELD(name) #name ,
	PROPERTY_DEF
#undef FIELD
	NULL
};

/*
 * Contact field definition
 */
#define CONTACT_DEF	FIELD(POSITION, "position")			\
					FIELD(NORMAL, "normal")				\
					FIELD(DEPTH, "depth")				\
					FIELD(GEOM1, "geom1")				\
					FIELD(GEOM2, "geom2")

enum contact_indices {
#define	FIELD(ID, name) CONTACT_##ID,
	CONTACT_DEF
#undef FIELD
    NUM_CONTACT_FIELDS
};

const char *fnames_contact[] = {
#define	FIELD(ID, name) name ,
	CONTACT_DEF
#undef FIELD
	NULL
};

/*
 * Face/vertex field definition
 */
#define FV_DEF		FIELD(FACES, "faces")				\
					FIELD(VERTICES, "vertices")

enum fv_indices {
#define	FIELD(ID, name) FV_##ID,
	FV_DEF
#undef FIELD
    NUM_FV_FIELDS
};

const char *fnames_fv[] = {
#define	FIELD(ID, name) name ,
	FV_DEF
#undef FIELD
	NULL
};

#endif
