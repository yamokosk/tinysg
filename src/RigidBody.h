#ifndef _BODYNODE_H_
#define _BODYNODE_H_

#include <SceneNode.h>

namespace tinysg
{

class RigidBody : public SceneNode
{
	friend class SceneGraph;

#if defined( TSG_HAVE_LOG4CXX )
	static log4cxx::LoggerPtr logger;
#endif

public:
	RigidBody(const std::string& name);
	~RigidBody();

	virtual void accept(Visitor* visitor);

private:
	float mMass;
	Vector3 mPosCenterOfMass;
};

}  // namespace tinysg

#endif
