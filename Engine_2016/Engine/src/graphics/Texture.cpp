#include <graphics/Texture.h>
#include <graphics/Image.h>
#include <core/log.h>

namespace graphics
{
	namespace 
	{
		void checkOpenGL()
		{
		#ifdef _DEBUG
			GLenum test = glGetError();
			if( test != GL_NO_ERROR )
			{
				switch( test )
				{
				case GL_INVALID_ENUM:
					LOG_ERROR("gl error: GL_INVALID_ENUM\n" );
					break;

				case GL_INVALID_VALUE:
					LOG_ERROR( "gl error: GL_INVALID_VALUE\n" );
					break;

				case GL_INVALID_OPERATION:
					LOG_ERROR( "gl error: GL_INVALID_OPERATION\n" );
					break;

				case GL_OUT_OF_MEMORY:
					LOG_ERROR( "gl error: GL_OUT_OF_MEMORY\n" );
					break;

				default:
					{
						LOG_ERROR( "gl error: %X\n", test );
					}
				}
			}
			assert( test == GL_NO_ERROR );

		#endif
		}

		bool isNPot(int d)
		{
			return d==2 || d==4 || d==8 || d==16 || d==32 || d==64 || d==128 || d==256 || d==512 || d==1024 || d==2048;
 		}

		bool isNpotSquare(int w, int h)
		{
			return w==h && isNPot(w) && isNPot(h);

		}
	}

Texture::Texture()
: m_nativeId(0)
{
	GLuint id;
	glGenTextures ( 1, &id );
	m_nativeId = id;
}


Texture::Texture(GLuint id)
: m_nativeId(id)
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_nativeId);
}


GLuint Texture::getTextureId() const
{
	return m_nativeId;
}

Texture2D::Texture2D(int width, int height, GLint fmt, GLenum type, Texture::FilteringMode filtering, Texture::WrappingMode wrapping)
: Texture()
{
	GLuint boundTexture = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*) &boundTexture);
	checkOpenGL();


	glBindTexture(GL_TEXTURE_2D, getTextureId());
	glTexImage2D(GL_TEXTURE_2D, 0, fmt, width, height, 0,  fmt, type, 0 );
	checkOpenGL();

	// Trilinear filtering
	if( filtering == TRILINEAR_FILTERING )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// Bilinear filtering
	if( filtering == BILINEAR_FILTERING )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// Linear filtering
	if( filtering == LINEAR_FILTERING )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	// No filtering
	if( filtering == NO_FILTERING )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	// Repeat
	if( wrapping == REPEAT )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	if( wrapping == CLAMP )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glBindTexture(GL_TEXTURE_2D, boundTexture );
}

Texture2D::Texture2D()
: Texture()
{
}


Texture2D::~Texture2D()
{
}

void Texture2D::setData( Image* image, Texture::FilteringMode filtering, Texture::WrappingMode wrapping )
{
	if( !isNpotSquare(image->getWidth(),image->getHeight()) )
	{
		LOG_ERROR("Image is not NPOT Square texture (w:%d, h:%d)", image->getWidth(), image->getHeight() );
	}

	GLuint boundTexture = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*) &boundTexture);
	checkOpenGL();

	GLint fmt = image->getBPP() == 3 ? GL_RGB : GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, getTextureId());
	glTexImage2D(GL_TEXTURE_2D, 0, fmt, image->getWidth(), image->getHeight(), 0,  fmt, GL_UNSIGNED_BYTE, image->getData() );
	checkOpenGL();

	// Trilinear filtering
	if( filtering == TRILINEAR_FILTERING )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// Bilinear filtering
	if( filtering == BILINEAR_FILTERING )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// Linear filtering
	if( filtering == LINEAR_FILTERING )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	// No filtering
	if( filtering == NO_FILTERING )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	// Repeat
	if( wrapping == REPEAT )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	if( wrapping == CLAMP )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glBindTexture(GL_TEXTURE_2D, boundTexture );
}

TextureCube::TextureCube()
	: Texture()
{
}

