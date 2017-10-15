/**
 * @file ic_file.h
 * @author impworks.
 * ic_file header.
 * Defines properties and methods of ic_file class.
 */

#ifndef IC_FILE_H
#define IC_FILE_H

/**
 * ic_file constructor.
 */
ic_file::ic_file()
{
  mFile = NULL;
  mName = NULL;
  mMode = 0;
}

/**
 * ic_file constructor.
 * @param name Filename.
 */
ic_file::ic_file(const char *name)
{
  mFile = NULL;
  mName = new char[strlen(name)];
  strcpy(mName, name);
  mMode = 0;
}

/**
 * ic_file constructor.
 * @param name Filename.
 */
ic_file::ic_file(ic_string *name)
{
  mFile = NULL;
  mName = new char[name->length()];
  strcpy(mName, name->get());
  mMode = 0;
}

/**
 * ic_file destructor.
 */
ic_file::~ic_file()
{
  this->close();
}

/**
 * Select a new file instead of the current one.
 * @param name New file name.
 */
void ic_file::choose(ic_string *name)
{
  this->close();
  delete mName;

  mName = new char[name->length()];
  strcpy(mName, name->get());
  mMode = 0;
}

/**
 * Select a new file instead of the current one.
 * @param name New file name.
 */
void ic_file::choose(const char *name)
{
  this->close();
  delete mName;

  mName = new char[strlen(name)];
  strcpy(mName, name);
  mMode = 0;
}

/**
 * Opens a file for reading / writing.
 * @param mode File access mode.
 * @return Flag indicating success of the operation.
 */
bool ic_file::open(char mode)
{
  // check if file is already open
  if(mFile) this->close();

  char mode_str[4] = {0, 0, 0, 0};
  char idx         = 0;

  switch(mode)
  {
    case IO_READ: mode_str[0] = 'r'; break;
    case IO_WRITE: mode_str[0] = 'w'; break;
    case IO_WRITE | FILE_APPEND: mode_str[0] = 'a'; break;
    case IO_RW:
      mode_str[0] = 'r';
      mode_str[1] = '+';
      break;
    case IO_RW | FILE_REPLACE:
      mode_str[0] = 'w';
      mode_str[1] = '+';
      break;
    case IO_RW | FILE_APPEND:
      mode_str[0] = 'a';
      mode_str[1] = '+';
      break;
  }

  if(mode & FILE_BINARY)
  {
    if(mode_str[1] == 0)
      mode_str[1] = 'b';
    else
      mode_str[2] = 'b';
  }

  mFile = fopen(mName, mode_str);
  return mFile ? true : false;
}

/**
 * Closes a file.
 */
void ic_file::close()
{
  if(mFile)
  {
    fclose(mFile);
    delete mFile;
    mFile = NULL;
  }
}

/**
 * Reads bytes from file into ic_string and returns it.
 * @param size Number of bytes to read from file.
 * @return Data loaded from file or nullptr.
 */
ic_string *ic_file::read(long size)
{
  // is file open and readable?
  if(!(mMode & IO_READ)) return nullptr;

  // detect actual file size
  long tmpseek = ftell(mFile);
  fseek(mFile, 0, SEEK_END);
  long realsize = ftell(mFile);
  size          = MIN(size, realsize);
  fseek(mFile, tmpseek, 0);

  // load data from file
  ic_string *str = new ic_string();
  str->empty(size);
  fread(str->mFirst->mBuf, 1, size, mFile);
  return str;
}

/**
 * Writes bytes to file from a char buffer.
 * @param data Char buffer containing required bytes.
 * @param size Number of bytes to write to file.
 * @return Flag indicating success of the operation.
 */
bool ic_file::write(const char *data, long size)
{
  // is file open and writable?
  if(!(mMode & IO_WRITE)) return false;

  // detect filesize
  if(!size) size = strlen(data);

  long realsize = fwrite(data, 1, size, mFile);
  return realsize == size;
}

/**
 * Writes bytes to file from an ic_string.
 * @param data ic_string containing required bytes.
 * @param size Number of bytes to write to file.
 * @return Flag indicating success of the operation.
 */
