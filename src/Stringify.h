/*
 * Stringify.h
 *
 *  Created on: Dec 11, 2008
 *      Author: yamokosk
 */

#ifndef STRINGIFY_H_
#define STRINGIFY_H_

#include "SceneGraph.h"
#include "Archive.h"

#include <utility>
#include <string>

#include <boost/any.hpp>

#include <linalg/Vector3.h>
#include <linalg/Quaternion.h>
#include <linalg/MathExpression.h>

#include <api/ObjectModel.h>

namespace tinysg
{

typedef std::pair<std::string, std::string> StringTuple;

bool is_obj_empty(const boost::any & operand);
bool is_int(const boost::any & operand);
bool is_unsigned_long(const boost::any & operand);
bool is_real(const boost::any & operand);
bool is_char_ptr(const boost::any & operand);
bool is_string(const boost::any& operand);
bool is_scene_object_ptr(const boost::any & operand);
bool is_vector_3(const boost::any & operand);
bool is_quaternion(const boost::any & operand);

StringTuple stringify(const boost::any& obj);
boost::any destringify(const std::string& objectclass, const std::string value);

} // end namespace tinysg

#endif /* STRINGIFY_H_ */
