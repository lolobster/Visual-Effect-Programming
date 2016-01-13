#ifndef _SHADER_H_
#define _SHADER_H_
#include <core/Ref.h>
#include <slmath/mat4.h>
#include <EASTL/vector.h>
#include <EASTL/string.h>
#include <graphics/FrmShader.h>

namespace graphics
{

inline char* BUFFER_OFFSET( int i )
{
	return ((char *)NULL + (i));
}

enum SHADER_ATTRIBUTES {
    ATTRIB_POSITION, 
	ATTRIB_NORMAL, 
	ATTRIB_UV, 
	ATTRIB_TANGENT, 
	ATTRIB_BINORMAL
};




class Shader : public core::Object
{
public:
	Shader(const char* const strVertexShaderFileName, 
		const char* const strFragmentShaderFileName, 
		const FRM_SHADER_ATTRIBUTE* attributes,
		int numAttributes, bool compileFromFile = true )
		: Object()
		, m_program(0)
	{
		if( compileFromFile )
		{
			FrmCompileShaderProgramFromFile(strVertexShaderFileName,
				strFragmentShaderFileName, &m_program, attributes, numAttributes );
		}
		else
		{
			// Compile from string
			FrmCompileShaderProgram(strVertexShaderFileName,
				strFragmentShaderFileName, &m_program, attributes, numAttributes );
		}
	}

	virtual ~Shader()
	{
	}

	GLuint getProgram() const
	{
		return m_program;
	}

	void bind()
	{
		glUseProgram(m_program);
	}

private:
	 GLuint m_program;
};




class ShaderUniforms : public core::Object
{
public:
	ShaderUniforms(Shader* shader)
		: m_shader(shader)
		, m_initDone(false)
	{
		assert( shader != 0 );
	}

	virtual ~ShaderUniforms()
	{
	}

	void bind()
	{
		if( !m_initDone )
		{
			getUniformLocations(m_shader);
			m_initDone = true;
		}

		bind(m_shader);
	}

	Shader* getShader() const 
	{
		return m_shader.ptr();
	}

	virtual void getUniformLocations(Shader* shader) = 0;
	virtual void bind(Shader* shader) = 0;

private:
	core::Ref<Shader> m_shader;
	bool m_initDone;

};


typedef ShaderUniforms BaseMaterial;

}

#endif

