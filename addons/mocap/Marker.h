/*
 * Marker.h
 *
 *  Created on: Apr 16, 2009
 *      Author: yamokosk
 */

#ifndef MARKER_H_
#define MARKER_H_

#include <string>
#include <linalg/Vector3.h>

struct Marker
{
public:
	Marker();
	Marker(unsigned int, const std::string&, const Vector3&);
	virtual ~Marker();

	unsigned int id;
	std::string name;
	Vector3 bodyCoordinates;
	Vector3 worldCoordinates;
};

#endif /* MARKER_H_ */
