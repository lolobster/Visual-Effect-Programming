#ifndef RENDERER_H_
#define RENDERER_H_


#include <scene/Entity.h>
#include <graphics/Mesh.h>
#include <graphics/Shader.h>

namespace graphics
{

// Struct, which maps attribute names to locations.
// Each shader program must have attribute arrays with same variable 
// names.
static const FRM_SHADER_ATTRIBUTE MY_SHADER_ATTRIBUTES[] = 
{
		{"g_vPositionOS", ATTRIB_POSITION },
		{"g_vTexCoordOS", ATTRIB_UV },
		{"g_vNormalOS", ATTRIB_NORMAL },
		{"g_vBinormalOS", ATTRIB_BINORMAL },
		{"g_vTangentOS", ATTRIB_TANGENT }
};


// Shared "global values", which is used by each shader program. 
// Typically this consists of different matrix values, light
// related values etc. which might be needed by each shader.
// Usage: Create one object of type SharedShaderValues to the 
// scene and pass it for each shader as uniforms to be used.
struct SharedShaderValues : public core::Object
{
	slmath::mat4 matModel;				// Model matrix (object world matrix)
	slmath::mat4 matView;				// View matrix. (inverse of camera world matrix) 
	slmath::mat4 matProj;				// Projision matrix of the camera
	slmath::mat4 matModelView;			// Model view matrix. Used to transform position vertices to camera space.
	slmath::mat4 matNormal;				// Model view matrix. Used to transform normal/binormal/tangent vertices to camera space.
	slmath::mat4 matModelViewProj;		// Model view projection matrix. Used to transform position vertices to clip space.
	slmath::vec3 lightPos;				// World poition of point light.
	slmath::vec3 lightColor;				// Light color.
	slmath::vec3 camPos;				// World position of camera.
};

// Class for uniforms, which are used by each shader. Sets values
// of SharedShaderValues-object to shader program in bind.
class GlobalShaderUniforms : public graphics::ShaderUniforms
{
public:
	GlobalShaderUniforms(graphics::Shader* shader, const SharedShaderValues* shaderShaderValues)
		: ShaderUniforms(shader)
		, m_shaderShaderValues(shaderShaderValues)
	{
	}

	virtual ~GlobalShaderUniforms()
	{
	}

protected:
	virtual void getUniformLocations(graphics::Shader* shader)
	{
		// Get uniform locations
		m_ids[0] = glGetUniformLocation(shader->getProgram(),	"g_matModel");
		m_ids[1] = glGetUniformLocation(shader->getProgram(),	"g_matView");
		m_ids[2] = glGetUniformLocation(shader->getProgram(),	"g_matProj");
		m_ids[3] = glGetUniformLocation(shader->getProgram(),	"g_matModelView");
		m_ids[4] = glGetUniformLocation(shader->getProgram(),	"g_matNormal");
		m_ids[5] = glGetUniformLocation(shader->getProgram(),	"g_matModelViewProj");
		m_ids[6] = glGetUniformLocation(shader->getProgram(),	"g_lightPos");
		m_ids[7] = glGetUniformLocation(shader->getProgram(),	"g_lightColor");
		m_ids[8] = glGetUniformLocation(shader->getProgram(),	"g_camPos");
 	}

	virtual void bind(graphics::Shader* shader)
	{
		shader->bind();

		glUniformMatrix4fv( m_ids[0], 1, GL_FALSE, &m_shaderShaderValues->matModel[0][0]);
		glUniformMatrix4fv( m_ids[1], 1, GL_FALSE, &m_shaderShaderValues->matView[0][0]);
		glUniformMatrix4fv( m_ids[2], 1, GL_FALSE, &m_shaderShaderValues->matProj[0][0]);
		glUniformMatrix4fv( m_ids[3], 1, GL_FALSE, &m_shaderShaderValues->matModelView[0][0]);
		glUniformMatrix4fv( m_ids[4], 1, GL_FALSE, &m_shaderShaderValues->matNormal[0][0]);
		glUniformMatrix4fv( m_ids[5], 1, GL_FALSE, &m_shaderShaderValues->matModelViewProj[0][0]);
		
		glUniform3f( m_ids[6], m_shaderShaderValues->lightPos.x, m_shaderShaderValues->lightPos.y, m_shaderShaderValues->lightPos.z);
		glUniform3f( m_ids[7], m_shaderShaderValues->lightColor.x, m_shaderShaderValues->lightColor.y, m_shaderShaderValues->lightColor.z);
		glUniform3f( m_ids[8], m_shaderShaderValues->camPos.x, m_shaderShaderValues->camPos.y, m_shaderShaderValues->camPos.z);
	}
private:
	const SharedShaderValues*	m_shaderShaderValues;
	GLint						m_ids[9];
};

class MeshInstance : public scene::Component
{
public:
	MeshInstance(graphics::Mesh* mesh, const SharedShaderValues* shaderShaderValues, graphics::BaseMaterial* material)
		: Component()
		, m_mesh(mesh)
		, m_globalUniforms( new GlobalShaderUniforms(material->getShader(),shaderShaderValues) )
		, m_material(material)
	{
	}

