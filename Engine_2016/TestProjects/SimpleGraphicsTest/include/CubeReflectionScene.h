#ifndef CubeReflectionScene_h
#define CubeReflectionScene_h
#include "scene.h"
#include "MyMaterials.h"
#include "SimpleMaterialUniforms.h"
#include "teapot.h"
#include "graphics\Texture.h"
#include "graphics\Image.h"
#include "graphics\Mesh.h"

class CubeReflectionScene : public Scene
{
public:
	CubeReflectionScene()
	{
		LOG("PlimPlomScene construct");
		checkOpenGL();
		m_sharedValues.totalTime = 0.0f;
		FRM_SHADER_ATTRIBUTE attributes[3] =
		{
			{ "g_vPositionOS", graphics::ATTRIB_POSITION },
			{ "g_vNormalOS", graphics::ATTRIB_NORMAL },
			{ "g_vTextureCoords", graphics::ATTRIB_UV }
		};

		core::Ref<graphics::Shader>shader =
			new graphics::Shader("assets/PlimPlomTexCube.vs", "assets/PlimPlomTexCube.fragS",
			attributes, sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE));

		// Load cube images
		eastl::string name = "BedroomCubeMap";

		cubeImageRefs[0] = graphics::Image::loadFromTGA("assets/" + name + "_RT.tga");
		cubeImageRefs[1] = graphics::Image::loadFromTGA("assets/" + name + "_LF.tga");
		cubeImageRefs[2] = graphics::Image::loadFromTGA("assets/" + name + "_DN.tga");
		cubeImageRefs[3] = graphics::Image::loadFromTGA("assets/" + name + "_UP.tga");
		cubeImageRefs[4] = graphics::Image::loadFromTGA("assets/" + name + "_FR.tga");
		cubeImageRefs[5] = graphics::Image::loadFromTGA("assets/" + name + "_BK.tga");

		graphics::Image* cubeImages[6];
		for (int i = 0; i < 6; i++)
			cubeImages[i] = cubeImageRefs[i].ptr();

		// create cube map and set data to it
		core::Ref<graphics::TextureCube> cubeMap = new graphics::TextureCube();
		cubeMap->setData(cubeImages);

		SimpleMaterialUniformsTextured* smut = new SimpleMaterialUniformsTextured(shader, &m_sharedValues);

		// Material values for mesh
		smut->vAmbient = slmath::vec4(0.5f, 0.2f, 1.0f, 1.0f);
		smut->vDiffuse = slmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		smut->vSpecular = slmath::vec4(1.0f, 1.0f, 1.0f, 50.0f);
		

		m_image = graphics::Image::loadFromTGA("assets/WoodDiffuse.tga");
		m_texture = new graphics::Texture2D();
		m_texture->setData(m_image);


		smut->diffuseMap = m_texture;
		
		m_material = smut;	
		// Create mesh
		m_mesh = createTeapotMesh();
	}

	~CubeReflectionScene(){};

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
			graphics::ATTRIB_NORMAL, (slmath::vec3*)TeapotData::normals, TeapotData::numVertices),

			new graphics::VertexArrayImpl<slmath::vec3>(
			graphics::ATTRIB_UV, (slmath::vec3*)TeapotData::texCoords, TeapotData::numVertices)
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

		// Calculate needed stuff for m_sharedValues
		m_sharedValues.matModel = m_matModel;
		m_sharedValues.matView = m_matView;
		m_sharedValues.matProj = m_matProjection;

		slmath::mat4 matModelView = m_matView * m_matModel;
		slmath::mat4 matModelViewProj = m_matProjection * matModelView;
		slmath::mat4 matNormal = slmath::transpose(slmath::inverse(matModelView));

		m_sharedValues.matModelView = matModelView;
		m_sharedValues.matNormal = matNormal;
		m_sharedValues.matModelViewProj = matModelViewProj;

		m_sharedValues.lightPos = slmath::vec3(0.0, 70.0f, 70.0f);
		m_sharedValues.camPos = slmath::vec3(0.0, 70.0f, 70.0f);

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
	core::Ref<graphics::Texture2D> m_texture;
	core::Ref<graphics::Image> m_image;
	core::Ref<graphics::Mesh>m_mesh;
	core::Ref<graphics::ShaderUniforms>m_material;
	slmath::mat4 m_matProjection;
	slmath::mat4 m_matView;
	slmath::mat4 m_matModel;

	core::Ref<graphics::Image>cubeImageRefs[6];
};

#endif