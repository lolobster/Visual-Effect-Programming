#ifndef ENTITY_H_
#define ENTITY_H_

#include <eastl/vector.h>
#include <eastl/string.h>
#include <core/Ref.h>
#include <core/Object.h>

#include <slmath/mat4.h>
#include <slmath/quat.h>

namespace scene
{

	class Entity;

/** 
 * Class for Component.
 */
class Component : public core::Object
{
public:
	/** Default constructor */
	Component( );

	/** Destructor */
	virtual ~Component( );	
	
	/** 
	 * This abstract method is called, when component is set to its owner Entity and 
	 * this component should be active.
	 */
	virtual void construct() = 0;

	/**
	 * This abstract method is called, when component is removed from its owner Entity and
	 * this component should be disabled.
	 */
	virtual void destroy() = 0;

	void setName( const eastl::string& name );

	/** Returns owner entity of this component. */
	Entity* getOwner() const;

	/** Returns name of this component. */
	const eastl::string getName() const;

private:
	eastl::string	m_name;
	Entity*			m_owner;

	// Hidden copy constructor and assigment operator.
	Component( const Component& );
	Component& operator=( const Component& );

	friend class Entity;
	
};


/** 
 * Class for game entity. Game entity contains several components, which specify
 * actual game entity functionality.
 */
class Entity : public core::Object
{
public:
	/** Default constructor */
	Entity( const eastl::string& name );

	/** Destructor */
	virtual ~Entity( );	
	
	/** Adds new component to game entity. Sets owner of the component and constructs component. */
	void addComponent( Component* newComponent );
	
	/** Adds new child to game entity and set childs parent. */
	void addChild( Entity* child );

	/** Reteurns parent of this Entity*/
	Entity* getParent() const;
	
	/** Returns name of this entity */
	const eastl::string getName() const;

	/** Gets child by name */
	Entity* getChildByName(const eastl::string& name) const;

	/** Finds child by name also from child objects */
	Entity* find(const eastl::string& name) const;

	/**
	 * Returns component by type. Method will assert, if several component with same type 
     * is found or component for given type does not exist. Uses dynamic_cast.
	 */
	template<class T>
	T* getComponent() const
	{
		T* res = 0;
		for( size_t i=0; i<m_components.size(); ++i )
		{
			T* t=dynamic_cast<T*>(m_components[i].ptr());

			if( t != 0 )
			{
				assert( res == 0 ); // "Several objects with same type exists. Use T* getComponent(const eastl::string& name ) instead." );
				res = t;
			}
		}

		assert( res != 0 ); //, "Component not found" );
		return res;
	}

	/**
	 * Returns component by type. Method will assert, if several component with same type 
	 * and name is found or component for given type and name does not exist. Uses dynamic_cast.
	 */
	template<class T>
	T* getComponent(const eastl::string& name) const
	{
		T* res = 0;
		for( size_t i=0; i<m_components.size(); ++i )
		{
			T* t=dynamic_cast<T*>(m_components[i].ptr());

			if( t != 0 && t->getName() == name )
			{
				assert( res == 0 );//, "Several objects with same type and name exists." );
				res = t;
			}
		}

		assert_message( res != 0 );//, "Component not found" );
		return res;
	}

	/**
	 * Sets translation.
	 */
	void setTranslation( const slmath::vec3& translation );
    
	 	 

	void setTranslation( float x, float y, float z )
	{
		setTranslation(slmath::vec3(x,y,z));
	}
	
	 
	/**
	 * Sets orientation.
	 */
	void setOrientation( const slmath::quat& orientation );

	
	void setOrientation( const slmath::mat4& orientation );

	/**
	 * Sets trasformation from node space to parent space.
	 */
	void setTransform( const slmath::mat4& tr );

	/**
	 * Sets trasformation from node space to world space. Whitout affecting to parent entity transforms.
	 */
	void setWorldTransform( const slmath::mat4& tr );
	
	/*
	 * Returns translation.
	 */
    const slmath::vec3& getTranslation() const;
    
	/**
	 * Returns orientation.
	 */
	const slmath::quat& getOrientation() const;

	/**
	 * Returns trasformation from node space to parent space.
	 */
    slmath::mat4 getTransform() const;
	
	/**
	 * Returns transformation from node space to world space.
	 */
	slmath::mat4 getWorldTransform() const;

	/**
	 * Returns object world translation.
	 */
	slmath::vec3 getWorldTranslation() const;

private:
	void setParent( Entity* parent );
	typedef core::Ref<Component> ComponentRef;
	typedef core::Ref<Entity> EntityRef;

	eastl::vector< ComponentRef >		m_components;
	eastl::vector< EntityRef >			m_childs;
	Entity*								m_parent;
	eastl::string						m_name;

	// Hidden copy constructor and assigment operator.
	Entity( const Entity& );
	Entity& operator=( const Entity& );
	
};


/** 
 * TransformComponent. All Entitys must have one TransformComponent.
 */
class Transform : public Component
{
public:
	/** Default constructor */
	Transform();
	
	/** Destructor */
	virtual ~Transform( );
	
	/** 
	 * This abstract method is called, when component is set to its owner Entity and 
	 * this component should be active.
	 */
	virtual void construct();

	/**
	 * This abstract method is called, when component is removed from its owner Entity and
	 * this component should be disabled.
	 */
	virtual void destroy();

	/**
	 * Sets translation.
	 */
	void setTranslation( const slmath::vec3& translation );
	void setTranslation( float x, float y, float z )
	{
		setTranslation(slmath::vec3(x,y,z));
	}
		 
	/**
	 * Sets orientation.
	 */
	void setOrientation( const slmath::quat& orientation );
	
	void setOrientation( const slmath::mat4& orientation )
	{
		slmath::quat q(orientation);
		setOrientation(q);
	}

	void setScale(float scale) { m_scale = scale; }
	/**
	 * Sets trasformation from node space to parent space.
	 */
	void setTransform( const slmath::mat4& tr );

	/**
	 * Sets trasformation from node space to world space. Whitout affecting to parent entity transforms.
	 */
	void setWorldTransform( const slmath::mat4& tr );
	
	/*
	 * Returns translation.
	 */
    const slmath::vec3& getTranslation() const;
    
	/**
	 * Returns orientation.
	 */
	const slmath::quat& getOrientation() const;

	/**
	 * Returns trasformation from node space to parent space.
	 */
    slmath::mat4 getTransform() const;
	
	/**
	 * Returns transformation from node space to world space.
	 */
	slmath::mat4 getWorldTransform() const;

	slmath::vec3 getWorldTranslation() const;

private:
	slmath::quat                  m_orientation;
    slmath::vec3                  m_translation;
	float                 m_scale;

	// Hidden copy constructor and assigment operator.
	Transform( const Transform& );
	Transform& operator=( const Transform& );
	
};


}



#endif



