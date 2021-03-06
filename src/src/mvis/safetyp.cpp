/****************************************************************************
*
*					 MegaVision Application Framework
*
*	   A C++ GUI Toolkit for the SciTech Multi-platform Graphics Library
*
*  ========================================================================
*
*    The contents of this file are subject to the SciTech MGL Public
*    License Version 1.0 (the "License"); you may not use this file
*    except in compliance with the License. You may obtain a copy of
*    the License at http://www.scitechsoft.com/mgl-license.txt
*
*    Software distributed under the License is distributed on an
*    "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
*    implied. See the License for the specific language governing
*    rights and limitations under the License.
*
*    The Original Code is Copyright (C) 1991-1998 SciTech Software, Inc.
*
*    The Initial Developer of the Original Code is SciTech Software, Inc.
*    All Rights Reserved.
*
*  ========================================================================
*
* Language:		C++ 3.0
* Environment:	Any
*
* Description:	Module to overload the global operator new for MegaVision.
*               This provides a simple saftey pool mechanism for allocating
*				main memory.
*
****************************************************************************/

#include "mvis/mvision.hpp"
#include "mvis/mfontmgr.hpp"
#include <iostream.h>
#include <stdlib.h>

// These are not necessary in protected mode anymore

#ifndef	REALMODE
//---------------------------------------------------------------------------
// Class to manage the safety pool. A single global instantiation of this
// class is provided to automatically initialise the safety pool mechanism
// at program inception.
//---------------------------------------------------------------------------

class MVSafetyPool {
protected:
	void	*safetyPool;			// Pointer to allocated safety pool

public:
			// Constructor
			MVSafetyPool()	{ resize(); };

			// Destructor
			~MVSafetyPool()	{ resize(0); };

			// Method to re-size the safety pool
			void resize(size_t size = 4096);

			// Method to determine if safety pool is exhausted
			ibool exhausted()
				{ return safetyPool == NULL; };
	};

/*----------------------------- Implementation ----------------------------*/

static MVSafetyPool	_MV_safetyPool;
static ibool		_MV_majorConsumer = false;

void MVSafetyPool::resize(size_t size)
/****************************************************************************
*
* Function:		MVSafetyPool::resize
* Parameters:	size	- New size for the safety pool.
*
****************************************************************************/
{
	if (safetyPool != NULL)
		free(safetyPool);
	safetyPool = (size == 0) ? NULL : malloc(size);
}

void *operator new(size_t size)
/****************************************************************************
*
* Function:		operator new
* Parameters:	size	- Size of block to allocate
* Returns:		Pointer to the allocated block
*
* Description:	Overloaded memory allocation routine. We attempt to allocate
*				the memory initially by calling malloc. If this fails, we
*				free the safety pool and try again. If this also fails
*				we bomb out with an error message.
*
****************************************************************************/
{
	void *temp = malloc(size);

	// Only dip into the safety pool if the current memory allocation is
	// _not_ by a major consumer. Major consumer's will always allocate
	// more than the safety pool, and require the result of the operator
	// new to return NULL if the memory allocation fails.

	if (!_MV_majorConsumer) {
		while (temp == NULL) {
			if (_MV_safetyPool.exhausted())
				MGL_fatalError("Out of memory - saftey pool exhausted!");

			// First try purging the least recently used font until we
			// have enough memory, then dip into the safety pool
			if (MV_fontManager.purgeLRUFont() == -1) {
				// Resize the safety pool and try again

				_MV_safetyPool.resize(0);
				temp = malloc(size);
				}
			}
		}

	return temp;
}

void operator delete(void *p)
/****************************************************************************
*
* Function:		operator delete
* Parameters:	p	- Pointer to block to delete
*
* Description:	Overloaded memory deletion routine. We free the memory for
*				the block, and attempt to reallocate the safety pool if
*				it was freed.
*
****************************************************************************/
{
	free(p);
	if (_MV_safetyPool.exhausted())
		_MV_safetyPool.resize();
}
#endif

void MV_majorConsumer()
/****************************************************************************
*
* Function:		MV_majorConsumer
*
* Description:	Sets the major consumer flag to true, so memory allocations
*				will not dip into the safety pool but return NULL on
*				failure.
*
****************************************************************************/
{
#ifndef	REALMODE
	_MV_majorConsumer = true;
#endif
}

void MV_normalConsumer()
/****************************************************************************
*
* Function:		MV_normalConsumer
*
* Description:	Resets the major consumer flag to normal.
*
****************************************************************************/
{
#ifndef	REALMODE
	_MV_majorConsumer = false;
#endif
}

ibool MV_lowMemory()
/****************************************************************************
*
* Function:		MV_lowMemory
*
* Description:	Returns true if memory allocation reached into the safety
*				pool.
*
****************************************************************************/
{
#ifndef	REALMODE
	return _MV_safetyPool.exhausted();
#else
	return false;
#endif
}
