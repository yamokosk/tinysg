#ifndef _VISITOR_H_
#define _VISITOR_H_

namespace tinysg {

// Forward declare node types
class SceneNode;
class RigidBody;
class RevoluteJoint;


class Visitor
{
public:
	virtual void visit(SceneNode* node) = 0;
	virtual void visit(RigidBody* body) = 0;
	virtual void visit(RevoluteJoint* joint) = 0;
};

}  // namespace tinysg

#endif
