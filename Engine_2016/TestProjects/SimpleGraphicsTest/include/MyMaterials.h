#pragma once

#include "graphics\Shader.h"


struct SharedShaderValues
{
	float totalTime;
};


class GlobalShaderUniforms :
	public graphics::ShaderUniforms
	
{
public:
	GlobalShaderUniforms(graphics::Shader* shader, const SharedShaderValues* shaderShaderValues) : ShaderUniforms(shader), m_shaderShaderValues(shaderShaderValues)
	{
	}
	~GlobalShaderUniforms() {};
	
	virtual void getUniformLocations(graphics::Shader* shader) 
	{
		m_id = glGetUniformLocation(shader->getProgram(), "totalTime");
	}

	virtual void bind(graphics::Shader* shader) {
		shader->bind();

		if (m_shaderShaderValues)
		{
			glUniform1f(m_id, m_shaderShaderValues->totalTime);
		}
	}

private:
	const SharedShaderValues* m_shaderShaderValues;
	GLint m_id;
};