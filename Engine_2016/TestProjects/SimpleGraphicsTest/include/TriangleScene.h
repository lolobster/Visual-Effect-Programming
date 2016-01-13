#include "scene.h"
#include <core/log.h>

class TriangleScene : public Scene
{
public:
	TriangleScene()
	{
		
		LOG("TriangleScene construct");
		checkOpenGL();

		m_count = 0.0f;
		m_hShaderProgram = 0;

		const char* g_strVSProgram = 
			"attribute vec4 g_vVertex;											 \n"
			"attribute vec4 g_vColor;											 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"    gl_Position  = vec4( g_vVertex.x, g_vVertex.y,                  \n"
			"                         g_vVertex.z, g_vVertex.w );                \n"
			"    g_vVSColor = g_vColor;                                          \n"
			"}																	 \n";


		const char* g_strFSProgram = 
			"#ifdef GL_FRAGMENT_PRECISION_HIGH									 \n"
			"   precision highp float;											 \n"
			"#else																 \n"
			"   precision mediump float;										 \n"
			"#endif																 \n"
			"																	 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"    gl_FragColor = g_vVSColor;                                      \n"
			"}																	 \n";

		FRM_SHADER_ATTRIBUTE attributes[2] = {
			{"g_vVertex", 0 },
			{"g_vColor",  1 }
		};

		FrmCompileShaderProgram(g_strVSProgram,g_strFSProgram,&m_hShaderProgram,attributes,sizeof(attributes)/sizeof(FRM_SHADER_ATTRIBUTE) );

		checkOpenGL();
	}


    virtual ~TriangleScene()
	{
		glDeleteProgram( m_hShaderProgram );
		LOG("TriangleScene destruct");
	}


    virtual void update( graphics::ESContext* esContext, float deltaTime )
	{
		m_count += deltaTime;

		if( m_count > 1.0f )
			m_count = 0.0f;
	}


	virtual void render(graphics::ESContext* esContext)
	{
		checkOpenGL();

		// Set the viewport
		glViewport( 0, 0, esContext->width, esContext->height );
		checkOpenGL();

		float aspectRatio = float(esContext->width)/float(esContext->height);
		float fSize = 0.5f;
		float VertexPositions[] =
		{
			 0.0f,  +fSize*aspectRatio, 0.0f, 1.0f,
			-fSize, -fSize*aspectRatio, 0.0f, 1.0f,
			+fSize, -fSize*aspectRatio, 0.0f, 1.0f,
		};

		float VertexColors[] = {
			1.0f, 0.0f, 0.0f, 1.0f,
								0.0f, 1.0f, 0.0f, 1.0f,
								0.0f, 0.0f, 1.0f, 1.0f
							   };

		// Clear the backbuffer and depth-buffer
		glClearColor( 0.0f, m_count, m_count, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		checkOpenGL();

		// Set the shader program and the texture
		glUseProgram( m_hShaderProgram );
		checkOpenGL();

		// Draw the colored triangle
		glVertexAttribPointer( 0, 4, GL_FLOAT, 0, 0, VertexPositions );
		checkOpenGL();

		glEnableVertexAttribArray( 0 );
		checkOpenGL();

		glVertexAttribPointer( 1, 4, GL_FLOAT, 0, 0, VertexColors);
		checkOpenGL();
		glEnableVertexAttribArray( 1 );
		checkOpenGL();

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 3 );
		checkOpenGL();

		glDisableVertexAttribArray( 0 );
		checkOpenGL();
		glDisableVertexAttribArray( 1 );
		checkOpenGL();
	}
private:
	float m_count;
	
	GLuint       m_hShaderProgram;

};


