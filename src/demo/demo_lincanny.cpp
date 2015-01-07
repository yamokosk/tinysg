#include "demowrapper.h"

void description()
{
	std::cout
		<< " --== Distance query demo with TinySG ==--\n\n"
		<< "\tCreates a simple 4 node (n1,n2,n3, and n4) scene. Two objects\n"
		<< "are created and attached to nodes n2 and n3 respectively. Distance\n"
		<< "queries are then performed on the two objects for different poses of\n"
		<< "of the parent node of n3.\n\n";
}

void printme (const PropertyContainer& props)
{
	BOOST_FOREACH(Property prop, props)
	{
		std::string msg = prop.value_str();
		std::cout << msg << std::endl;
	}
}

void dome(const ObjectInfo& info)
{
	std::cout << "Size of meshes: " << info.meshes.size() << std::endl;
}

bool rundemo(int argc, char **argv)
{
	description();

	TinySG::Initialize();

	//SceneLoader::load("/Users/yamokosk/Workspace", *g_SceneGraph);

	/* Create the simple scene */
	SceneGraph graph;
	createSimpleGraph(graph);

	/* Create two Lin-canny objects */
	PropertyContainer cube_properties;
	cube_properties.push_back( Property("filename", std::string("b0_01.obj")) );
	SceneObject* cube = graph.createObject("acube", "LCCritBody", cube_properties);

	PropertyContainer sphere_properties;
	sphere_properties.push_back( Property("filename", std::string("b0_02.obj")) );
	SceneObject* sphere = graph.createObject("asphere", "LCCritBody", sphere_properties);

	/* Attach these objects to our graph */
	graph.getNode("n2")->attach(cube);
	graph.getNode("n3")->attach(sphere);

	/* Create a distance query object and its arguments which we will
	 * reuse several times.
	 */
	/* Preform the query once. This should result in a positive collision. */
	QueryArguments args = graph.executeQuery("LCDistanceQuery");

	/* Now lets move the objects around via the graph are redo the collision query */
	translate(graph.getNode("n1"), Vector3(2.0,0.0,0.0));

	/* Instruct graph to update */
	graph.update();

	/* Do the collision query again. This time the objects should not be in collision */
	args = graph.executeQuery("LCDistanceQuery");
	//LOG4CXX_INFO(demo_logger, "Distance: " << args.distanceMap[0]);

	// Test any_cast problem
	SceneObject* obj = graph.getObject("acube");
	ObjectInfo cube_info;
	obj->getInfo(cube_info);
	printme(cube_info.parameters);
	dome(cube_info);

	return true;
}
