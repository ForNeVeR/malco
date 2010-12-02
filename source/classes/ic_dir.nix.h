/**
 * @file ic_dir.nix.h
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

}

/**
 * Recursively removes items in the directory.
 * @param name Directory path.
 * @return Success flag.
 */
bool ic_dir::remove_r(const char *name)
{

}

/**
 * Copies directory to another place.
 * @param name Destination path.
 * @return Success flag.
 */
bool ic_dir::copy(const char *name)
{

}

/**
 * Copies directory to another place.
 * @param name Destination path.
 * @return Success flag.
 */
bool ic_dir::copy(ic_string *name)
{
  copy(name->get());
}

/**
 * Moves directory to another place.
 * @param name Destination path.
 * @return Success flag.
 */
bool ic_dir::rename(const char *name)
{

}

/**
 * Moves directory to another place.
 * @param name Destination path.
 * @return Success flag.
 */
bool ic_dir::rename(ic_string *name)
{
  rename(name->get());
}

/**
 * Creates the directory if it doesn't exist.
 * @return Success flag.
 */
bool ic_dir::create()
{

}

/**
 * Removes the directory if it exists.
 * @return Success flag.
 */
bool ic_dir::remove()
{

}

/**
 * Returns the number of items in the directory.
 * @return Count of items in the directory.
 */
long ic_dir::length()
{

}

/**
 * Returns another item from the dir.
 * @return Current item's name.
 */
ic_string *ic_dir::iter_next()
{

}
 
/**
 * Rewinds the directory iterator,
 */
void ic_dir::iter_rewind()
{

}