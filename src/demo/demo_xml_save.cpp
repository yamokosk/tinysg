#include "demowrapper.h"

void description()
{
	std::cout
		<< " --== Saving a scene to file demo with TinySG ==--\n\n"
		<< "\tSaves the scene created in demo_ode to a file named test.xml.\n\n";
}

bool rundemo(int argc, char **argv)
{
	description();

	TinySG::Initialize();

	/* Create the simple scene */
	SceneGraph graph;
	createSimpleGraph(graph);

	/* Create some ODE spaces */
	SceneObject* lspace = graph.createObject("lspace", "ODESimpleSpace");
	SceneObject* rspace = graph.createObject("rspace", "ODESimpleSpace");

	/* Create two ODE objects - a box and a sphere */
	PropertyContainer box_properties;
	box_properties.push_back( Property("lengths", Vector3(1.0, 2.0, 3.0)) );
	box_properties.push_back( Property("space", lspace) );
	SceneObject* box = graph.createObject("abox", "ODEBox", box_properties);

	PropertyContainer sphere_properties;
	sphere_properties.push_back( Property("radius", float(2.0)) );
	sphere_properties.push_back( Property("space", rspace) );
	SceneObject* sphere = graph.createObject("asphere", "ODESphere", sphere_properties);

	/* Attach these objects to our graph */
	graph.getNode(SceneGraph::World)->attach(lspace);
	graph.getNode(SceneGraph::World)->attach(rspace);
	graph.getNode("n2")->attach(box);
	graph.getNode("n3")->attach(sphere);

	SceneLoader::save("test.xml", graph);

	return true;
}

