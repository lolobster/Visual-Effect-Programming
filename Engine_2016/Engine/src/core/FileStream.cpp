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
#include <core/FileStream.h>
#include <es_assert.h>

#include <core/log.h>

#if defined(ANDROID)
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

extern struct android_app* g_androidState;

#endif

namespace core
{
	void Stream::write( const char* const str )
	{
		write(str, strlen(str));
	}

#if defined(_WIN32)
FileStream::FileStream( const char* const fileName, FileOpenMode mode )
: Stream()
, m_mode(mode)
{
	if( m_mode == READ_ONLY )
	{
		m_file = fopen(fileName, "rb");
	}
	else if( m_mode == READ_WRITE )
	{
		m_file = fopen(fileName, "wb");
	}
	else
	{
		assert( m_mode == READ_ONLY || m_mode == READ_WRITE );
	}

	if( !m_file )
	{
		LOG_ERROR("[%s] File %s could not be opened", __FUNCTION__, fileName);
		assert( m_file != 0 );
	}
}
	
FileStream::~FileStream( )
{
	if( m_file )
	{
		fclose(m_file);
	}
}

void FileStream::write( const void* p, int n )
{
	assert( m_mode == READ_WRITE );
	size_t s = fwrite(p, 1, n, m_file);
	assert( s == size_t(n) );
}

int FileStream::read( void* p, int n )
{
	assert( m_mode == READ_ONLY || m_mode == READ_WRITE );
	assert(available() >= n );
	size_t s = fread(p, 1, n, m_file);
	assert( s == size_t(n) );
	return n;
}

int	FileStream::available() const
{
	long current = ftell(m_file);
	assert(current >= 0);
	fseek(m_file, 0, SEEK_END);
	long end = ftell(m_file);
	assert(end >= 0);
	fseek(m_file, current, SEEK_SET);
	int res = end-current;
	assert(res >= 0);
	return res;
}

int	FileStream::error() const
{
	return ::ferror(m_file);
}
#endif

#if defined(ANDROID)
FileStream::FileStream( const char* const fileName, FileOpenMode mode )
: Stream()
, m_mode(mode)
{
	if( m_mode == READ_ONLY )
	{
		const int BUFFER_SIZE=255;
		char buffer[BUFFER_SIZE];
		AAssetManager* assetManager = g_androidState->activity->assetManager;
		m_file = AAssetManager_open(assetManager, fileName, AASSET_MODE_BUFFER);
	}
	else 
	{
		assert( m_mode == READ_ONLY );
	}

	if( !m_file )
	{
		LOG_ERROR("[%s] File %s could not be opened", __FUNCTION__, fileName);
		assert( m_file != 0 );
	}

	m_available = AAsset_getLength(m_file);
}
	
FileStream::~FileStream( )
{
	if( m_file )
	{
		AAsset_close(m_file);
	}
}

void FileStream::write( const void* p, int n )
{
	assert( m_mode == READ_WRITE );
	(void)p;
	(void)n;
}

int FileStream::read( void* p, int n )
{
	assert( m_mode == READ_ONLY );
	assert(available() >= n );

	long s = AAsset_read (m_file,p,n);
	assert( s == n );
	m_available -= n;
	return n;
}

int	FileStream::available() const
{
	return m_available;
}

int	FileStream::error() const
{
	return 0;
}

#endif
}



