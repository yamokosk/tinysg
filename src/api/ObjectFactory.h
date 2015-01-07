/*************************************************************************
 * SceneML, Copyright (C) 2007, 2008  J.D. Yamokoski
 * All rights reserved.
 * Email: yamokosk at gmail dot com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the License,
 * or (at your option) any later version. The text of the GNU Lesser General
 * Public License is included with this library in the file LICENSE.TXT.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the file LICENSE.TXT for
 * more details.
 *
 *************************************************************************/
/*
 * ObjectFactory.h
 *
 *  Created on: Dec 10, 2008
 *      Author: yamokosk
 */

#ifndef OBJECTFACTORY_H_
#define OBJECTFACTORY_H_

#include <string>
#include <sstream>
#include <plugin_framework/PluginManager.h>

#include "ObjectModel.h"

namespace tinysg {

/*struct ObjectFactory
{
	static SceneObject* create(const ObjectInfo& info)
	{
		void* ptr = PluginManager::getInstance().createObject(info.type);
		if ( ptr == NULL )
		{
			return NULL;
		}
		SceneObject* object = static_cast<SceneObject*>(ptr);
		object->init(info);
		return object;
	}
};*/

}  // namespace tinysg
#endif /* OBJECTFACTORY_H_ */
