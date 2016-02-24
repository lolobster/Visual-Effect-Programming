#pragma once

#include "graphics\Shader.h"


struct SharedShaderValues
{
	float totalTime;
	slmath::mat4 matModelViewProj;	// Model view projection matrix. Used to transform position vertices to clip scape.
	slmath::mat4 matView;			// View matrix (inverse of camera world matrix)
	slmath::mat4 matProj;			// Projision matrix of the camera
	slmath::mat4 matModel;			// Model matrix (object world matrix)
	slmath::mat4 matModelView;		// Model view matrix. Used to transform normal/binormal/tangent vertices to camera space
	slmath::mat4 matNormal;			// Model view matrix. Used to transform normal/binormal/tangent vertices to camera space
	slmath::vec3 lightPos;			// World position of point light
	slmath::vec3 camPos;			// World position of camera
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
		m_id[0] = glGetUniformLocation(shader->getProgram(), "totalTime");
		m_id[1] = glGetUniformLocation(shader->getProgram(), "g_matModel");
		m_id[2] = glGetUniformLocation(shader->getProgram(), "g_matView");
		m_id[3] = glGetUniformLocation(shader->getProgram(), "g_matProj");
		m_id[4] = glGetUniformLocation(shader->getProgram(), "g_matModelView");
		m_id[5] = glGetUniformLocation(shader->getProgram(), "g_matNormal");
		m_id[6] = glGetUniformLocation(shader->getProgram(), "g_matModelViewProj");
		m_id[7] = glGetUniformLocation(shader->getProgram(), "g_lightPos");
		m_id[8] = glGetUniformLocation(shader->getProgram(), "g_camPos");
	}

	virtual void bind(graphics::Shader* shader) {
		shader->bind();

		if (m_shaderShaderValues)
		{
			glUniform1f(m_id[0], m_shaderShaderValues->totalTime);
			glUniformMatrix4fv(m_id[1], 1, GL_FALSE, &m_shaderShaderValues->matModel[0][0]);
			glUniformMatrix4fv(m_id[2], 1, GL_FALSE, &m_shaderShaderValues->matView[0][0]);
			glUniformMatrix4fv(m_id[3], 1, GL_FALSE, &m_shaderShaderValues->matProj[0][0]);
			glUniformMatrix4fv(m_id[4], 1, GL_FALSE, &m_shaderShaderValues->matModelView[0][0]);
			glUniformMatrix4fv(m_id[5], 1, GL_FALSE, &m_shaderShaderValues->matNormal[0][0]);
			glUniformMatrix4fv(m_id[6], 1, GL_FALSE, &m_shaderShaderValues->matModelViewProj[0][0]);

			glUniform3f(m_id[7], m_shaderShaderValues->lightPos.x, m_shaderShaderValues->lightPos.y, m_shaderShaderValues->lightPos.z);
			glUniform3f(m_id[8], m_shaderShaderValues->camPos.x, m_shaderShaderValues->camPos.y, m_shaderShaderValues->camPos.z);
		}
	}

private:
	const SharedShaderValues* m_shaderShaderValues;
	GLint m_id[9];

};