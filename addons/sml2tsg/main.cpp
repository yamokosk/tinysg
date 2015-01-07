/*
 * main.cpp
 *
 *  Created on: Oct 23, 2009
 *      Author: yamokosk
 */

#include <iostream>

#include "xml_scene_builder.h"
#include <TinySG.h>

int main(int argc, char* argv[])
{
	if ( argc < 3 )
	{
		std::cout << "Usage: sml2tsg [SceneML_Filename.scene] [Out_Filename.scene]" << std::endl;
		return -1;
	}

	tinysg::TinySG::Initialize();
	tinysg::SceneGraph graph;

	sceneml::SceneDirector sceneDirector;
	sceneml::XMLSceneBuilder xmlSceneBuilder(argv[1]);
	sceneDirector.SetSceneBuilder( &xmlSceneBuilder );
	sceneDirector.ConstructScene(graph);

	std::cout << "SceneML file " << argv[1] << " parsed successfully." << std::endl;
	tinysg::SceneLoader::save(argv[2], graph);

	std::cout << "It worked!" << std::endl;
	return 0;
}
