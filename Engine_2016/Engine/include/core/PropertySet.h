// Copyright (C) 2015 Mikko Romppainen. See license.txt for license terms.
#ifndef CORE_PROPERTY_SET_H_
#define CORE_PROPERTY_SET_H_

#include <core/Object.h>
#include <core/Ref.h>
#include <core/Log.h>

#include <eastl/string.h>
#include <eastl/map.h>

namespace core
{

class PropertySet;

/**
 * Property is a class for accessing value of named property. Property 
 * has a name and value. Value can be accessed using get-methods or in 
 * case of array type of property, it can be accessed by []-operator.
 */
class Property : public Object
{
public:
    class PropertyValueBase : public Object
    {
    protected:
        PropertyValueBase()
        {
        }
    public:
        virtual ~PropertyValueBase()
        {
        }
    };

	template<class T> 
    class PropertyValue : public PropertyValueBase
	{
	public:
        PropertyValue(const T& v)
            : PropertyValueBase()
			, value(v)
        {
		}


        PropertyValue()
            : PropertyValueBase()
            , value()
        {
        }

        virtual ~PropertyValue()
        {
		}
		
		template<class T>
		void SetValue(T v)
		{
			value = v;
		}

        template<class T>
        const T getValue() const
		{
			return value;
		}

        template<class T>
		T getValue()
		{
			return value;
		}

        const T getValue() const
        {
            return value;
        }

        T getValue()
        {
            return value;
        }

        template<class T>
        void AddValue(T v)
        {
            value.push_back(v);
        }
    private:
        PropertyValue(const PropertyValue&);
        PropertyValue& operator=(const PropertyValue&);
	private:
		T value;
	};

public:
	typedef eastl::vector< Ref<PropertyValueBase> > PropertyValueArray;

    typedef PropertyValue<bool> BoolPropertyValue;
	typedef  PropertyValue<eastl::string> StringPropertyValue;
    typedef  PropertyValue<float> FloatPropertyValue;
    typedef  PropertyValue<int> IntPropertyValue;
    typedef  PropertyValue< PropertySet > ObjectPropertyValue;
    typedef  PropertyValue< PropertyValueArray > ArrayPropertyValue;


	/** Initializes property by name. */
	Property(const eastl::string& name);

	/** Copy constructor. Initializes property by key and value from other property. */
	Property(const Property& o);

	virtual ~Property();

	Property& operator=(const Property& o);
	Property& operator=(const char* value);
	Property& operator=(char* value);
	Property& operator=(int value) { return assign<int>(value); }
	Property& operator=(float value) { return assign<float>(value); }
	Property& operator=(const eastl::string& value) { return assign<eastl::string>(value); }
    Property& operator=(Property::PropertyValueBase* value) { m_property = value; return *this; }
	Property& operator=(const PropertySet& value)	{ return assign<PropertySet>(value); }
    Property& operator=(const PropertyValueArray& values)	{ return assign<PropertyValueArray>(values); }

	/** Returns true, if property is empty (property value is not set) */
	bool isVoid() const;
	
	/** Returns true, if property is property set, which contains child properties. */
    bool isChildPropertySet() const;
	
	/** Returns true, if property is type of the template argument. */
    template<class T>
	inline bool isTypeOf() const 
	{
	//	assert(this->m_property != 0);
        PropertyValue<T>* res = dynamic_cast< PropertyValue<T>* >( this->m_property.ptr() );
		return res != 0;
    }


	/** Returns value as type of the template argument. If property type is not correct, method asserts in debug mode. */
    template<class T>
	inline const T& get() const 
	{
        if(this->m_property == 0)
        {
            LOG_ERROR("Property %s does not exist", this->getName().c_str() );
            assert(0);
        }
        PropertyValue<T>* res = dynamic_cast< PropertyValue<T>* >(this->m_property.ptr());
        if(res == 0)
        {
            LOG_ERROR("Property %s is not correct type", this->getName().c_str() );
            assert(0);
        }

		return res->getValue<T&>();
	}

    template<class T>
    inline T& get()
    {
        if(this->m_property == 0)
        {
            LOG_ERROR("Property %s does not exist", this->getName().c_str() );
            assert(0);
        }
        PropertyValue<T>* res = dynamic_cast< PropertyValue<T>* >(this->m_property.ptr());
        if(res == 0)
        {
            LOG_ERROR("Property %s is not correct type", this->getName().c_str() );
            assert(0);
        }

        return res->getValue<T&>();
    }