TextureCube::~TextureCube()
{
}

void TextureCube::setData(Image* images[6], Texture::FilteringMode filtering)
{
    glBindTexture( GL_TEXTURE_CUBE_MAP, getTextureId() );
	checkOpenGL();

	for( int i = 0; i < 6; i++ )
    {
		Image* image = images[i];

		if( !isNpotSquare(image->getWidth(),image->getHeight()) )
		{
			LOG_ERROR("Image is not NPOT Square texture (w:%d, h:%d)", image->getWidth(), image->getHeight() );
		}

		GLint fmt = image->getBPP() == 3 ? GL_RGB : GL_RGBA;
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, fmt, image->getWidth(), image->getHeight(), 0,  fmt, GL_UNSIGNED_BYTE, image->getData() );
		checkOpenGL();
    }


	assert( filtering != BILINEAR_FILTERING && filtering != TRILINEAR_FILTERING );

	// Linear filtering
	if( filtering == LINEAR_FILTERING )
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	// No filtering
	if( filtering == NO_FILTERING )
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	// Clamp to edge
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	checkOpenGL();
}


TextureDepth::TextureDepth(int width, int height, GLenum type)
: Texture()
{
	glBindTexture( GL_TEXTURE_2D, getTextureId() );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, type, 0);
}


TextureDepth::~TextureDepth()
{
}


RenderTarget::RenderTarget (int w, int h, bool createDepthBuffer, const GLenum format, const GLenum type )
{
	m_width = w;
	m_height = h;
    // Make a depth buffer texture
    if( createDepthBuffer )
    {
		m_depthBuffer = new TextureDepth(m_width, m_height, GL_UNSIGNED_SHORT);
    }

	checkOpenGL();

	// Make a color buffer texture
	m_colorBuffer = new Texture2D(m_width, m_height, format, type, Texture::LINEAR_FILTERING, Texture::CLAMP );
   
	checkOpenGL();

    // Create a frame buffer object (FBO)
	glGenFramebuffers( 1, &m_FBO );
	
	//checkOpenGL();

    glBindFramebuffer( GL_FRAMEBUFFER, m_FBO );
	
	//checkOpenGL();

	// Set color attachment 0 to FBO
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer->getTextureId(), 0 );
    
//	checkOpenGL();

    if( m_depthBuffer ) 
    {
		// Set depth attachement to FBO
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer->getTextureId(), 0 );
	}
	
	//checkOpenGL();

	// Make sure everything went ok
    if( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE )
    {
		assert(0);
    }

	GLenum test = glGetError();
	/*
    // Make sure everything went ok
    switch( glCheckFramebufferStatus(GL_FRAMEBUFFER) )
    {
    case GL_FRAMEBUFFER_UNSUPPORTED:
		assert(0);
        return;
    }*/

	
		checkOpenGL();


    // Reset default frame buffer (screen)
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	test = glGetError();
		checkOpenGL();
}


RenderTarget::~RenderTarget()
{
	if( m_FBO )
    {
		// Unbind color and texture attachments and delete frame buffer object
		glBindFramebuffer( GL_FRAMEBUFFER, m_FBO );
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0 );

        if( m_depthBuffer )
		{
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0 );
		}
		
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
  
        glDeleteFramebuffers( 1, &m_FBO );
    }
}


void RenderTarget::shareDepthBuffer( TextureDepth* depthBuffer )
{
	m_depthBuffer = depthBuffer;
}

void RenderTarget::bind()
{
	// Bind the FBO and set viewport to be the sizer of the FBO
    glBindFramebuffer( GL_FRAMEBUFFER, m_FBO );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer->getTextureId(), 0 );
    if( m_depthBuffer )
	{
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer->getTextureId(), 0 );
	}

	// Make sure everything went ok
    if( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE )
    {
		assert(0);
    }

    glViewport( 0, 0, m_width, m_height );
}


void RenderTarget::unbind()
{
	// Unbind textures from FBO
    glBindFramebuffer( GL_FRAMEBUFFER, m_FBO );

    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0 );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0 );
}

}


