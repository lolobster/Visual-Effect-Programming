#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <core/Object.h>
#include <core/Ref.h>
#include <eastl/string.h>
#include <graphics/OpenGLES/es_util.h>

namespace graphics
{
	class Image;

class Texture : public core::Object
{
public:
	Texture();
	Texture(GLuint id);

	virtual ~Texture();

	GLuint getTextureId() const;


	enum FilteringMode
	{
		NO_FILTERING,
		LINEAR_FILTERING,
		BILINEAR_FILTERING,
		TRILINEAR_FILTERING
	};

	enum WrappingMode
	{
		CLAMP,
		REPEAT		
	};
private:

	Texture(const Texture&);
	Texture& operator=(const Texture&);

	GLuint m_nativeId;
};


class Texture2D : public Texture
{
public:
	Texture2D(int width, int height, GLint fmt, GLenum type, Texture::FilteringMode filtering = TRILINEAR_FILTERING, Texture::WrappingMode wrapping = REPEAT);
	Texture2D();
	virtual ~Texture2D();
	void setData(Image*, Texture::FilteringMode filtering = TRILINEAR_FILTERING, Texture::WrappingMode wrapping = REPEAT);
	//void setData(Image*, Texture::FilteringMode filtering = TRILINEAR_FILTERING, Texture::WrappingMode wrapping = REPEAT);
	
//	void setData(Image*);
private:
	Texture2D(const Texture2D&);
	Texture2D& operator=(const Texture2D&);
};

class TextureCube : public Texture
{
public:
	TextureCube();
	~TextureCube();

	void setData(Image* images[6], Texture::FilteringMode filtering = LINEAR_FILTERING);
private:

};

class TextureDepth : public Texture
{
public:
	TextureDepth(int width, int height, GLenum type);
	virtual ~TextureDepth();
private:
	TextureDepth(const TextureDepth&);
	TextureDepth& operator=(const TextureDepth&);
};

class RenderTarget : public core::Object
{
public:
    RenderTarget (int width, int height, bool createDepthBuffer, const GLenum format = GL_RGBA, const GLenum type = GL_UNSIGNED_BYTE );
    ~RenderTarget ();
    void shareDepthBuffer( TextureDepth* depthBuffer );
    
	void bind();
    void unbind();

	Texture* getColorBuffer() const { return 	m_colorBuffer.ptr();}
	Texture* getDepthBuffer() const { return 	m_depthBuffer.ptr(); }

private:
    // frame buffer object
    uint32_t  m_FBO;

    // re-using buffers as samplers
	core::Ref<Texture>		m_colorBuffer;
	core::Ref<Texture>	m_depthBuffer;

	int m_width;
	int m_height;
};



}


#endif 


