/*
 * Plugin.h
 *
 *  Created on: Nov 21, 2008
 *      Author: yamokosk
 */

#include <plugin_framework/Plugin.h>
namespace plugin = obrsp::plugin;

extern "C" int LinCanny_Plugin_ExitFunc();
extern "C" plugin::PF_ExitFunc LinCanny_Plugin_InitPlugin(const plugin::PF_PlatformServices * params);
