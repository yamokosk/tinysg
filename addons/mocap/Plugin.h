/*
 * Plugin.h
 *
 *  Created on: Nov 21, 2008
 *      Author: yamokosk
 */

#include <plugin_framework/Plugin.h>

extern "C" int MoCap_Plugin_ExitFunc();
extern "C" PF_ExitFunc MoCap_Plugin_InitPlugin(const PF_PlatformServices * params);
