#include <api/Services.h>
#include <plugin_framework/PluginRegistrar.h>

// ODE library
#include <ode/ode.h>

#include "Sphere.h"
#include "Box.h"
#include "Plane.h"
#include "CappedCylinder.h"
#include "Cylinder.h"
#include "TriangleMesh.h"
#include "SimpleSpace.h"
#include "CollisionQuery.h"

extern "C" int ODE_Plugin_ExitFunc()
{
	dCloseODE();
	return 0;
}

extern "C" PF_ExitFunc ODE_Plugin_InitPlugin(const PF_PlatformServices* params)
{
	LOG_MESSAGE(params, "ode_plugin loading");

	// ODE specific initialization
	dInitODE();

	int status = 0;

	PF_RegisterParams rp;
	rp.version.major = 1;
	rp.version.minor = 0;
	rp.programmingLanguage = PF_ProgrammingLanguage_CPP;

	// Register ode objects
	REGISTER_CPP_CLASS( params, rp, Sphere, status );
	REGISTER_CPP_CLASS( params, rp, Box, status );
	REGISTER_CPP_CLASS( params, rp, Plane, status );
	REGISTER_CPP_CLASS( params, rp, CappedCylinder, status );
	REGISTER_CPP_CLASS( params, rp, Cylinder, status );
	REGISTER_CPP_CLASS( params, rp, TriangleMesh, status );
	REGISTER_CPP_CLASS( params, rp, SimpleSpace, status );
	REGISTER_CPP_CLASS( params, rp, CollisionQuery, status );

	if (status < 0) {
		LOG_ERROR(params, "A problem occurred during initialization of the ode_plugin.");
		return NULL;
	} else {
		LOG_MESSAGE(params, "ode_plugin registration completed successfully.");
	}

	return ODE_Plugin_ExitFunc;
}

PluginRegistrar ODE_Plugin_registrar(PluginRegistrar);
