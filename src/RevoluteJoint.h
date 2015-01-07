#ifndef _REVOLUTE_JOINT_H_
#define _REVOLUTE_JOINT_H_

#include <SceneNode.h>
#include <Visitor.h>

namespace tinysg
{

class RevoluteJoint : public SceneNode
{
	friend class SceneGraph;

#if defined( TSG_HAVE_LOG4CXX )
	static log4cxx::LoggerPtr logger;
#endif

public:
	RevoluteJoint(const std::string& name);
	virtual ~RevoluteJoint() {};

	virtual void accept(Visitor* visitor);

private:
	float mPosition;
	float mSpeed;
	float mAcceleration;
	float mTorque;
};

}  // namespace tinysg

#endif
