/**
 * @file sc_file.h
 * @author impworks.
 * sc_file header.
 * Defines properties and methods of sc_file class.
 */

#ifndef SC_FILE_H
#define SC_FILE_H

char sc_file::mStrBuf[16] = "               ";

/**
 * Determines file's modification time.
 * @param name Name of the file.
 */
long sc_file::time(const char *name)
{
  struct stat tmp;
  if(stat(name, &tmp))
    return (long)tmp.st_mtime;
  else
    return 0;
}

/**
 * Determines file's modification time.
 * @param name Name of the file.
 */
inline long sc_file::time(ic_string *name)
{
  return sc_file::time(name->get());
}

/**
 * Checks if file exists or not.
 * @param name Name of the file.
 */
inline bool sc_file::exists(const char *name)
{
  FILE *f = fopen(name, "r");
  if(f)
  {
    fclose(f);
    return true;
  }
  else
    return false;
}

/**
 * Checks if file exists or not.
 * @param name Name of the file.
 */
inline bool sc_file::exists(ic_string *name)
{
  return sc_file::exists(name->get());
}

/**
 * Renames a file from old name to new one.
 * @param oldname Current name of the file.
 * @param newname Desired name of the file.
 */
inline bool sc_file::rename(const char *oldname, const char *newname)
{
  return (rename(oldname, newname) == 0);
}

/**
 * Renames a file from old name to new one.
 * @param oldname Current name of the file.
 * @param newname Desired name of the file.
 */
inline bool sc_file::rename(ic_string *oldname, ic_string *newname)
{
  return (rename(oldname->get(), newname->get()) == 0);
}

/**
 * Removes a file.
 * @param name Name of the file.
 */
inline bool sc_file::remove(const char *name)
{
  return (remove(name) == 0);
}

/**
 * Removes a file.
 * @param name Name of the file.
 */
inline bool sc_file::remove(ic_string *name)
{
  return (remove(name->get()) == 0);
}

/**
 * Calculates a checksum from the file name and it's last modification date.
 * @param name Name of the file.
 */
char *sc_file::checksum(const char *name)
{
  ic_int mtime = sc_file::time(name);
  ic_string str = mtime.to_s();
  sc_md5 md5;

  str.append(name);
  strncpy(sc_file::mStrBuf, md5.string(&str), 15);
  return sc_file::mStrBuf;
}

/**
 * Calculates a checksum from the file name and it's last modification date.
 * @param name Name of the file.
 */
char *sc_file::checksum(ic_string *name)
{
  ic_int mtime = sc_file::time(name);
  ic_string str = mtime.to_s();
  sc_md5 md5;

  str.append(name);
  strncpy(sc_file::mStrBuf, md5.string(&str), 15);
  return sc_file::mStrBuf;
}



#endif
