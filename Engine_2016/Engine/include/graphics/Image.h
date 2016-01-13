#ifndef _IMAGE_H_
#define _IMAGE_H_
#include <core/Object.h>
#include <EASTL/vector.h>
#include <core/FileStream.h>
#include <core/Ref.h>

namespace graphics
{

class Image : public core::Object
{
public:
	Image(int width, int height, int bytesPerPixel)
		: Object()
		, m_width(width)
		, m_height(height)
		, m_bytesPerPixel(bytesPerPixel)
	{
		m_data.resize(m_width*m_height*m_bytesPerPixel);
	}
	
	uint8_t* getData()
	{
		return &m_data[0];
	}

	int getDataLenInBytes()
	{
		return (int)m_data.size();
	}

	int getWidth()
	{
		return m_width;
	}
	int getHeight()
	{
		return m_height;
	}
	int getBPP()
	{
		return m_bytesPerPixel;
	}

	virtual ~Image()
	{
	}

	static Image* loadFromTGA( const eastl::string& strFileName)
	{
		return loadFromTGA(strFileName.c_str());
	}

	static Image* loadFromTGA( const char* strFileName)
	{
		struct TARGA_HEADER
		{
			uint8_t   IDLength, ColormapType, ImageType;
			uint8_t   ColormapSpecification[5];
			uint16_t XOrigin, YOrigin;
			uint16_t ImageWidth, ImageHeight;
			uint8_t   PixelDepth;
			uint8_t   ImageDescriptor;
		};

		core::Ref<core::FileStream> s = new core::FileStream(strFileName, core::FileStream::READ_ONLY);

		static TARGA_HEADER header;
		if( sizeof(header) != s->read(&header, sizeof(header)) )
		{
			return 0;
		}
		
		Image* img = new Image(header.ImageWidth, header.ImageHeight, header.PixelDepth/8);

		if( img->getDataLenInBytes() != s->read(img->getData(), img->getDataLenInBytes()) )
		{
			delete img;
			return 0;
		}
		
		// Convert the image from BGRA to RGBA
		uint8_t* p = img->getData();
		for( int y=0; y<header.ImageHeight; y++ )
		{
			for( int x=0; x<header.ImageWidth; x++ )
			{
				uint8_t temp = p[2]; p[2] = p[0]; p[0] = temp;
				p += header.PixelDepth/8;
			}
		}

		return img;
	}
private:
	int m_width;
	int m_height;
	int m_bytesPerPixel;
	eastl::vector<uint8_t> m_data;

	Image();
};

}



#endif



