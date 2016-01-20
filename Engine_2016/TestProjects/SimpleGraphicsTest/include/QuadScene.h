#include "scene.h"
#include <core/log.h>
#include "graphics\Shader.h"

class QuadScene : public Scene
{
public:
	QuadScene()
	{

		LOG("QuadScene construct");
		checkOpenGL();

		m_count = 0.0f;
		m_hShaderProgram = 0;

		FRM_SHADER_ATTRIBUTE attributes[3] = {
			{ "g_vVertex", 0 },
			{ "g_vColor", 1 },
			{ "g_vTexCoord", 2 }
		};

		int numAttributes = sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE);
		m_shader = new graphics::Shader("assets/FullScreenQuad.vs", "assets/SparkleEffect.fragmentS", attributes, numAttributes);

		checkOpenGL();
	}


	virtual ~QuadScene()
	{
		
		LOG("QuadScene destruct");
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		m_count += (deltaTime/3);

		if (m_count > 1.0f)
			m_count = 0.0f;

		t += deltaTime;

	}


	virtual void render(graphics::ESContext* esContext)
	{
		checkOpenGL();

		// Set the viewport
		glViewport(0, 0, esContext->width, esContext->height);
		checkOpenGL();

		float aspectRatio = float(esContext->width) / float(esContext->height);
		float fSize = 0.5f;
		float VertexPositions[] =
		{
			-1.0, -1.0, 0.0f, 1.0f,
			1.0, -1.0, 0.0f, 1.0f,
			-1.0, 1.0, 0.0f, 1.0f,
			1.0, 1.0, 0.0f, 1.0f,
		};

		float VertexColors[] = { 
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
		};

		float TexCoords[] = {
			1.0f, 0.0f, // vasen ala
			1.0f, 1.0f, // oikea ala
			0.0f, 0.0f, // vasen ylä
			0.0f, 1.0f, // oikea ylä
		};

		// Clear the backbuffer and depth-buffer
		glClearColor(0.0f, m_count, m_count, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkOpenGL();

		// Set the shader program and the texture
		m_shader->bind();
		checkOpenGL();
		glUniform1f(glGetUniformLocation(m_shader->getProgram(), "t"), t);

		// Draw the colored quad
		glVertexAttribPointer(0, 4, GL_FLOAT, 0, 0, VertexPositions);
		checkOpenGL();

		glEnableVertexAttribArray(0);
		checkOpenGL();

		glVertexAttribPointer(1, 4, GL_FLOAT, 0, 0, VertexColors);
		checkOpenGL();
		glEnableVertexAttribArray(1);
		checkOpenGL();

		glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, TexCoords);
		checkOpenGL();
		glEnableVertexAttribArray(2);
		checkOpenGL();

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		checkOpenGL();

		glDisableVertexAttribArray(0);
		checkOpenGL();
		glDisableVertexAttribArray(1);
		checkOpenGL();
	}
private:
	float m_count;
	float t;
	GLuint       m_hShaderProgram;
	core::Ref<graphics::Shader>m_shader;
};


