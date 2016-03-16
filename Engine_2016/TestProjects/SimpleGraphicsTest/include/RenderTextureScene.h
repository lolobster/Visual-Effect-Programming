#ifndef RenderTextureScene_h
#define RenderTextureScene_h
#include "scene.h"
#include "MyMaterials.h"
#include "SimpleMaterialUniforms.h"
#include "teapot.h"
#include "graphics\Texture.h"
#include "graphics\Image.h"
#include "graphics\Mesh.h"
#include "graphics\SpriteBatch.h"

class RenderTextureScene : public Scene
{
public:
	RenderTextureScene()
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


		SimpleMaterialUniformsTextured* smut = new SimpleMaterialUniformsTextured(shader, &m_sharedValues);

		// Material values for mesh
		smut->vAmbient = slmath::vec4(0.5f, 0.2f, 1.0f, 1.0f);
		smut->vDiffuse = slmath::vec4(0.5f, 0.2f, 1.0f, 1.0f);
		smut->vSpecular = slmath::vec4(1.0f, 1.0f, 1.0f, 5.0f);
		

		m_image = graphics::Image::loadFromTGA("assets/MetalPlateDiffuse.tga");
		m_texture = new graphics::Texture2D();
		m_texture->setData(m_image);


		smut->diffuseMap = m_texture;
		
		m_material = smut;	

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

		SimpleMaterialWithTextureUniformsCube* texCube = new SimpleMaterialWithTextureUniformsCube(shader, &m_sharedValues);

		// create cube map and set data to it
		core::Ref<graphics::TextureCube> cubeMap = new graphics::TextureCube();
		cubeMap->setData(cubeImages);
		
		texCube->cubeMap = cubeMap;
		checkOpenGL();

		m_material = texCube;
		checkOpenGL();
		m_mesh = createTeapotMesh();

		m_spriteBatch = new graphics::SpriteBatchGroup();

		checkOpenGL();

		glDisable(GL_BLEND);
		glEnable(GL_CULL_Face);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		checkOpenGL();
	}

	~RenderTextureScene()
	{
		glDeleteProgram(m_shader->getProgram());
		LOG("RenderTextureScene destruct");
	}

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
	}
	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		camX = 0.0f;
		camY = 0.7f;
		camZ = cos(m_sharedValues.m_totalTime) + 1.0f;

		m_matView = slmath::lookAtRH(
			slmath::vec3(camX, camY, camZ),
			slmath::vec3(0.0f, 0.0f, 0.0f),
			slamath::vec3(0.0f, 1.0f, 0.0f));

		camX = 0.7*cos(m_sharedValues.totalTime);
		camZ = 0.7*cos(m_sharedValues.totalTime);

		m_matView2 = slmath::lookAtRH(
			slmath::vec3(camX, camY, camZ),
			slmath::vec3(0.0f, 0.2f, 0.0f),
			slamath::vec3(0.0f, 1.0f, 0.0f));

		m_matModel = slmath::rotationX(-3.14159f*0.5f);
		m_matModel = slmath::rotationY(0)*m_matModel;
		m_matModel = slmath::translation(slmath::vec3(0.0f, 0.0f, 0.0f))*m_matModel;
		m_matModel = slmath::scaling(0.01f)*m_matModel;

		m_sharedValues.matModel = m_matModel;
		m_sharedValues.matView = m_matView2;
		m_sharedValues.matProj = m_matProjection;

		slmath::mat4 matModelView = m_matView2 * m_matModel;
		slmath::mat4 matModelViewProj = m_matProjection * matModelView;
		slmath::mat4 matNormal = slmath::transpose(slmath::inverse(matModelView));

		m_sharedValues.matModelView = matModelView;
		m_sharedValues.matNormal = matNormal;
		m_sharedValues.matModelViewProj = matModelViewProj;

		m_sharedValues.camPos = slmath::vec3(camX, camY, camZ);
		
		// create new render target for offscreen rendering if it doesn't exist yet. (Screen size is unknown at construction phase)
		if (m_renderTarget == 0)
		{
			checkOpenGL();
			//											size x,				size y,			 has depth buffer
			m_renderTarget = new graphics::RenderTarget(esContext->width, esContext->height, true);
			checkOpenGL();
		}

		m_sharedValues.totalTime += deltaTime;

		m_spriteBatch->clear();
		float twoPI = 2.0f * 3.14159f;
		float aspectRatio = (float)esContext->width / (float)esContext->height;
		slmath::vec2 screenScale = slmath::vec2(1.0f / aspectRatio, 1.0f);
		slmath::vec2 pos = cosf(twoPI * m_sharedValues.totalTime * 0.1f) * slmath::vec2(0.5f);
		float rot = 0.5f * twoPI + 0.5 * twoPI * cosf(twoPI * m_sharedValues.totalTime * 0.1f);

		m_spriteBatch->addSprite(m_renderTarget->getColorBuffer(), slmath::vec2(0.0f), slmath::vec2(1.0f),
			0xffffffff, pos, rot, 0.0f, slmath::vec2(aspectRatio, 1.0f), screenScale);

	}

	virtual void render(graphics::ESContext* esContext)
	{
		checkOpenGL();

		// Bind offscreen render target to be as active frame buffer
		m_renderTarget->bind();
		GLenum test = glGetError();
		checkOpenGL();

		// Clear render target
		glClearColor(0.0f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkOpenGL();

		/////////////////////////////////
		
		// mlg codezzz
		
		////////////////////////////////

		m_renderTarget->unbind();
		// Bind screen to be as active framebuffer and clear it with black color
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, esContext->width, esContext->height);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		test = glGetError();
		checkOpenGL();

		// Render the mesh using active material
		m_spriteBatch->render();
		checkOpenGL();
	}

private:
	float camX = 0.0f;
	float camY = 0.0f;
	float camZ = 0.0f;

	SharedShaderValues m_sharedValues;

	core::Ref<graphics::Shader> m_shader;
	core::Ref<graphics::Texture2D> m_texture;
	core::Ref<graphics::Image> m_image;
	core::Ref<graphics::Mesh>m_mesh;
	core::Ref<graphics::ShaderUniforms>m_material;

	slmath::mat4 m_matProjection;
	slmath::mat4 m_matView;
	slmath::mat4 m_matView2;
	slmath::mat4 m_matModel;

	core::Ref<graphics::Image>cubeImageRefs[6];

	core::Ref<graphics::RenderTarget> m_renderTarget;
	core::Ref<graphics::SpriteBatchGroup> m_spriteBatch;
};

#endif