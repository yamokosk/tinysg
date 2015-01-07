#include <plugin_framework/Plugin.h>
#include "HelloWorldObject.h"

void*
HelloWorldObject::create(PF_ObjectParams* params)
{
	HelloWorldObject* ptr = new HelloWorldObject();
	return HelloWorldObject;
}

int
HelloWorldObject::destroy(void *p)
{
	if (!p) return -1;

	delete static_cast<Box*>(p);

	return 0;
}

HelloWorldObject::HelloWorldObject()
{
}

HelloWorldObject::~HelloWorldObject()
{

}

void
HelloWorldObject::init(const ObjectInfo& info)
{

}

Property
HelloWorldObject::getProperty(const std::string& name) const
{

}

void
HelloWorldObject::setProperty(const Property& p)
{

}

void
HelloWorldObject::getInfo(ObjectInfo& info) const
{

}

void
HelloWorldObject::notifyMoved( const Real* translation, const Real* rotation )
{

}
