#include "demowrapper.h"

#ifndef TSG_HAVE_ODE
#error demo_ode requires the ODE library to be installed.
#endif

void description()
{
	std::cout
		<< " --== Collision query demo with ODE & TinySG ==--\n\n"
		<< "\tCreates a simple 4 node (n1,n2,n3, and n4) scene. Two ODE spaces\n"
		<< "are created. Then an ODE box and sphere are created and attached to\n"
		<< "nodes n2 and n3 respectively. The positions of nodes n2 and n3 are set\n"
		<< "such that the ODE objects are in collision. Then node n1 (parent of\n"
		<< "node n3) is translated so that the ODE objects should no longer be\n"
		<< "in collision.\n\n";
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
	graph.getNode("n2")->attach(box);
	graph.getNode("n3")->attach(sphere);

	/* Create an ODE collision query object and its arguments which we will
	 * reuse several times.
	 */
	QueryArguments args;
	args.parameters.push_back(
		Property("CollisionPair", SceneObjectPair(lspace, rspace)) );
	graph.executeQuery("ODECollisionQuery", args);

	/* Peform the query once. This should result in a positive collision. */
	VERIFY( args.objectsInCollision.size() > 0 );

	/* Now lets move the objects around via the graph are redo the collision
	 * query
	 */
	translate(graph.getNode("n1"), Vector3(2.0,0.0,0.0));

	/* Instruct graph to update */
	graph.update();

	/* .. and verify that our node has indeed moved */
	VERIFY( graph.getNode("n3")->getPosition( tinysg::TS_WORLD )
			== Vector3(5.0, 1.0, 0.0) );

	/* Do the collision query again. This time the objects should not be in
	 * collision
	 */
	args.resetResults();
	graph.executeQuery("ODECollisionQuery", args);
	VERIFY( args.objectsInCollision.size() == 0 );

	return true;
}
