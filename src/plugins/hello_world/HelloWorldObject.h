#ifndef HELLOWORLDOBJECT_H
#define HELLOWORLDOBJECT_H

#include <api/ObjectModel.h>

/**
 * Simple HelloWorld type object.
 *
 * This is the meat of this plugin example. The class below inherits from the
 * SceneObject interface class defined in ObjectModel.h. ALL scene objects
 * in TinySG must inherit from this interface. The interface class by definition
 * is pure virtual. Therefore this class must implement the methods it
 * inherits.. including init(), getProperty(), setProperty(), getInfo(), and
 * notifyMoved(). Implementation of these functions is necessary for all
 * scene objects registered with TinySG.
 *
 * The other two functions this class must implement are necessary for the
 * creation and destruction of these objects. The create() and destroy() methods
 * are called, not by TinySG directly, but rather by the PluginManager for
 * TinySG.
 */
class HelloWorldObject : public SceneObject
{
public:
	virtual ~HelloWorldObject();

	// Required static plugin interface functions
	/** Create an instance of this class. A static function of this class which
	 * creates an instance of a HelloWorldObject.
	 * @param params PF_ObjectParams stores both the type of object to create
	 * (in case there is more than one type) as well as a pointer to the
	 * services provided by the main program.
	 * @return A void pointer the newly created HelloWorldObject
	 */
	static void* create(PF_ObjectParams *params);
	static int destroy(void *);

	// The following are inherited from the SceneObject interface class
	void init(const ObjectInfo& info);
	Property getProperty(const std::string& name) const;
	void setProperty(const Property& p);
	virtual void getInfo(ObjectInfo& info) const;
	virtual void notifyMoved( const Real* translation, const Real* rotation );

private:
	/* Notice that the constructor is private. These objects should only be
	 * created by the static create() method above.*/
	HelloWorldObject();
};

#endif /* GEOMETRY_H_ */
