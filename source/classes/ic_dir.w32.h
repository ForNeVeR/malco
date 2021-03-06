/**
 * @file ic_dir.w32.h
 * @author impworks
 * Platform-specific method definitions for ic_dir class.
 */

/**
 * Recursively copies items in the directory.
 * @param from Source path.
 * @param to Destination path.
 * @return Success flag.
 */
bool ic_dir::copy_r(const char *from, const char *to)
{
  return true;
}

/**
 * Recursively removes items in the directory.
 * @param name Directory path.
 * @return Success flag.
 */
bool ic_dir::remove_r(const char *name)
{
  return true;
}

/**
 * Copies directory to another place.
 * @param name Destination path.
 * @return Success flag.
 */
bool ic_dir::copy(const char *name)
{
  return true;
}

/**
 * Copies directory to another place.
 * @param name Destination path.
 * @return Success flag.
 */
bool ic_dir::copy(ic_string *name)
{
  return copy(name->get());
}

/**
 * Moves directory to another place.
 * @param name Destination path.
 * @return Success flag.
 */
bool ic_dir::rename(const char *name)
{
  return true;
}

/**
 * Moves directory to another place.
 * @param name Destination path.
 * @return Success flag.
 */
bool ic_dir::rename(ic_string *name)
{
  return rename(name->get());
}

/**
 * Creates the directory if it doesn't exist.
 * @return Success flag.
 */
bool ic_dir::create()
{
  return true;
}

/**
 * Removes the directory if it exists.
 * @return Success flag.
 */
bool ic_dir::remove()
{
  return true;
}

/**
 * Returns the number of items in the directory.
 * @return Count of items in the directory.
 */
long ic_dir::length()
{
  return 0;
}

/**
 * Returns another item from the dir.
 * @return Current item's name.
 */
ic_string *ic_dir::iter_next()
{
  return NULL;
}
 
/**
 * Rewinds the directory iterator,
 */
void ic_dir::iter_rewind()
{

}