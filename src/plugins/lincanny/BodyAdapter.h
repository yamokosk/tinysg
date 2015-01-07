/*
 * BodyWrapper.h
 *
 *  Created on: Dec 5, 2008
 *      Author: yamokosk
 */

#ifndef BODYWRAPPER_H_
#define BODYWRAPPER_H_

#include <api/ObjectModel.h>
#include <linalg/Vector3.h>
using namespace obrsp::linalg;

#include <plugin_framework/Plugin.h>
namespace plugin = obrsp::plugin;

#include "algorithm/BoundingMesh.h"

class BodyAdapter : public tinysg::SceneObject
{
	enum BodyType
	{
		Critical,
		NonCritical
	};

public:
	// static plugin interface
	static void* create(plugin::PF_ObjectParams *);
	static int destroy(void *);
	static const std::string Type;
	~BodyAdapter();

	// SceneObject methods
	virtual void init(const tinysg::ObjectInfo& info);
	virtual tinysg::Property getProperty(const std::string& name) const;
	virtual void setProperty(const tinysg::Property& p);
	virtual void getInfo(tinysg::ObjectInfo& info) const;
	virtual void notifyMoved( const float* translation, const float* rotation );

	const plugin::PF_PlatformServices* services;

private:
	BodyAdapter();

	Vector3 getPosition() const;
	Quaternion getOrientation() const;

	int bodyID;
	BodyType bodyType;
	//RigidBody* bodyptr;
	std::string name;

	std::vector<float> alphas;
	std::vector<Vector3> colors;
	std::vector<BoundingMesh*> meshes;
};

#endif /* BODYWRAPPER_H_ */
