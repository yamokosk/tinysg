#include <api/Services.h>
#include <plugin_framework/PluginRegistrar.h> // NOTE: Only needed for static plugins

#include "HelloWorldObject.h" // The object this plugin provides

extern "C" int Plugin_ExitFunc()
{
	return 0;
}

extern "C" PF_ExitFunc Plugin_InitPlugin(const PF_PlatformServices* params)
{
	PF_RegisterParams rp;

	/* The PF_RegisterParams struct contains all the information this plugin
	 * needs to report back to the main program about itself. The first
	 * bit of information is the version of this plugin... Especially useful to
	 * prevent older plugins from being dynamically loaded and possibly
	 * crashing the program.
	 */
	rp.version.major = 1;
	rp.version.minor = 0;

	/* This plugin framework nominally allows you to define both C and C++
	 * language plugins. However, I have disabled the C language support.
	 * Someone who really knew what they were doing could even expand this
	 * language support to something like, *gasp*, Python!
	 */
	rp.programmingLanguage = PF_ProgrammingLanguage_CPP;

	/* Now we register the object this plugin is providing. In this simple
	 * example, we provide a "HelloWorldObject" which is derived from
	 * the main program's SceneObject class.
	 */
	rp.createFunc = HelloWorldObject::create;
	rp.destroyFunc = HelloWorldObject::destroy;

	/* Finally, call back to the main program to register our new object.
	 */
	int status = params->registerObject("HelloWorld", &rp);

	if (status < 0) {
		// Oh no! An error occurred. Do something about it!
		return NULL;
	}

	return Plugin_ExitFunc;
}

PluginRegistrar Plugin_registrar(PluginRegistrar); // NOTE: Only needed for static plugins