bool ic_file::write(ic_string *data, long size)
{
  // is file open and writable?
  if(!(mMode & IO_WRITE)) return false;

  // detect filesize
  if(!size) size = data->length();

  long realsize = fwrite(data->get(), 1, size, mFile);
  return realsize == size;
}

/**
 * Checks if the file exists.
 * @return Flag indicating the file exists.
 */
bool ic_file::exists()
{
  // check idiotic cases
  if(mFile) return true;
  if(mName == NULL) return false;

  FILE *f = fopen(mName, "r");
  if(f)
  {
    fclose(f);
    return true;
  }
  else
    return false;
}

/**
 * Moves the internal pointer to another position in the file.
 * @param pos Desired position.
 * @param mode Seeking mode (end, start or current).
 */
void ic_file::seek(long pos, char mode)
{
  if(mFile) fseek(mFile, pos, mode);
}

/**
 * Returns current position in the file.
 * @retrun Current position of the internal pointer.
 */
inline long ic_file::tell()
{
  if(mFile)
    return ftell(mFile);
  else
    return 0;
}

/**
 * Copies a file to another location.
 * @param name New file name.
 * @return Flag indicating success of the operation.
 */
bool ic_file::copy(const char *name)
{
  FILE *from = fopen(mName, "rb");
  FILE *to   = fopen(name, "wb");
  if(!from || !to)
  {
    fclose(from);
    fclose(to);
    return false;
  }

  char buf[4096];
  while(!feof(from))
  {
    int size = fread(buf, 4096, 1, from);
    fwrite(buf, size, 1, to);
  }

  fclose(from);
  fclose(to);

  return true;
}

/**
 * Copies a file to another location.
 * @param name New file name.
 * @return Flag indicating success of the operation.
 */
inline bool ic_file::copy(ic_string *name)
{
  copy(name->get());
}

/**
 * Renames or moves a file to another location.
 * @param name New file name.
 * @return Flag indicating success of the operation.
 */
inline bool ic_file::rename(const char *name)
{
  return std::rename(mName, name) == 0;
}

/**
 * Renames or moves a file to another location.
 * @param name New file name.
 * @return Flag indicating success of the operation.
 */
inline bool ic_file::rename(ic_string *name)
{
  return std::rename(mName, name->get()) == 0;
}

/**
 * Removes the file from system.
 * @return Flag indicating success of the operation.
 */
inline bool ic_file::remove()
{
  return std::remove(mName) == 0;
}

/**
 * Chops the filesize down.
 * @param size Required size of the file.
 * @return Flag indicating success of the operation.
 */
bool ic_file::truncate(long size)
{
  FILE *from = fopen(mName, "rb");
  FILE *to   = tmpfile();

  if(!from || !to)
  {
    fclose(from);
    fclose(to);
    return false;
  }

  // check for file actually being bigger
  fseek(from, 0, SEEK_END);
  long realsize = ftell(from);
  if(realsize <= size)
  {
    fclose(from);
    fclose(to);
    return false;
  }
  else
    fseek(from, 0, SEEK_SET);

  // copy
  char buf[4096];
  int count = 0;
  while(count < size)
  {
    int bufsize = MIN(size - count, 4096);
    fread(buf, bufsize, 1, from);
    fwrite(buf, bufsize, 1, to);
    count += bufsize;
  }

  fclose(from);
  fclose(to);

  return true;
}

/**
 * Returns the length of the file.
 * @return Length of the file.
 */
long ic_file::length()
{
  long length = 0;

  if(mFile)
  {
    long curr = ftell(mFile);
    fseek(mFile, 0, SEEK_END);
    length = ftell(mFile);
    fseek(mFile, curr, SEEK_SET);
  }
  else
  {
    FILE *file = fopen(mName, "r");
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fclose(file);
  }

  return length;
}

/**
 * ic_file -> boolean converter.
 * @return bool
 */
inline bool ic_file::to_b()
{
  return exists();
}

/**
 * ic_file -> int converter.
 * @return int
 */
inline long ic_file::to_i()
{
  return length();
}

/**
 * ic_file -> char* converter.
 * @return char*
 */
inline char *ic_file::to_s() const
{
  return mName;
}

#endif
