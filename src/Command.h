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
 * Command.h
 *
 *  Created on: Jul 18, 2009
 *      Author: yamokosk
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <boost/function.hpp>

namespace tinysg
{

template<class T>
class Command
{
private:
	T* mObjectPtr; // the object pointer
	boost::function<void (T*)> mObjectMethod;

public:
	explicit Command(T* ptr, boost::function<void (T*)> pmethod) :
		mObjectPtr(ptr),
		mObjectMethod(pmethod)
	{ }

	void operator()()
	{
		execute();
	}
	//The execute method that is invoked by the client
	void execute()
	{
		// Calls the method of the object
		mObjectMethod(mObjectPtr);
	}
	~Command(){}; //Note do not call 'delete m_objptr' as we do not own this
};

}

#endif /* COMMAND_H_ */
