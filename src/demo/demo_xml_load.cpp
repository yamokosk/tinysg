#include "demowrapper.h"

void usage()
{
	std::cout
		<< "Usage:\n\tdemo_xml_load scene_description.xml\n";
}

void description()
{
	std::cout
		<< " --== Loading a scene from file demo with TinySG ==--\n\n"
		<< "\tLoads a scene specified on the command line. Then re-saves it to\n"
		<< "another filename.\n\n";
}

bool rundemo(int argc, char **argv)
{
	if ( argc < 2 )
	{
		usage();
		return false;
	}

	description();

	TinySG::Initialize();

	SceneGraph graph;

	std::stringstream stm;
	stm << argv[1];

	SceneLoader::load(stm.str().c_str(), graph);
	std::cout << "Loaded scene: " << stm.str() << std::endl;

	// Telling graph to update
	graph.update();
	std::cout << "Scene updated." << std::endl;

	stm << ".out";
	SceneLoader::save(stm.str().c_str(), graph);
	std::cout << "Saved scene to: " << stm.str() << std::endl;

	return true;
}
