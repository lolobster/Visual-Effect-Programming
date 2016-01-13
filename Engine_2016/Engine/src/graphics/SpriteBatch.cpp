// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// http://code.google.com/p/yam2d/
//
// Copyright (c) 2013 Mikko Romppainen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in the
// Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <graphics/SpriteBatch.h>

namespace graphics
{


// anonymous namespace for internal functions
namespace
{
	int numTriangles = 0;
	int numDrawCalls = 0;
	int numSpritesBatched = 0;
	
	void swap(float& v1, float& v2)
	{
		float temp = v1;
		v1 = v2;
		v2 = temp;
	}


	void createSpriteVertexData(const slmath::vec2& cropStart, const slmath::vec2& cropSize, float depth, uint32_t color,
		eastl::vector<float>& verts, eastl::vector<float>& texCoords, eastl::vector<float>& colors )
	{
		{
			size_t startIndex = verts.size();
			static const float S = 0.5f;
			static const size_t DS = 3*6;
			float v [DS] = {
				-S,		-S,		depth,
				 S,		 S,		depth,
				-S,		 S,		depth,
				-S,		-S,		depth,
				 S,		-S,		depth,
				 S,		 S,		depth
			};

			verts.resize( verts.size()+DS );
			memcpy(&verts[startIndex], &v, sizeof(v) );
		}

		{
			size_t startIndex = texCoords.size();
			static const size_t DS = 2*6;
			float v [DS] = {
				cropStart.x,				(cropStart.y),
				cropStart.x+cropSize.x,		(cropStart.y+cropSize.y),
				cropStart.x,				(cropStart.y+cropSize.y),
				cropStart.x,				(cropStart.y),
				cropStart.x+cropSize.x,		(cropStart.y),
				cropStart.x+cropSize.x,		(cropStart.y+cropSize.y),
			};


			texCoords.resize( texCoords.size()+DS );
			memcpy(&texCoords[startIndex], &v, sizeof(v) );
		}

		for( int i=0; i<6; ++i )
		{
			size_t startIndex = colors.size();
			colors.resize( colors.size()+ (4*4) );
			for( ; startIndex<colors.size(); startIndex+=4 )
			{
				static const float MUL = 1.0f/255.0f;
				colors[startIndex + 0] = MUL * float((color&0xff000000)>>24);	// r
				colors[startIndex + 1] = MUL * float((color&0x00ff0000)>>16);	// g
				colors[startIndex + 2] = MUL * float((color&0x0000ff00)>>8);	// b
				colors[startIndex + 3] = MUL * float((color&0x000000ff));		// a
			}
		}
	}


}


void SpriteBatch::resetStatsValues()
{
	numTriangles = 0;
	numDrawCalls = 0;
	numSpritesBatched = 0;
}


int SpriteBatch::getNumTriangles()
{
	return numTriangles;
}


int SpriteBatch::getNumDrawCalls()
{
	return numDrawCalls;
}


int SpriteBatch::getNumSpritesBatched()
{
	return numSpritesBatched;
}


SpriteBatch::SpriteBatch()
: m_positions()
, m_textureCoords()
, m_texture(0)
{
	FRM_SHADER_ATTRIBUTE attributes[] = 
	{
			{"g_vPositionOS", 0 },
			{"g_vTexCoordOS", 1 },
			{"g_vColor", 2 }
	};
	
	char spriteVS[] = 
		"attribute vec3	g_vPositionOS;\n"
		"attribute vec2	g_vTexCoordOS;\n"
		"attribute vec4	g_vColor;\n"
		"varying vec2   g_texCoordOS;\n"
		"varying vec4    g_color\n;"
		"\n"
		"void main()\n"
		"{\n"
		"   gl_Position  = vec4(g_vPositionOS, 1.0);\n"
		"   g_color  = g_vColor;\n"
		"	g_texCoordOS = g_vTexCoordOS;\n"
		"}\n";

	char spriteFS[] = 
		"// default to medium precision\n"
		"precision mediump float;\n"
		"\n"
		"// OpenGL ES require that precision is defined for a fragment shader\n"
		"// usage example: varying NEED_HIGHP vec2 vLargeTexCoord;\n"
		"#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
		"   #define NEED_HIGHP highp\n"
		"#else\n"
		"   #define NEED_HIGHP mediump\n"
		"#endif\n"
		"\n"
		"\n"
		"varying vec2    g_texCoordOS;\n"
		"varying vec4    g_color;\n"
		"\n"
		"uniform sampler2D s_diffuseMap;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_FragColor = texture2D( s_diffuseMap, g_texCoordOS.xy );\n"
		"}\n";


#if 0
	// From file
	m_shader = new Engine::Shader("assets/Sprite.vs", "assets/Sprite.fs", attributes,sizeof(attributes)/sizeof(FRM_SHADER_ATTRIBUTE));
#else
	// From strings
	m_shader = new Shader(spriteVS, spriteFS, attributes,sizeof(attributes)/sizeof(FRM_SHADER_ATTRIBUTE), false);
#endif
}


SpriteBatch::~SpriteBatch()
{
}


void SpriteBatch::setTexture(Texture* texture)
{
	m_texture = texture;
}


void SpriteBatch::addSprite( const slmath::vec2& cropStart, const slmath::vec2& cropSize, uint32_t color, const slmath::vec2& position, float rotation, float depth, const slmath::vec2& scale, const slmath::vec2& postScale )
{
	size_t start = m_positions.size();
	size_t startTex = m_textureCoords.size();

	createSpriteVertexData(cropStart, cropSize,depth, color, m_positions, m_textureCoords, m_colors);

	// If negative scale (rotate triangles winding order)
	if( scale.x*scale.y < 0.0f )
	{
		for(size_t i=start; i<m_positions.size(); i += 2*3*3 )
		{
			swap( m_positions[i+ 0], m_positions[i+ 3] );
			swap( m_positions[i+ 1], m_positions[i+ 4] );
			swap( m_positions[i+ 2], m_positions[i+ 5] );

			swap( m_positions[i+ 9], m_positions[i+12] );
			swap( m_positions[i+10], m_positions[i+13] );
			swap( m_positions[i+11], m_positions[i+14] );
		}

		for(size_t i=startTex; i<m_textureCoords.size(); i += 2*3*2 )
		{
			swap( m_textureCoords[i+ 0], m_textureCoords[i+ 2] );
			swap( m_textureCoords[i+ 1], m_textureCoords[i+ 3] );

			swap( m_textureCoords[i+ 6], m_textureCoords[i+ 8] );
			swap( m_textureCoords[i+ 7], m_textureCoords[i+ 9] );
		}
	}

	for( ; start<m_positions.size(); start += 3 )
	{
		float x0 =  (m_positions[start+0]*scale.x);
		float y0 = (m_positions[start+1]*scale.y);
		
		float sinAngle = sinf(rotation);
		float cosAngle = cosf(rotation);

		// Rotation
		float x = x0*cosAngle - y0*sinAngle;
		float y = x0*sinAngle + y0*cosAngle;

		// Translation
		m_positions[start+0] = postScale.x * (position.x + x);
		m_positions[start+1] = postScale.y * (position.y + y);
		m_positions[start+2] = m_positions[start+2];
	}

	++numSpritesBatched;
}


void SpriteBatch::clear()
{
	m_positions.clear();
	m_textureCoords.clear();
	m_colors.clear();
}


void SpriteBatch::render()
{
	if( m_positions.size() == 0 )
		return;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, 0, &m_positions[0]);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, 0, &m_textureCoords[0]);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, FALSE, 0, &m_colors[0]);
		
	m_shader->bind();


	if( m_texture )
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, m_texture->getTextureId());

		int loc = glGetUniformLocation(m_shader->getProgram(),"s_diffuseMap");
		// Set sampler unit 0 to be used as sampler for diffuse map uniform.
		glUniform1i(loc, 0);
	}
	

	numTriangles += m_positions.size()/3;
	++numDrawCalls;
	glDrawArrays(GL_TRIANGLES, 0, m_positions.size()/3);


	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(0);

}

