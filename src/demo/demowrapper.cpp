/*
 * demowrapper.cpp
 *
 *  Created on: Aug 11, 2008
 *      Author: yamokosk
 */
#include "demowrapper.h"

#if defined(TSG_HAVE_LOG4CXX)
	LoggerPtr demo_logger(Logger::getLogger("Demo"));
#endif

#ifdef __APPLE__
#	include <stdlib.h>

	int atexit(void (*func)(void));

	void persistent()
	{
		std::cout << "Press any key to continue ...";
		std::string z;
		getline(std::cin,z);
	}
#endif


void createSimpleGraph(SceneGraph& graph)
{
	/* Create the graph */
	graph.getNode(SceneGraph::World)->createChild("n1");
	graph.getNode(SceneGraph::World)->createChild("n2");
	graph.getNode("n1")->createChild("n3");
	graph.getNode("n1")->createChild("n4");

	/* Rotate and translate these nodes */
	translate(graph.getNode("n1"), Vector3(1.0, 1.0, 0.0));
	translate(graph.getNode("n2"), Vector3(2.0, 1.0, 0.0));
	translate(graph.getNode("n3"), Vector3(2.0, 0.0, 0.0));

	/* Instruct graph to update all nodes since we have moved them all around */
	graph.update();
}

int main( int argc, char **argv)
{
#if defined(TSG_HAVE_LOG4CXX)
	// Set up a simple configuration that logs on the console.
	/*if (argc > 1)
	{
		// BasicConfigurator replaced with PropertyConfigurator.
		PropertyConfigurator::configure(argv[1]);
	}
	else
	{*/
		BasicConfigurator::configure();
	//}
#endif

#ifdef __APPLE__
	/*
	 * author yamokosk
	 *
	 * Hack to get some memory leak detection on OS X. This call will prevent the program
	 * from terminating, thus allowing us to run a program like leaks to look for memory leaks.
	 */
	atexit(persistent);
#endif

	bool wasSucessful = rundemo(argc, argv);
	if ( !wasSucessful )
	{
		std::cout << "Demo application failed!" << std::endl;
	} else {
		std::cout << "Demo completed successfully." << std::endl;
	}

	return !wasSucessful;
}
