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


		//SimpleMaterialUniformsTextured* smut = new SimpleMaterialUniformsTextured(shader, &m_sharedValues);

		//// Material values for mesh
		//smut->vAmbient = slmath::vec4(0.5f, 0.2f, 1.0f, 1.0f);
		//smut->vDiffuse = slmath::vec4(0.5f, 0.2f, 1.0f, 1.0f);
		//smut->vSpecular = slmath::vec4(1.0f, 1.0f, 1.0f, 5.0f);
		//

		//m_image = graphics::Image::loadFromTGA("assets/WoodDiffuse.tga");
		//m_texture = new graphics::Texture2D();
		//m_texture->setData(m_image);


		//smut->diffuseMap = m_texture;
		//
		//m_material = smut;	

		//// Load cube images
		//eastl::string name = "BedroomCubeMap";

		//cubeImageRefs[0] = graphics::Image::loadFromTGA("assets/" + name + "_RT.tga");
		//cubeImageRefs[1] = graphics::Image::loadFromTGA("assets/" + name + "_LF.tga");
		//cubeImageRefs[2] = graphics::Image::loadFromTGA("assets/" + name + "_DN.tga");
		//cubeImageRefs[3] = graphics::Image::loadFromTGA("assets/" + name + "_UP.tga");
		//cubeImageRefs[4] = graphics::Image::loadFromTGA("assets/" + name + "_FR.tga");
		//cubeImageRefs[5] = graphics::Image::loadFromTGA("assets/" + name + "_BK.tga");

		//graphics::Image* cubeImages[6];
		//for (int i = 0; i < 6; i++)
		//	cubeImages[i] = cubeImageRefs[i].ptr();

		//SimpleMaterialWithTextureUniformsCube* texCube = new SimpleMaterialWithTextureUniformsCube(shader, &m_sharedValues);

		//// create cube map and set data to it
		//core::Ref<graphics::TextureCube> cubeMap = new graphics::TextureCube();
		//texCube->cubeMap = cubeMap;
		//cubeMap->setData(cubeImages);

		m_spriteBatch = new graphics::SpriteBatchGroup();

		checkOpenGL();

	}

	~RenderTextureScene(){};

	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		// create new render target for offscreen rendering if it doesn't exist yet. (Screen size is unknown at construction phase)
		if (m_renderTarget == 0)
		{
			checkOpenGL();
			//											size x,				size y,			 has depth buffer
			m_renderTarget = new graphics::RenderTarget(esContext->width, esContext->height, true);
			checkOpenGL();
		}

		m_spriteBatch->clear();
		float twoPI = 2.0f * 3.14159f;
		float aspectRatio = (float)esContext->width / (float)esContext->height;
		slmath::vec2 screenScale = slmath::vec2(1.0f / aspectRatio, 1.0f);
		slmath::vec2 pos = cosf(twoPI * m_sharedValues.totalTime * 0.1f) * slmath::vec2(0.5f);
		float rot = 0.5f * twoPI + 0.5 * twoPI * cosf(twoPI * m_sharedValues.totalTime * 0.1f);

		m_spriteBatch->addSprite(m_renderTarget->getColorBuffer(), slmath::vec2(0.0f), slmath::vec2(1.0f),
			0xffffffff, pos, rot, 0.0f, slmath::vec2(aspectRatio, 1.0f), screenScale);

		//// Camera perspective matrix = FOV, aspect ratio, near plane distance and far plane distance
		//float fAspect = (float)esContext->width / (float)esContext->height;
		//m_matProjection = slmath::perspectiveFovRH(
		//	slmath::radians(45.0f),
		//	fAspect,
		//	5.0f,
		//	1000.0f);

		//// Look at view matrix = eye point, look at point and world up vector
		//m_matView = slmath::lookAtRH(
		//	slmath::vec3(0.0f, 70.0f, 70.0f),
		//	slmath::vec3(0.0f, 15.0f, 0.0f),
		//	slmath::vec3(0.0f, 1.0f, 0.0f));

		//// Update teapot model matrix
		//m_matModel = slmath::rotationX(-3.1415f*0.5f); // -90 degrees around x first
		//m_matModel = slmath::rotationY(m_sharedValues.totalTime) * m_matModel; // Rotate according to total time
		//m_matModel = slmath::translation(slmath::vec3(0.0f, 0.0f, 0.0f)) * m_matModel; // Translate

		m_sharedValues.totalTime += deltaTime;
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

	core::Ref<graphics::RenderTarget> m_renderTarget;
	core::Ref<graphics::SpriteBatchGroup> m_spriteBatch;
};

#endif