Texture* SpriteBatch::getTexture() const
{
	return m_texture.ptr();
}




SpriteBatchGroup::SpriteBatchGroup()
{
}


SpriteBatchGroup::~SpriteBatchGroup()
{
}

	
void SpriteBatchGroup::addSprite(Texture* texture, const slmath::vec2& cropStart, const slmath::vec2& cropSize, uint32_t color, const slmath::vec2& position, float rotation, float depth, const slmath::vec2& scale, const slmath::vec2& postScale )
{
	getBatch(texture)->addSprite(cropStart,cropSize,color,position,rotation,depth,scale,postScale);
}



void SpriteBatchGroup::clear()
{
	for (eastl::map<Texture*, core::Ref<SpriteBatch> >::iterator it = m_spriteBatches.begin(); it != m_spriteBatches.end(); ++it)
	{
		SpriteBatch* batch = it->second;
		batch->clear();
	}
}


void SpriteBatchGroup::render()
{
	for( eastl::map<Texture*, core::Ref<SpriteBatch> >::iterator it = m_spriteBatches.begin(); it != m_spriteBatches.end(); ++it )
	{
		SpriteBatch* batch = it->second;
		batch->render();
	}
}


SpriteBatch* SpriteBatchGroup::getBatch(Texture* texture)
{
	SpriteBatch* batch = m_spriteBatches[texture];
	if( batch == 0 )
	{
		batch = new SpriteBatch();
		batch->setTexture(texture);
		m_spriteBatches[texture] = batch;
	}
	
	return batch;
}


}

