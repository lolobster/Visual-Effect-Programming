#include "scene.h"
#include <core/log.h>

class QuadScene : public Scene
{
public:
	QuadScene()
	{

		LOG("QuadScene construct");
		checkOpenGL();

		m_count = 0.0f;
		m_hShaderProgram = 0;

		const char* g_strVSProgram =
			"attribute vec4 g_vVertex;											 \n"
			"attribute vec4 g_vColor;											 \n"
			"attribute vec2 g_vTexCoord;										 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"varying   vec2 g_vVSTexCoord;										 \n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"    gl_Position  = vec4( g_vVertex.x, g_vVertex.y,                  \n"
			"                         g_vVertex.z, g_vVertex.w );                \n"
			"    g_vVSColor = g_vColor;                                          \n"
			"    g_vVSTexCoord = g_vTexCoord;                                    \n"
			"}																	 \n";


		const char* g_strFSProgram =
			"#ifdef GL_FRAGMENT_PRECISION_HIGH									 \n"
			"   precision highp float;											 \n"
			"#else																 \n"
			"   precision mediump float;										 \n"
			"#endif																 \n"
			"																	 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"varying   vec2 g_vVSTexCoord;										 \n"
			"uniform float t;													 \n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"	float x = g_vVSTexCoord.x * 612;                                 \n"
			"	float y = g_vVSTexCoord.y * 612;								 \n"
			"    float vv = 0.25 * (											 \n"
			"    (0.5 + (0.5 * sin(x / 7.0 + t)))+								 \n"
			"    (0.5 + (0.5 * sin(y / 5.0 - t)))+                               \n"
			"    (0.5 + (0.5 * sin((x + y) / 6.0 - t)))+                         \n"
			"    (0.5 + (0.5 * tan(sqrt(float(x * x + y * y)) / 4.0 - t)))       \n"
			"	);                                                               \n"
			"    gl_FragColor = vec4(vv, vv, vv, 1.0) * g_vVSColor;							 \n"
			"}																	 \n";

		FRM_SHADER_ATTRIBUTE attributes[3] = {
			{ "g_vVertex", 0 },
			{ "g_vColor", 1 },
			{ "g_vTexCoord", 2 }
		};

		FrmCompileShaderProgram(g_strVSProgram, g_strFSProgram, &m_hShaderProgram, attributes, sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE));

		checkOpenGL();
	}


	virtual ~QuadScene()
	{
		glDeleteProgram(m_hShaderProgram);
		LOG("QuadScene destruct");
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		m_count += deltaTime;

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
			-0.95, -0.95, 0.0f, 1.0f,
			0.95, -0.95, 0.0f, 1.0f,
			-0.95, 0.95, 0.0f, 1.0f,
			0.95, 0.95, 0.0f, 1.0f,
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
		glUseProgram(m_hShaderProgram);
		checkOpenGL();
		glUniform1f(glGetUniformLocation(m_hShaderProgram, "t"), t);

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

};


