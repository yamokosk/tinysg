#include "demowrapper.h"

#include <boost/timer.hpp>


void description()
{
	std::cout
		<< " --== Saving a scene to file demo with TinySG ==--\n\n"
		<< "\tSaves the scene created in demo_ode to a file named test.xml.\n\n";
}

/*
 * Recursive function for creating node children. This blows up REALLY quickly,
 * so don't set the level to high.
 */
void createChildren(SceneNode* node, int level)
{
	if (level > 0)
	{
		SceneNode* n1 = node->createChild();
		if ( n1 == NULL )
		{
			std::cout << "Oh no. n1 wasn't created properly." << std::endl;
			return;
		} else {
			//std::cout << n1->getName() << " created properly." << std::endl;
			translate(n1, Vector3(1.0, 1.0, 0.0));
		}

		//std::cout << "Trying to create second node." << std::endl;
		SceneNode* n2 = node->createChild();
		if ( n2 == NULL )
		{
			std::cout << "Oh no. n2 wasn't created properly." << std::endl;
		} else {
			translate(n2, Vector3(0.0, 1.0, 1.0));
		}

		createChildren(n1, level-1);
		createChildren(n2, level-1);
	}
}

void createBigGraph(SceneGraph& graph)
{
	/* Create the graph */
	const int num_levels = 14;

	SceneNode* ptr = graph.getNode(SceneGraph::World)->createChild("n1");

	// Recursively create 4096 nodes.
	createChildren(ptr, num_levels);
}

bool rundemo(int argc, char **argv)
{
	description();

	log4cxx::LoggerPtr  root_logger( log4cxx::Logger::getRootLogger() );
	root_logger->setLevel(log4cxx::Level::getInfo());

	TinySG::Initialize();

	// Create a timer to see if there is a speed up
	boost::timer stopwatch;

	/* Create the big scene graph */
	stopwatch.restart();
	SceneGraph graph;
	createBigGraph(graph);
	double stop = stopwatch.elapsed();
	std::cout << "Created big graph with 4096 nodes in " << stop << " seconds." << std::endl;

	{
		// move main node - worst case scenario
		SceneNode* n1 = graph.getNode("n1");
		translate(n1, Vector3(1.0, 1.0, 1.0));

		/* Tell it to update */
		stopwatch.restart();
		graph.update();
		stop = stopwatch.elapsed();
		std::cout << "Graph updated (using threads) in " << stop << " seconds." << std::endl;
	}

	{
		// move main node - worst case scenario
		SceneNode* n1 = graph.getNode("n1");
		translate(n1, Vector3(1.0, 1.0, 1.0));

		/* Tell it to update */
		stopwatch.restart();
		graph.__deprecated_update();
		stop = stopwatch.elapsed();
		std::cout << "Graph updated (old method) in " << stop << " seconds." << std::endl;
	}
	return true;
}

