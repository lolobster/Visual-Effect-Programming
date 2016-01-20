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
		shader->bind();
	}
	~GlobalShaderUniforms() {};
	
	virtual void getUniformLocations(graphics::Shader* shader) {};
	virtual void bind(graphics::Shader* shader) {};
private:
	const SharedShaderValues* m_shaderShaderValues;
};