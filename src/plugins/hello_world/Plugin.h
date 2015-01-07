/*
 * TinySG plugin example.
 *
 * This "Hello-World" example show how to create a very simply plugin for
 * TinySG. Plugins for TinySG can either be static (compiled with the main
 * library) or dynamic (compiled separately and loaded at run-time). The
 * difference in implementation between static and dynmaic plugins is noted
 * below.
 *
 * All plugins (regardless whether they are static or dynamic) MUST implement
 * two functions:
 *
 *  PF_ExitFunc Plugin_InitPlugin(const PF_PlatformServices* params);
 * 	int Plugin_ExitFunc();
 *
 * The purpose of these functions should be self-explanatory. Plugin_InitPlugin
 * gets called by when the plugin is first loaded. It should register what
 * objects it will provide to the main program. Plugin_ExitFunc is called when
 * the plugin is unloaded so it can clean up and globally allocated data or
 * execute some other clean-up tasks. More details for each function are
 * provided below.
 *
 * This example plugin will register one 'HelloWorldObject' which is derived
 * from the main libraries SceneObject interface.
 */
#include <plugin_framework/Plugin.h>

/**
 * Called when plugin is unloaded.
 * @return 0 on success or negative value on failure.
 */
extern "C" int Plugin_ExitFunc();

/**
 * Called when plugin is loaded.
 * @param params Services structure provided by the main program.
 * @return Function pointer to the Plugin_ExitFunc above.
 */
extern "C" PF_ExitFunc Plugin_InitPlugin(const PF_PlatformServices * params);
