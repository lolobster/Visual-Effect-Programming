#pragma once

#include "graphics\Shader.h"


struct SharedShaderValues
{
	float totalTime;
	slmath::mat4 matModelViewProj; // Model view projection matrix. Used to transform position vertices to clip scape.
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
		m_id2 = glGetUniformLocation(shader->getProgram(), "g_matModelViewProj");
	}

	virtual void bind(graphics::Shader* shader) {
		shader->bind();

		if (m_shaderShaderValues)
		{

			glUniformMatrix4fv(m_id2, 1, GL_FALSE, &m_shaderShaderValues->matModelViewProj[0][0]);
			
			glUniform1f(m_id, m_shaderShaderValues->totalTime);
		}
	}

private:
	const SharedShaderValues* m_shaderShaderValues;
	GLint m_id;
	GLint m_id2;

};