	virtual void construct()
	{
	}

	virtual void destroy()
	{
	}

	graphics::Mesh* getMesh() const
	{
		return m_mesh.ptr();
	}

	graphics::BaseMaterial* getMaterial() const
	{
		return m_material.ptr();
	}

	void render()
	{
		m_globalUniforms->bind();
		m_material->bind();
		m_mesh->render();
	}

private: 
	core::Ref<graphics::Mesh> m_mesh;
	core::Ref<graphics::ShaderUniforms> m_globalUniforms;
	core::Ref<graphics::BaseMaterial> m_material;
};



class Camera : public scene::Component
{
public:
	Camera( SharedShaderValues* shaderShaderValues )
		: Component()
		, m_shaderShaderValues(shaderShaderValues)
	{
	}

	virtual void construct()
	{
	}

	virtual void destroy()
	{
	}

	void setPerspective(float fovyInRadians, float aspect, float znear, float zfar )
	{
		m_fovyInRadians = fovyInRadians;		
		m_aspect = aspect;
		m_znear = znear;
		m_zfar = zfar;
	}

	void updateToSharedValues()
	{
		scene::Transform* transform = getOwner()->getComponent<scene::Transform>();

		m_shaderShaderValues->matView = slmath::inverse(transform->getWorldTransform());
		m_shaderShaderValues->matProj = slmath::perspectiveFovRH(m_fovyInRadians, m_aspect, m_znear, m_zfar);
		m_shaderShaderValues->camPos = transform->getWorldTranslation();
		
	//	LOG("Cam pos: x=%2.2f,y=%2.2f,z=%2.2f", m_shaderShaderValues->camPos.x, m_shaderShaderValues->camPos.y, m_shaderShaderValues->camPos.z);
	}

private: 
	float m_fovyInRadians;
	float m_aspect;
	float m_znear;
	float m_zfar;

	core::Ref<SharedShaderValues> m_shaderShaderValues;

};


class Light : public scene::Component
{
public:
	Light( SharedShaderValues* shaderShaderValues )
		: Component()
		, m_shaderShaderValues(shaderShaderValues)
	{
		m_shaderShaderValues->lightColor = slmath::vec3(1.0f);
	}

	virtual void construct()
	{
	}

	virtual void destroy()
	{
	}

	void setColor(const slmath::vec3& color)
	{
		m_shaderShaderValues->lightColor = color;
	}

	void updateToSharedValues()
	{
		scene::Transform* transform = getOwner()->getComponent<scene::Transform>();
		m_shaderShaderValues->lightPos = transform->getWorldTranslation();
	}

private: 
	core::Ref<SharedShaderValues> m_shaderShaderValues;
};


class Renderer : public core::Object
{
public:
	Renderer()
		: Object()
	{
		m_shaderShaderValues = new SharedShaderValues();
	}

	Camera* createCamera()
	{
		assert( m_camera == 0);
		m_camera = new Camera(m_shaderShaderValues);
		return m_camera;
	}

	Light* createLight()
	{
		assert( m_light == 0);
		m_light = new Light(m_shaderShaderValues);
		return m_light;
	}

	MeshInstance* createMeshInstance(Mesh* mesh, BaseMaterial* material)
	{
		MeshInstance* res = new MeshInstance(mesh, m_shaderShaderValues,material);
		m_rendableComponents.push_back(res);
		return res;
	}

	virtual ~Renderer()
	{
	}

	void render( ESContext* esContext)
	{
		// Set the viewport
		glViewport( 0, 0, esContext->width, esContext->height );
   
		// Clear the backbuffer and depth-buffer
		glClearColor( 0.0f, 0.8f, 0.8f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//		checkOpenGL();

		// Initialize GL state.
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

//		checkOpenGL();
		if( m_camera )
			m_camera->updateToSharedValues();

		if( m_light )
				m_light->updateToSharedValues();

		for( size_t i=0; i<m_rendableComponents.size(); ++i )
		{
			MeshInstance* mesh = m_rendableComponents[i];
			assert(mesh != 0);
			assert( mesh->getOwner() );

			m_shaderShaderValues->matModel		= mesh->getOwner()->getWorldTransform();
			slmath::mat4 matModelView			= m_shaderShaderValues->matView * m_shaderShaderValues->matModel;
			slmath::mat4 matModelViewProj		= m_shaderShaderValues->matProj * matModelView;
			slmath::mat4 matNormal				= slmath::transpose(slmath::inverse(matModelView));
			m_shaderShaderValues->matModelView		= matModelView;
			m_shaderShaderValues->matNormal			= matNormal;
			m_shaderShaderValues->matModelViewProj	= matModelViewProj;

			mesh->render();
		}
	}

private:
	core::Ref<SharedShaderValues>	m_shaderShaderValues;
	eastl::vector<MeshInstance*>	m_rendableComponents;
	core::Ref<Camera>				m_camera;
	core::Ref<Light>				m_light;
};

}


#endif