    inline const PropertyValueBase& getValue() const
    {
        assert(this->m_property != 0);
       // PropertyValue<T>* res = dynamic_cast< PropertyValue<T>* >(this->m_property.ptr());
        //assert( res != 0 );
        //return res->getValue<T&>();
        return *this->m_property.ptr();
    }
	    
	inline PropertyValueBase* getValuePtr()
    {
        assert(this->m_property != 0);
       // PropertyValue<T>* res = dynamic_cast< PropertyValue<T>* >(this->m_property.ptr());
        //assert( res != 0 );
        //return res->getValue<T&>();
        return this->m_property.ptr();
    }

	const Property& operator[](int index) const;

	const Property& operator[](const char* const index) const;

	Property& operator[](int index);

	Property& operator[](const char* const index);

	bool operator==(const eastl::string& rhs) const;

	/** Returns the name of the property. */
	const eastl::string& getName() const
	{
		return m_name;
	}

    /**
     * @brief getUserData
     * Sets user data (Application data) to property. User data can be
     * for example ui widged information etc. User data is NOT saves to json during serialization.
     */
    void setUserData(Object* userData) { m_userData = userData; }

    /**
     * @brief getUserData
     * @return User data (Application data).
     */
    const Object* getUserData() const { return m_userData.ptr(); }

/*	bool hasAttributes() const;
	const PropertySet& attributes() const;
	PropertySet& attributes();
    void setAttributes(const PropertySet& attributes);*/
private:
	template<class T>
	Property& assign(const T& value)
	{
        this->m_property = new PropertyValue<T>(value);
		return *this;
	}
	
	const eastl::string getAsString() const;
    const bool getAsBool() const;
	const float getAsFloat() const;
	bool isFloat() const;

	eastl::string		m_name;
    Ref<Object>			m_userData;
    Ref<PropertyValueBase>			m_property;
//	Ref<PropertySet>	m_attributes;
	
};

template<>
inline bool Property::isTypeOf<float>() const 
{
	return isFloat();
}

template<>
inline const float& Property::get() const 
{
	static float v = 0.0f;
	v = getAsFloat();
	return v;
}

template<>
inline const eastl::string& Property::get() const
{
	static eastl::string v = "";
	v = getAsString();
	return v;
}

template<>
inline const bool& Property::get() const 
{
	static bool v = false;
	v = getAsBool();
	return v;
}

template<>
inline bool& Property::get() 
{
	static bool v = false;
	v = getAsBool();
	return v;
}

/**
 * Property set is a collection of properties. You can access and set different 
 * properties by name using []-operator. If you want to iterate whole property set, 
 * then you can use begin and end -methods for that. 
 */
class PropertySet : public Object
{
public:
	typedef eastl::vector< Property > PropertySetType;
	typedef PropertySetType::const_iterator const_iterator;
	typedef PropertySetType::iterator iterator;
	
	PropertySet();

	PropertySet(const PropertySet& o);

	PropertySet& operator=(const PropertySet& o);

	virtual ~PropertySet();
	
	static PropertySet createFromJson(const eastl::string& json);

	Property* insertNewProperty(const char* const name);

	Property& operator [](const char* const name);

	const Property& operator [](const char* const name) const;

	Property& operator [](const eastl::string& name);

	const Property& operator [](const eastl::string& name) const;

	Property& operator [](int index);

	const Property& operator [](int index) const;
	
	const_iterator begin() const;
	const_iterator end() const;
	
	iterator begin();
	iterator end();

	void setValues(const eastl::map< eastl::string, eastl::string >& v);

	// Get a numeric property (integer).
	int getNumericProperty(const eastl::string &name) const;

	// Get a literal property (string).
	eastl::string getLiteralProperty(const eastl::string &name) const;

	int size() const { return m_properties.size(); }

	bool hasProperty(const eastl::string& name) const;

	static PropertySet readFromFile(const eastl::string& filename);

	static void writeToFile(const PropertySet& properties, const eastl::string& filename);

    PropertySet clone() const;

	void updateProperties(const PropertySet& from);

	eastl::string toString() const;


private:
	PropertySetType		m_properties;

	const_iterator findByName(const eastl::string& name) const;
	iterator findByName(const eastl::string& name);

	const_iterator findByIndex(int n) const;
	iterator findByIndex(int n);
};

}

#endif

