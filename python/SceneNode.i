%{
#include <SceneNode.h>
%}

%include "SceneNode.h"
%extend tinysg::SceneNode 
{
	double* getPositionVector(tinysg::TransformSpace relativeTo) const
	{
		const Vector3& v = self->getPosition(relativeTo);
		double* ret = new double[3];
		ret[0] = v[0];
		ret[1] = v[1];
		ret[2] = v[2];
		return ret;	
	}
};