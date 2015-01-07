/*
 * Marker.cpp
 *
 *  Created on: Apr 16, 2009
 *      Author: yamokosk
 */

#include "Marker.h"

Marker::Marker() :
	id(0),
	name("_UNNAMED_"),
	bodyCoordinates( Vector3::ZERO ),
	worldCoordinates( Vector3::ZERO )
{
	// TODO Auto-generated constructor stub

}

Marker::Marker(unsigned int n, const std::string& name_in, const Vector3& bCoords) :
	id(n),
	name(name_in),
	bodyCoordinates(bCoords),
	worldCoordinates( Vector3::ZERO )
{

}

Marker::~Marker()
{
	// TODO Auto-generated destructor stub
}
