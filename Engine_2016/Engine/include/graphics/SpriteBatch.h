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
#ifndef SPRITEBATCH_H_
#define SPRITEBATCH_H_

#include <core/Object.h>

#include <eastl/map.h>
#include <eastl/vector.h>
#include <core/Ref.h>
#include <slmath/vec2.h>
#include <graphics/Texture.h>
#include <graphics/Shader.h>

namespace graphics
{

/**
 * Class for SpriteBatch.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class SpriteBatch : public core::Object
{
public:
	/**
	 * Resets statistics values to zero.
	 */
	static void resetStatsValues();

	/**
	 * Returns number of triangles rendered since last call to resetStatsValues, or program startup.
	 */
	static int getNumTriangles();

	/**
	 * Returns draw calls used since last call to resetStatsValues, or program startup.
	 */
	static int getNumDrawCalls();

	/**
	 * Returns number of batched sprites since last call to resetStatsValues, or program startup.
	 */
	static int getNumSpritesBatched();

	SpriteBatch();

	virtual ~SpriteBatch();

	void setTexture(Texture* texture);

	void addSprite( const slmath::vec2& cropStart, const slmath::vec2& cropSize,
		uint32_t color, const slmath::vec2& position, float rotation, float depth, 
		const slmath::vec2& scale, const slmath::vec2& postScale );

	void clear();

	void render();

	Texture* getTexture() const;

private:
	eastl::vector<float>	m_positions;
	eastl::vector<float>	m_textureCoords;
	eastl::vector<float>	m_colors;
	core::Ref< Texture>		m_texture;
	core::Ref< Shader > m_shader;
};


/**
 * Class for SpriteBatchGroup. Sprite batch group can be used to render Sprites to screen.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class SpriteBatchGroup : public core::Object
{
public:
	/** Constructs new empty sprite batch. */
	SpriteBatchGroup();

	virtual ~SpriteBatchGroup();
	
	/**
	 * Adds new sprite to batch, which is rendered using given texture to specified transform (position, rotation, scale and pivot/offset point).
	 *
	 *
	 */
	void addSprite( Texture* texture,
		const slmath::vec2& cropStart, const slmath::vec2& cropSize,
		uint32_t color, const slmath::vec2& position, float rotation = 0.0f, float depth = 0.0f,
		const slmath::vec2& scale = slmath::vec2(1.0f), const slmath::vec2& postScale = slmath::vec2(1.0f) );

	/** Clears the content of the Sprite batch. */
	void clear();

	/** Renders the content of the Sprite batch to the screen. */
	void render();

private:
	eastl::map<Texture*, core::Ref<SpriteBatch> > m_spriteBatches;

	SpriteBatch* getBatch(Texture* texture);

};

}

#endif /* SPRITEBATCH_H_ */


