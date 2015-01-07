/*
 * Stringify.cpp
 *
 *  Created on: Oct 27, 2009
 *      Author: yamokosk
 */

#include "Stringify.h"

namespace tinysg
{

bool is_obj_empty(const boost::any & operand)
{
	return operand.empty();
}

bool is_int(const boost::any & operand)
{
	return (operand.type() == typeid(int));
}

bool is_unsigned_long(const boost::any & operand)
{
	return (operand.type() == typeid(unsigned long));
}

bool is_real(const boost::any & operand)
{
	return (operand.type() == typeid(Real));
}

bool is_char_ptr(const boost::any & operand)
{
	/*try
	{
		boost::any_cast<const char *>(operand);
		return true;
	}
	catch(const boost::bad_any_cast &)
	{
		return false;
	}*/
	return (operand.type() == typeid(char*));
}

bool is_string(const boost::any& operand)
{
	return ( operand.type() == typeid(std::string) );
	//try
	//{
		//return boost::any_cast<std::string>(&operand);
	//	return true;
	//}
	//catch (const boost::bad_any_cast &)
	//{
	//	return false;
	//}
}

bool is_scene_object_ptr(const boost::any & operand)
{
	try
	{
		boost::any_cast<SceneObject*>(operand);
		return true;
	}
	catch(const boost::bad_any_cast &)
	{
		return false;
	}
}

bool is_vector_3(const boost::any & operand)
{
	try
	{
		boost::any_cast<Vector3>(operand);
		return true;
	}
	catch(const boost::bad_any_cast &)
	{
		return false;
	}
}

bool is_quaternion(const boost::any & operand)
{
	try
	{
		boost::any_cast<Quaternion>(operand);
		return true;
	}
	catch(const boost::bad_any_cast &)
	{
		return false;
	}
}

StringTuple stringify(const boost::any& obj)
{
	if ( is_obj_empty(obj) ) {
		return StringTuple(Archive::EmptyValue, "");
	}

	if ( is_int(obj) ) {
		std::stringstream ss; ss << boost::any_cast<int>(obj);
		return StringTuple("int", ss.str());
	}

	if ( is_real(obj) ) {
		std::stringstream ss; ss << boost::any_cast<Real>(obj);
		return StringTuple("real", ss.str());
	}

	if ( is_unsigned_long(obj) ) {
		std::stringstream ss; ss << boost::any_cast<unsigned long>(obj);
		return StringTuple("int", ss.str());
	}

	if ( is_char_ptr(obj) ) {
		std::stringstream ss; ss << boost::any_cast<char*>(obj);
		return StringTuple("char_ptr", ss.str());
	}

	if ( is_string(obj) ) {
		std::stringstream ss; ss << boost::any_cast<std::string>(obj);
		return StringTuple("string", ss.str());
	}

	if ( is_vector_3(obj) ) {
		Vector3 v( boost::any_cast<Vector3>(obj) );
		return StringTuple("vector3", v.toString());
	}

	if ( is_quaternion(obj) ) {
		Quaternion q( boost::any_cast<Quaternion>(obj) );
		return StringTuple("quaternion", q.toString());
	}

	if ( is_scene_object_ptr(obj) ) {
		SceneObject* sceneObj = boost::any_cast<SceneObject*>(obj);
		ObjectInfo info;
		sceneObj->getInfo(info);
		return StringTuple("scene_object_ptr", info.name );
	}

	return StringTuple(Archive::UnknownValue, "");
}

boost::any destringify(const std::string& objectclass, const std::string value)
{
	if ( objectclass == "int")
	{
		return boost::any( ExpressionFactory::getAsInt(value) );
	}

	if ( objectclass == "real")
	{
		return boost::any( ExpressionFactory::getAsReal(value) );
	}

	if ( objectclass == "char_ptr")
	{
		return boost::any( value.c_str() );
	}

	if ( objectclass == "string")
	{
		return boost::any( value );
	}

	if ( objectclass == "vector3")
	{
		/*std::vector<Real> v;
		ExpressionFactory::getAsSequence(value, 3, v);
		Vector3 vec( v[0], v[1], v[2] );
		return boost::any( vec );*/
		return boost::any( ExpressionFactory::getAsSequence<Vector3>(value, 3) );
	}

	if ( objectclass == "quaternion")
	{
		/*std::vector<Real> v;
		ExpressionFactory::getAsSequence(value, 4, v);
		Quaternion q( v[0], v[1], v[2], v[3] );
		return boost::any( q );*/
		return boost::any( ExpressionFactory::getAsSequence<Quaternion>(value, 4) );
	}

	/*if ( objectclass == "scene_object_ptr")
	{
		SceneGraph& graph = SceneGraph::getInstance();
		return boost::any( graph.getObject(value) );
	}*/

	return boost::any();
}

}  // namespace tinysg
