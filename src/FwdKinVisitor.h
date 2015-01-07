#ifndef _FWDKINVISITOR_H_
#define _FWDKINVISITOR_H_

#include <Visitor.h>

namespace tinysg
{

class FwdKinVisitor
{
	enum LastVisited
	{
		ROOT_NODE,
		RIGID_BODY,
		REVOLUTE_JOINT,
		PRISMATIC_JOINT
	};

public:
	virtual void visit(SceneNode* node);
	virtual void visit(RigidBody* body);
	virtual void visit(RevoluteJoint* joint);

private:
	Vector3 dim1; // Vector from Frame i-1 to Joint i
	Matrix3 Rim1; // Rotation matrix from Frame i-1 to Joint i
	LastVisited mLastVisited;
};

}  // namespace tinysg

#endif
