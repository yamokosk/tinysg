/*************************************************************************
 * TinySG, Copyright (C) 2007, 2008  J.D. Yamokoski
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
 * SceneObjectContainer.h
 *
 *  Created on: Dec 1, 2008
 *      Author: yamokosk
 */

#ifndef SCENEOBJECTCONTAINER_H_
#define SCENEOBJECTCONTAINER_H_

#include <vector>

namespace tinysg {

template <class T>
class VectorIterator
{
private:
	typename T::iterator mCurrent;
	typename T::iterator mEnd;
	/// Private constructor since only the parameterised constructor should be used
	VectorIterator() {};
public:
	typedef typename T::value_type ValueType;

	/** Constructor.
	@remarks
		Provide a start and end iterator to initialise.
	*/
	VectorIterator(typename T::iterator start, typename T::iterator end)
		: mCurrent(start), mEnd(end)
	{
	}

	/** Constructor.
	@remarks
		Provide a container to initialise.
	*/
	explicit VectorIterator(T& c)
		: mCurrent(c.begin()), mEnd(c.end())
	{
	}

	/** Returns true if there are more items in the collection. */
	bool hasMoreElements(void) const
	{
		return mCurrent != mEnd;
	}

	/** Returns the next element in the collection, and advances to the next. */
	typename T::value_type getNext(void)
	{
		return *mCurrent++;
	}
	/** Returns the next element in the collection, without advancing to the next. */
	typename T::value_type peekNext(void)
	{
		return *mCurrent;
	}
	/** Returns a pointer to the next element in the collection, without advancing to the next afterwards. */
	typename T::pointer peekNextPtr(void)
	{
		return &(*mCurrent);
	}
	/** Moves the iterator on one element. */
	void moveNext(void)
	{
		++mCurrent;
	}
};

template <class T>
class MapIterator
{
private:
	typename T::iterator mCurrent;
	typename T::iterator mEnd;
	/// Private constructor since only the parameterised constructor should be used
	MapIterator() {};
public:
	typedef typename T::mapped_type MappedType;
	typedef typename T::key_type KeyType;

	/** Constructor.
	@remarks
		Provide a start and end iterator to initialise.
	*/
	MapIterator(typename T::iterator start, typename T::iterator end)
		: mCurrent(start), mEnd(end)
	{
	}

	/** Constructor.
	@remarks
		Provide a container to initialise.
	*/
	explicit MapIterator(T& c)
		: mCurrent(c.begin()), mEnd(c.end())
	{
	}

	/** Returns true if there are more items in the collection. */
	bool hasMoreElements(void) const
	{
		return mCurrent != mEnd;
	}

	/** Returns the next value element in the collection, and advances to the next. */
	typename T::mapped_type getNext(void)
	{
		return (mCurrent++)->second;
	}
	/** Returns the next value element in the collection, without advancing to the next. */
	typename T::mapped_type peekNextValue(void)
	{
		return mCurrent->second;
	}
	/** Returns the next key element in the collection, without advancing to the next. */
	typename T::key_type peekNextKey(void)
	{
		return mCurrent->first;
	}
	/** Required to overcome intermittent bug */
	MapIterator<T> & operator=( MapIterator<T> &rhs )
	{
		mCurrent = rhs.mCurrent;
		mEnd = rhs.mEnd;
		return *this;
	}
	/** Returns a pointer to the next value element in the collection, without
		advancing to the next afterwards. */
	typename T::mapped_type* peekNextValuePtr(void)
	{
		return &(mCurrent->second);
	}
	/** Moves the iterator on one element. */
	void moveNext(void)
	{
		++mCurrent;
	}
};


}  // namespace tinysg


#endif /* PROPERTYCONTAINER_H_ */
