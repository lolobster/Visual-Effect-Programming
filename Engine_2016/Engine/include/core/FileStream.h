#ifndef _FILE_STREAM_H_
#define _FILE_STREAM_H_
#include <core/Stream.h>

#include <stdio.h>

#if defined(ANDROID)
	struct AAsset;
#endif

	namespace core
{

/**
 * Stream implementation for regular file system stream.
 */
class FileStream : public Stream
{
public:
	enum FileOpenMode
	{
		READ_ONLY = 0,
		READ_WRITE = 1
	};

	/** Constructor */
	FileStream( const char* const fileName, FileOpenMode mode );
	
	/** Destructor */
	virtual ~FileStream( );

	/**
	 * Writes n bytes to the stream.
	 * @param p Data to write.
	 * @param n Number of bytes to write.
	 * @return Number of (uncompressed) bytes written.
	 */
	virtual void write( const void* p, int n );

	/**
	 * Reads n bytes from the stream.
	 * @param p [out] Receives read bytes
	 * @param n Number of bytes to read.
	 * @return Number of (uncompressed) bytes read.
	 */
	virtual int read( void* p, int n );

	/**
	 * Returns number of bytes, which can be read whitout blocking.
	 */
	virtual int	available() const;

	virtual int error() const;
	

private:
#if defined(_WIN32)
	typedef FILE* FileHandleType;
#elif defined(ANDROID)
	typedef AAsset* FileHandleType;
	long m_available;
#else
	Missing file handle type
#endif

	FileHandleType m_file;
	FileOpenMode m_mode;

	FileStream();
	FileStream( const FileStream& );
	FileStream& operator=( const FileStream& );
};

}


#endif

