#ifndef SIMPLEMESHSCENE_H
#define SIMPLEMESHSCENE_H
#include "scene.h"
#include "MyMaterials.h"
#include "teapot.h"
#include "graphics\Mesh.h"

class SimpleMeshScene : public Scene
{
public:
	SimpleMeshScene()
	{
		LOG("SimpleMeshScene construct");
		checkOpenGL();
		m_sharedValues.totalTime = 0.0f;
		FRM_SHADER_ATTRIBUTE attributes[2]=
		{
			{ "g_vPositionOS", graphics::ATTRIB_POSITION },
			{ "g_vNormalOS", graphics::ATTRIB_NORMAL }
		};

		int numAttributes = sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE);
		// Load shader
		m_shader = new graphics::Shader("assets/Simple3D.vs", "assets/Simple3D.fragS", attributes, numAttributes);

		m_material = new GlobalShaderUniforms(m_shader, &m_sharedValues);

		// Create mesh
		m_mesh = createTeapotMesh();
	}

	~SimpleMeshScene(){};

	graphics::Mesh* createTeapotMesh()
	{
		// Create Index buffer
		graphics::IndexBuffer* ib = new graphics::IndexBuffer(TeapotData::indices, TeapotData::numIndices);

		// Create Vertex arrays
		graphics::VertexArray* va[] =
		{
			new graphics::VertexArrayImpl<slmath::vec3>(
			graphics::ATTRIB_POSITION, (slmath::vec3*)TeapotData::positions, TeapotData::numVertices),

			new graphics::VertexArrayImpl<slmath::vec3>(
			graphics::ATTRIB_NORMAL, (slmath::vec3*)TeapotData::normals, TeapotData::numVertices)
		};

		// Create vertex buffer from vertex arrays
		graphics::VertexBuffer* vb = new graphics::VertexBuffer(&va[0], sizeof(va) / sizeof(va[0]));

		// Create mesh from ib and vb
		return new graphics::Mesh(ib, vb);
	}

	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		// Camera perspective matrix = FOV, aspect ratio, near plane distance and far plane distance
		float fAspect = (float)esContext->width / (float)esContext->height;
		m_matProjection = slmath::perspectiveFovRH(
			slmath::radians(45.0f),
			fAspect,
			5.0f,
			1000.0f);

		// Look at view matrix = eye point, look at point and world up vector
		m_matView = slmath::lookAtRH(
			slmath::vec3(0.0f, 70.0f, 70.0f),
			slmath::vec3(0.0f, 15.0f, 0.0f),
			slmath::vec3(0.0f, 1.0f, 0.0f));

		// Update teapot model matrix
		m_matModel = slmath::rotationX(-3.1415f*0.5f); // -90 degrees around x first
		m_matModel = slmath::rotationY(m_sharedValues.totalTime) * m_matModel; // Rotate according to total time
		m_matModel = slmath::translation(slmath::vec3(0.0f, 0.0f, 0.0f)) * m_matModel; // Translate

		m_sharedValues.totalTime += deltaTime;
	}

	virtual void render(graphics::ESContext* esContext)
	{
		checkOpenGL();
	
		// Set the viewport
		glViewport(0, 0, esContext->width, esContext->height);

		// Clear the backbuffer and depth-buffer
		glClearColor(0.0f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkOpenGL();

		// Initialize GL state
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		checkOpenGL();

		// These values are needed to update for each mesh which is rendered (different model matrix)
		slmath::mat4 matModelView = m_matView * m_matModel;
		slmath::mat4 matModelViewProj = m_matProjection * matModelView;

		// Set matrix to shared values
		m_sharedValues.matModelViewProj = matModelViewProj;

		// Bind material (sets uniform values)
		m_material->bind();
		checkOpenGL();

		// Render the mesh using active material
		m_mesh->render();
		checkOpenGL();
	}

private:
	SharedShaderValues m_sharedValues;
	core::Ref<graphics::Shader> m_shader;
	core::Ref<graphics::Mesh>m_mesh;
	core::Ref<graphics::ShaderUniforms>m_material;
	slmath::mat4 m_matProjection;
	slmath::mat4 m_matView;
	slmath::mat4 m_matModel;
};

#endif