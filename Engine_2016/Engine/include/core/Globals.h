#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <core/Singleton.h>
#include <core/PropertySet.h>

namespace core
{
	class Globals : public PropertySet, public Singleton<Globals>
	{
	public:
		Globals()
		{
		}
	};

}


#endif