extern "C" 
{
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

#include <core/Ref.h>

typedef struct LoadF {
  int n;  /* number of pre-read characters */
  core::Ref<core::FileStream> file;  /* file being read */
  char buff[LUAL_BUFFERSIZE];  /* area for reading file */
} LoadF;


static const char *getF (lua_State *L, void *ud, size_t *size) {
  LoadF *lf = (LoadF *)ud;
  (void)L;  /* not used */
  if (lf->n > 0) {  /* are there pre-read characters to be read? */
    *size = lf->n;  /* return them (chars already in buffer) */
    lf->n = 0;  /* no more pre-read characters */
  }
  else {  /* read a block from file */
    /* 'fread' can return > 0 *and* set the EOF flag. If next call to
       'getF' called 'fread', it might still wait for user input.
       The next check avoids this problem. */
	  if ( lf->file->available() == 0 ) return NULL;
	  int rs = sizeof(lf->buff) < lf->file->available() ? sizeof(lf->buff) : lf->file->available();
	  *size = lf->file->read(lf->buff, rs );
  }
  return lf->buff;
}


static int errfile (lua_State *L, const char *what, int fnameindex) {
#if defined(_WIN32)
  const char *serr = strerror(errno);
#else
	const char *serr = "Unknown error";
#endif
  const char *filename = lua_tostring(L, fnameindex) + 1;
  lua_pushfstring(L, "cannot %s %s: %s", what, filename, serr);
  lua_remove(L, fnameindex);
  return LUA_ERRFILE;
}


static int skipBOM (LoadF *lf) {
  const char *p = "\xEF\xBB\xBF";  /* Utf8 BOM mark */
  char c;
  lf->n = 0;
  do {
	  lf->file->read(&c,1);
   // c = //getc(lf->f);
    if (c == EOF || c != *(const unsigned char *)p++) return c;
    lf->buff[lf->n++] = c;  /* to be read by the parser */
  } while (*p != '\0');
  lf->n = 0;  /* prefix matched; discard it */

  lf->file->read(&c,1);
  return c;  /* return next character */
}


/*
** reads the first character of file 'f' and skips an optional BOM mark
** in its beginning plus its first line if it starts with '#'. Returns
** true if it skipped the first line.  In any case, '*cp' has the
** first "valid" character of the file (after the optional BOM and
** a first-line comment).
*/
static int skipcomment (LoadF *lf, int *cp) {
  int c = *cp = skipBOM(lf);
  if (c == '#') {  /* first line is a comment (Unix exec. file)? */
    do {  /* skip first line */
	  lf->file->read(&c,1);
	  //     c = getc(lf->f);
    } while (c != EOF && c != '\n') ;
	
	lf->file->read(&c,1);

    *cp = c;  /* skip end-of-line, if present */
    return 1;  /* there was a comment */
  }
  else return 0;  /* no comment */
}


LUALIB_API int luaL_loadfilex (lua_State *L, const char *filename,
                                             const char *mode) {
  LoadF lf;
  int status, readstatus;
  int c;
  int fnameindex = lua_gettop(L) + 1;  /* index of filename on the stack */
  if (filename == NULL) {
	  assert(0);
 //   lua_pushliteral(L, "=stdin");
//    lf.f = stdin;
  }
  else {
    lua_pushfstring(L, "@%s", filename);
	lf.file = new core::FileStream(filename, core::FileStream::READ_ONLY);
	if (lf.file->available() == 0) return errfile(L, "open", fnameindex);
  }
  if (skipcomment(&lf, &c))  /* read initial portion */
    lf.buff[lf.n++] = '\n';  /* add line to correct line numbers */
  if (c == LUA_SIGNATURE[0] && filename) {  /* binary file? */
	  lf.file = new core::FileStream(filename, core::FileStream::READ_ONLY);
    if (lf.file->available() == 0) return errfile(L, "reopen", fnameindex);
    skipcomment(&lf, &c);  /* re-read initial portion */
  }
  if (c != EOF)
    lf.buff[lf.n++] = c;  /* 'c' is the first character of the stream */
  status = lua_load(L, getF, &lf, lua_tostring(L, -1), mode);
  readstatus = lf.file->error();
  if (filename)
	  lf.file = 0;  /* close file (even in case of errors) */
  if (readstatus) {
    lua_settop(L, fnameindex);  /* ignore results from `lua_load' */
    return errfile(L, "read", fnameindex);
  }
  lua_remove(L, fnameindex);
  return status;
}



