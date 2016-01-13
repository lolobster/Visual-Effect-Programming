#ifndef _STREAM_H_
#define _STREAM_H_

#include <core/Object.h>

namespace core
{

/**
 * Interface for general input/output stream.
 */
class Stream : public Object
{
public:
	
	/** Destructor */
	virtual ~Stream() {};

	/**
	 * Writes n bytes to the stream.
	 * @param p Data to write.
	 * @param n Number of bytes to write.
	 * @return Number of (uncompressed) bytes written.
	 */
	virtual void write( const void* p, int n ) = 0;
	void write( const char* const str );

	/**
	 * Reads n bytes from the stream.
	 * @param p [out] Receives read bytes
	 * @param n Number of bytes to read.
	 * @return Number of (uncompressed) bytes read.
	 */
	virtual int read( void* p, int n ) = 0;

	/**
	 * Returns number of bytes, which can be read whitout blocking.
	 */
	virtual int	available() const = 0;

protected:
	Stream() {};

private:
	Stream( const Stream& );
	Stream& operator=( const Stream& );
};

}

#endif

