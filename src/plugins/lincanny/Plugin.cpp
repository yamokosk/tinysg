#include <api/Services.h>
#include <plugin_framework/PluginRegistrar.h>

#include "BodyAdapter.h"
#include "DistanceQuery.h"

extern "C" int LinCanny_Plugin_ExitFunc()
{
	return 0;
}

extern "C" plugin::PF_ExitFunc LinCanny_Plugin_InitPlugin(const plugin::PF_PlatformServices * params)
{
	LOG_MESSAGE(params, "lincanny_plugin loading");

	int status = 0;

	plugin::PF_RegisterParams rp;
	rp.version.major = 1;
	rp.version.minor = 0;
	rp.programmingLanguage = plugin::PF_ProgrammingLanguage_CPP;

	// Register lincanny objects
	rp.createFunc = BodyAdapter::create;
	rp.destroyFunc = BodyAdapter::destroy;
	status = params->registerObject("LCCritBody", &rp);

	rp.createFunc = BodyAdapter::create;
	rp.destroyFunc = BodyAdapter::destroy;
	status = params->registerObject("LCBody", &rp);

	REGISTER_CPP_CLASS( params, rp, DistanceQuery, status );

	if (status < 0)
	{
		LOG_ERROR(params, "A problem occurred during initialization of the lincanny_plugin.");
		return NULL;
	}
	else
	{
		LOG_MESSAGE(params, "lincanny_plugin registration completed successfully.");
	}

	return LinCanny_Plugin_ExitFunc;
}

plugin::PluginRegistrar LinCanny_Plugin_registrar(plugin::PluginRegistrar);
