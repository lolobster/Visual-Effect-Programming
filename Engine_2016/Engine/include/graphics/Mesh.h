#ifndef _ENGINE_MESH_H_
#define _ENGINE_MESH_H_
#include <core/Ref.h>
#include <slmath/mat4.h>
#include <graphics/OpenGLES/es_util.h>
#include <core/log.h>
#include <core/FileStream.h>

namespace graphics
{



class IndexBuffer : public core::Object
{
public:
	IndexBuffer( const eastl::vector<uint16_t>& data)
	{
		m_dataLen = data.size()*sizeof(uint16_t);
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_dataLen, &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer(uint16_t* data, int dataLen)
		: Object()
		, m_dataLen(dataLen)
	{
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataLen*sizeof(uint16_t) , data, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void drawElements()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glDrawElements(GL_TRIANGLES, m_dataLen, GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	virtual ~IndexBuffer()
	{
		glDeleteBuffers(1, &m_ibo);
	}
	
private:
	GLuint m_ibo;
	int m_dataLen;
//	uint16_t* m_data;
};



class VertexArray : public core::Object
{
public:
	VertexArray()
	{
	}

	virtual ~VertexArray()
	{
	}
	
	virtual SHADER_ATTRIBUTES getSemantic() const = 0;
	virtual int getVertexCount() const = 0;
	virtual int getStride() const = 0;
	virtual int getComponentCount() const = 0;
	virtual const float* getVertex(int index) const = 0;
};

template <class T>
class VertexArrayImpl : public VertexArray
{
public:
	VertexArrayImpl(SHADER_ATTRIBUTES semantic, T* data, int vertexCount)
	{
		m_semantic = semantic;
		m_data.resize(vertexCount);
		for( int i=0; i<vertexCount; ++i )
		{
			m_data[i] = data[i];
		}
	}

	VertexArrayImpl(SHADER_ATTRIBUTES semantic, const eastl::vector<T>& data)
	{
		m_semantic = semantic;
		m_data = data;
	}

	virtual ~VertexArrayImpl()
	{
	}
	
	virtual SHADER_ATTRIBUTES getSemantic() const
	{
		return m_semantic;
	}

	virtual int getVertexCount() const
	{
		return (int)m_data.size();
	}

	virtual int getStride() const
	{
		return sizeof(T);
	}

	virtual int getComponentCount() const
	{
		return getStride() / sizeof(float);
	}

	virtual const float* getVertex(int index) const
	{
		return (const float*)&m_data[index];
	}


private:
	SHADER_ATTRIBUTES m_semantic;
	eastl::vector<T> m_data;
};


class VertexBuffer : public core::Object
{
public:
	VertexBuffer( VertexArray** vertexArrays, int count )
		: Object()
	{
		m_vertexArrays.resize(count);
	
		assert( count > 0 );
		m_componentsPerVertex = 0;
		int numOfVertices = vertexArrays[0]->getVertexCount();
		for(int vaIndex=0; vaIndex<count; ++vaIndex )
		{
			VertexArray* va = vertexArrays[vaIndex];
			m_vertexArrays[vaIndex] = va;
			assert(va->getVertexCount() == numOfVertices );
			m_componentsPerVertex += va->getComponentCount(); 
		}

		m_data.resize(m_componentsPerVertex * numOfVertices);

		for( int vertexIndex = 0; vertexIndex<numOfVertices; ++vertexIndex )
		{
			float* destVertex = &m_data[vertexIndex*m_componentsPerVertex];

			int offs = 0;
			for(int vaIndex=0; vaIndex<count; ++vaIndex )
			{
				VertexArray* va = m_vertexArrays[vaIndex];
				const float* srcVertex = va->getVertex(vertexIndex);
				memcpy(&destVertex[offs], srcVertex, va->getStride() );
				offs += va->getComponentCount();
			}
		}

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_data.size()*sizeof(float),&m_data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	virtual ~VertexBuffer()
	{
	}

	void bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		int offs = 0;
		for( size_t i=0; i<m_vertexArrays.size(); ++i )
		{
			VertexArray* va = m_vertexArrays[i];
			//assert(!isEnabled(va->getSemantic()));
			glEnableVertexAttribArray(va->getSemantic());
			assert(isEnabled(va->getSemantic()));
			glVertexAttribPointer(va->getSemantic(), va->getComponentCount(), GL_FLOAT, GL_FALSE, m_componentsPerVertex*sizeof(float), BUFFER_OFFSET(offs*sizeof(GLfloat)));
						
			offs += va->getComponentCount();
		}
	}

	void unbind()
	{
		for( size_t i=0; i<m_vertexArrays.size(); ++i )
		{
			VertexArray* va = m_vertexArrays[i];
			//assert(isEnabled(va->getSemantic()));
			glDisableVertexAttribArray(va->getSemantic());
			assert(!isEnabled(va->getSemantic()));
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

private:
	bool isEnabled(int i)
	{
		GLint numAttribs;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttribs );
		assert(i<numAttribs);
		GLint params;
		glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &params);  
		return params ? true:false;
	}

	GLuint m_vbo;
	eastl::vector< core::Ref<VertexArray> > m_vertexArrays;
	eastl::vector<float> m_data;
	int m_componentsPerVertex;
};

class Mesh : public core::Object
{
public:
	Mesh(IndexBuffer* ib, VertexBuffer* vb)
		: Object()
		, m_ib(ib)
		, m_vb(vb)
	{
	}
	
	virtual ~Mesh()
	{
	}

	void render()
	{
		m_vb->bind();
		m_ib->drawElements();
		m_vb->unbind();
	}

	static Mesh* loadFromOBJ(const char* const fileName)
	{
		// Read file data to lines
		eastl::string str = "";
		{
			core::Ref<core::Stream> s = new core::FileStream(fileName, core::FileStream::READ_ONLY);
			assert( s->available() > 0 );
			eastl::vector<char> fileData;
			fileData.resize( s->available()+1 );
			fileData[fileData.size()-1] = 0;
			int numRead = s->read(&fileData[0],s->available());
			assert(numRead==(fileData.size()-1));
			str += (const char*)&fileData[0];
		}

		eastl::vector<eastl::string> lines = str.split('\n');
		
		// Parse obj file data to these obj* -arrays
		eastl::vector<slmath::vec3> objPositions;
		eastl::vector<slmath::vec3> objNormals;
		eastl::vector<slmath::vec2> objTexCoords;
		eastl::vector<Face>			objFaces;
		int objMaxIndex=0;
		bool hasNormals = false;
		bool hasTexCoords = false;
		{			
			for( size_t i=0; i<lines.size(); ++i )
			{
				const char trimChars[] = "\t\n\r ";
				eastl::string line = lines[i];
				line.rtrim(trimChars);
				line.ltrim(trimChars);
				if( line.length() < 1 || line.starts_with("#") )
				{
					continue; // Empty or Comment line, continue parsing
				}
	
				if( line.starts_with("vt") )
				{
					objTexCoords.push_back( parseVec2(line) );
					hasTexCoords = true;
				} 
				else if( line.starts_with("vn") )
				{
					objNormals.push_back( parseVec3(line) );
					hasNormals = true;
				} 
				else if( line.starts_with("v") )
				{
					objPositions.push_back( parseVec3(line) );
				}
				/*else if( line.starts_with("g") )
				{
				} */
				else if( line.starts_with("f ") )
				{ 
					eastl::vector<Face> faces = parseFace(line);
					for( size_t i=0; i<faces.size(); ++i )
					{
						for( int j=0; j<3; ++j )
						{
							if(faces[i].vertices[j].positionIndex > objMaxIndex )
								objMaxIndex = faces[i].vertices[j].positionIndex;
						}

						objFaces.push_back(faces[i]);
					}

					
				}
				else
				{
					LOG_WARNING("%s(%d): Unknown Line parsed: %s", fileName, i, line.c_str() ); 
				}
			}
		}
				
		eastl::vector<uint16_t> indices;
		eastl::vector<slmath::vec3> positions;
		eastl::vector<slmath::vec3> normals;
		eastl::vector<slmath::vec3> texCoords;
		positions.resize(objMaxIndex);
		if( hasNormals ) normals.resize(objMaxIndex);
		if( hasTexCoords ) texCoords.resize(objMaxIndex);

		// Convert data from obj* to engine arrays
		{
			// Go throught each face and add correct values to 
			for( size_t i=0; i<objFaces.size(); ++i )
			{
				for( int j=0; j<3; ++j )
				{
					Int3 objIndex = objFaces[i].vertices[j];
					assert(objIndex.positionIndex > 0 );
					assert( (objIndex.positionIndex-1) < (int)objPositions.size() );
					slmath::vec3 pos = objPositions[objIndex.positionIndex-1];
					indices.push_back(objIndex.positionIndex-1); // Use index from position
					positions[objIndex.positionIndex-1] = pos;

					if( objIndex.normalIndex > 0 )
					{
						slmath::vec3 normal = objNormals[objIndex.normalIndex-1];
						normals[objIndex.positionIndex-1] = normal;
					}

					if( objIndex.texCoordIndex > 0 )
					{
						slmath::vec2 texCoord = objTexCoords[objIndex.texCoordIndex-1];
						texCoords[objIndex.positionIndex-1] = slmath::vec3(texCoord,0.0f);
					}
				}
			}
		}

	//	const eastl::vector<slmath::vec3>& vertex, 
	// const eastl::vector<slmath::vec3>& normal,
	//	const eastl::vector<slmath::vec2>& texcoord,
	// const eastl::vector<unsigned int>& triangles, 
	// eastl::vector<slmath::vec4> &tangent



		//Create Index buffer
		IndexBuffer* ib = new IndexBuffer(indices); 
		
		eastl::vector<VertexArray*> vertexArrays;
		
		vertexArrays.push_back( new VertexArrayImpl<slmath::vec3>(ATTRIB_POSITION, positions) );

		if( hasTexCoords )
		{
			vertexArrays.push_back( new VertexArrayImpl<slmath::vec3>(ATTRIB_UV, texCoords) );
		}

		if( hasNormals )
		{
			vertexArrays.push_back( new VertexArrayImpl<slmath::vec3>(ATTRIB_NORMAL, normals) );
		}
		
		// Binormals & tangents
		if( hasNormals &&  hasTexCoords )
		{
			eastl::vector<slmath::vec3> tangents;
			eastl::vector<slmath::vec3> binormals;
			calculateTangentArray(positions,normals,texCoords,indices, &tangents, &binormals );
			vertexArrays.push_back( new VertexArrayImpl<slmath::vec3>(ATTRIB_BINORMAL, binormals) );
			vertexArrays.push_back( new VertexArrayImpl<slmath::vec3>(ATTRIB_TANGENT, tangents) );
		}

		// Create vertex buffer from vertex arrays
		VertexBuffer* vb = new VertexBuffer(&vertexArrays[0],vertexArrays.size());
		
		// Create mesh from ib and vb
		return new Mesh(ib,vb);
	}

private:

	static slmath::vec3 parseVec3(const eastl::string& str)
	{
		eastl::vector<eastl::string> lines = str.split(' ');

		int lineCount = lines.size();
		assert( lineCount == 4 );
		slmath::vec3 res;
		sscanf(lines[1].c_str(), "%f", &res.x );
		sscanf(lines[2].c_str(), "%f", &res.y );
		sscanf(lines[3].c_str(), "%f", &res.z );
		assert(slmath::check(res));
		return res;
	}

	static slmath::vec2 parseVec2(const eastl::string& str)
	{
		eastl::vector<eastl::string> lines = str.split(' ');

		int lineCount = lines.size();
		assert( lines.size() == 3 );
		slmath::vec2 res;
		sscanf(lines[1].c_str(), "%f", &res.x );
		sscanf(lines[2].c_str(), "%f", &res.y );
		assert(slmath::check(res));
		return res;
	}

	struct Int3 
	{
		int positionIndex;
		int texCoordIndex;
		int normalIndex;
	};

	
	static Int3 parseInt3(const eastl::string& str)
	{
		eastl::vector<eastl::string> lines = str.split('/');
		assert(lines.size()==2 || lines.size() == 3); // Trianlge expected

		Int3 res;
		res.positionIndex = -1;
		res.texCoordIndex = -1;
		res.normalIndex = -1;

		if( lines.size() >= 2 )
		{
			// Position, text coord (maybe)
			sscanf(lines[0].c_str(), "%d", &res.positionIndex );

			if( lines[1].length() > 0 )
				sscanf(lines[1].c_str(), "%d", &res.texCoordIndex );
		} 
		
		if( lines.size() == 3 )
		{
			// Normal (maybe)
			if( lines[2].length() > 0 )
				sscanf(lines[2].c_str(), "%d", &res.normalIndex );
		}

		assert(res.positionIndex > 0);
		return res;
	}

	struct Face 
	{
		Int3 vertices[3];
	};


	static eastl::vector<Face> parseFace(const eastl::string& str)
	{
		eastl::vector<eastl::string> lines = str.split(' ');
		assert(lines.size() == 4 || lines.size() == 5); // Trianlge expected
		eastl::vector<Face> res;

		if( lines.size() == 4 )
		{
			res.resize(1);
			res[0].vertices[0] = parseInt3(lines[1]);
			res[0].vertices[1] = parseInt3(lines[2]);
			res[0].vertices[2] = parseInt3(lines[3]);
		}
		else
		{
			res.resize(2);
			res[0].vertices[0] = parseInt3(lines[1]);
			res[0].vertices[1] = parseInt3(lines[2]);
			res[0].vertices[2] = parseInt3(lines[3]);

			res[1].vertices[0] = parseInt3(lines[1]);
			res[1].vertices[1] = parseInt3(lines[3]);
			res[1].vertices[2] = parseInt3(lines[4]);
		}

		return res;
	}

	

	static void calculateTangentArray(const eastl::vector<slmath::vec3>& vertex, const eastl::vector<slmath::vec3>& normal,
			const eastl::vector<slmath::vec3>& texcoord, const eastl::vector<uint16_t>& triangles, eastl::vector<slmath::vec3>* tangents, eastl::vector<slmath::vec3>* binormals)
	{
		int vertexCount = vertex.size();
		int triangleCount = triangles.size()/3;
		eastl::vector<slmath::vec3> tan1;
		tan1.resize(vertexCount * 2, slmath::vec3(0.0f) );


		slmath::vec3 *tan2 = &tan1[0] + vertexCount;

		for (long a = 0; a <triangleCount; a++)
		{
			unsigned int i1 = triangles[a*3+0];
			unsigned int i2 = triangles[a*3+1];
			unsigned int i3 = triangles[a*3+2];

			const slmath::vec3& v1 = vertex[i1];
			const slmath::vec3& v2 = vertex[i2];
			const slmath::vec3& v3 = vertex[i3];

			const slmath::vec2& w1 = texcoord[i1].xy();
			const slmath::vec2& w2 = texcoord[i2].xy();
			const slmath::vec2& w3 = texcoord[i3].xy();

			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;

			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;

			float r = 1.0F / (s1 * t2 - s2 * t1);
			slmath::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
					(t2 * z1 - t1 * z2) * r);
			slmath::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
					(s1 * z2 - s2 * z1) * r);

			tan1[i1] += sdir;
			tan1[i2] += sdir;
			tan1[i3] += sdir;

			tan2[i1] += tdir;
			tan2[i2] += tdir;
			tan2[i3] += tdir;
		}

		for (long a = 0; a < vertexCount; a++)
		{
			const slmath::vec3& n = normalize(normal[a]);
			const slmath::vec3& t = tan1[a];

			// Gram-Schmidt orthogonalize
			slmath::vec3 temp = slmath::normalize(t - n * slmath::dot(n, t));


			// Calculate handedness
			float w = (slmath::dot(slmath::cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
			slmath::vec3 tangent = temp * w;
			tangents->push_back(tangent);
			binormals->push_back( slmath::cross(tangent,-n) );
		}
	}


	core::Ref<IndexBuffer> m_ib;
	core::Ref<VertexBuffer> m_vb;
};


}


#endif

