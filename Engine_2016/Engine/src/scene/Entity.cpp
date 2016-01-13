#include <scene/Entity.h>
#include <es_assert.h>

namespace scene
{

Component::Component( )
	: Object()
{
}


Component::~Component( )
{
}


void Component::setName( const eastl::string& name )
{
	m_name = name;
}


Entity* Component::getOwner() const
{
	return m_owner;
}


const eastl::string Component::getName() const
{
	return m_name;
}


Entity::Entity( const eastl::string& name ) :
	 Object()
	, m_components()
	, m_childs()
	, m_parent(0)
	, m_name(name)
{
}


Entity::~Entity( )
{
	for( size_t i=0; i<m_components.size(); ++i )
	{
		m_components[i]->destroy();
	}
}

void Entity::addComponent( Component* newComponent )
{
	m_components.push_back(newComponent);
	newComponent->m_owner = this;
	newComponent->construct();
}

void Entity::addChild( Entity* child )
{
	assert( child->getParent() == 0 ); // Only one parent is allowed.

	child->setParent( this );
	m_childs.push_back(child);
}

Entity* Entity::getParent() const
{
	return m_parent;
}

const eastl::string Entity::getName() const
{
	return m_name;
}

Entity* Entity::getChildByName(const eastl::string& name) const
{
	for( size_t i=0; i<m_childs.size(); ++i )
	{
		if( m_childs[i]->getName() == name )
		{
			return m_childs[i].ptr();
		}
	}

	return 0;
}

Entity* Entity::find(const eastl::string& name) const
{
	Entity* res = getChildByName(name);
	
	for( size_t i=0; i<m_childs.size() && res == 0; ++i )
	{
		res = m_childs[i]->find(name);
	}
	
	return res;

}
void Entity::setParent( Entity* parent )
{
	m_parent = parent;
}


void Entity::setTranslation( const slmath::vec3& translation )
{
	getComponent<Transform>()->setTranslation(translation);
}


void Entity::setOrientation( const slmath::quat& orientation )
{
	getComponent<Transform>()->setOrientation(orientation);
}


void Entity::setOrientation( const slmath::mat4& orientation )
{
	getComponent<Transform>()->setOrientation(orientation);
}


void Entity::setTransform( const slmath::mat4& tr )
{
	getComponent<Transform>()->setTransform(tr);
}


void Entity::setWorldTransform( const slmath::mat4& tr )
{
	getComponent<Transform>()->setWorldTransform(tr);
}
	

const slmath::vec3& Entity::getTranslation() const
{
	return getComponent<Transform>()->getTranslation();
}
    

const slmath::quat& Entity::getOrientation() const
{
	return getComponent<Transform>()->getOrientation();
}


slmath::mat4 Entity::getTransform() const
{
	return getComponent<Transform>()->getTransform();
}


slmath::mat4 Entity::getWorldTransform() const
{
	return getComponent<Transform>()->getWorldTransform();
}


slmath::vec3 Entity::getWorldTranslation() const
{
	return getComponent<Transform>()->getWorldTranslation();
}



Transform::Transform()
	: m_scale(1.0f)
	, m_orientation( 0.0f, 0.0f, 0.0f, 1.0f )
	, m_translation( 0.0f, 0.0f, 0.0f )
{
}

Transform::~Transform( )
{
}

void Transform::construct()
{
}

void Transform::destroy()
{
}

void Transform::setTranslation( const slmath::vec3& translation )
{
    m_translation = translation;
}


void Transform::setOrientation( const slmath::quat& orientation )
{
    m_orientation = slmath::normalize(orientation);
}

void Transform::setTransform( const slmath::mat4& tr )
{
	slmath::vec4 zero(0.0f,0.0f,0.0f,1.0f);
	slmath::vec4 translation = tr * zero;
	setTranslation( translation.xyz() );
	setOrientation( slmath::quat(tr) );
	setScale(1.0f);
}

void Transform::setWorldTransform( const slmath::mat4& tr )
{
	slmath::mat4 localTransform = tr;
	if( getOwner()->getParent() != 0 )
	{
		slmath::mat4 parentTransform = getOwner()->getParent()->getComponent<Transform>()->getWorldTransform();
		localTransform = tr*slmath::inverse(parentTransform);
	}

	setTransform(localTransform); 
}

const slmath::vec3& Transform::getTranslation() const
{
    return m_translation;
}

const slmath::quat& Transform::getOrientation() const
{
    return m_orientation;
}

slmath::mat4 Transform::getTransform() const
{
	return slmath::translation(m_translation) * slmath::mat4(m_orientation) * slmath::scaling(m_scale);
}

slmath::mat4 Transform::getWorldTransform() const
{
	if( getOwner()->getParent() )
	{
		return getOwner()->getParent()->getComponent<Transform>()->getWorldTransform() * getTransform();
	}
	else
	{
		return getTransform();
	}
}

slmath::vec3 Transform::getWorldTranslation() const
{
	return (getWorldTransform() * slmath::vec4(0,0,0,1)).xyz();	
}


}

