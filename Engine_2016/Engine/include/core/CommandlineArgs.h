#ifndef COMMANDLINE_ARGS_H_
#define COMMANDLINE_ARGS_H_



#include <core/PropertySet.h>
#include <core/log.h>

namespace core
{





/**
 * Builds property set from commangline args.
 */
class CommandlineArgs : public PropertySet
{
public:	
	CommandlineArgs(int argc, char* argv[], const eastl::string& separatorString = eastl::string("") )
		: PropertySet()
	{
		for( int i=0; i<argc; ++i )
		{
			eastl::string name = argv[i];
			
			if( separatorString.length() == 0 || separatorString.find_first_of(name[0]) != eastl::string::npos )
			{
				// Found separator, handle all args for this property
				PropertySet& props = *this;
				name = name.substr(1,name.length()-1);
				props[name];
				++i;
				int numValues=0;
				for(; (i+numValues)<argc; ++numValues )
				{
					eastl::string value = argv[i+numValues];
					if( separatorString.find_first_of(value[0]) != eastl::string::npos )
					{
						break;
					}
				}
				
				// Add to props[name]
				if( numValues > 0 )
				{
					Property& prop = props[name];
					if( numValues > 1 || (!prop.isVoid() && prop.isChildPropertySet()) )
					{
						for(int j=0; j<numValues; ++j )
						{
							prop[j] = argv[i+j];
						}
					}
					else
					{
						prop = argv[i];
					}
				}

				i+=numValues-1;
			}
			else
			{
				LOG_ERROR("Unexpected commandline argument: \'%s\'", name.c_str());
			}
		}
	}

	virtual ~CommandlineArgs()
	{
	}
	
private:
};

}

#endif

