#include "FwdKinVisitor.h"
#include "RigidBody.h"
#include "RevoluteJoint.h"

void FwdKinVisitor::visit(SceneNode* node)
{

}

void FwdKinVisitor::visit(RigidBody* body)
{
	mLastVisited = RIGID_BODY;
}

void FwdKinVisitor::visit(RevoluteJoint* joint)
{
	mLastVisited = REVOLUTE_JOINT;
}
