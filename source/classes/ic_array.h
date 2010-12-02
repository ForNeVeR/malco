/**
 * @file ic_array.h
 * @author impworks.
 * ic_array header.
 * Defines properties and methods of ic_array class.
 */

#ifndef IC_ARRAY_H
#define IC_ARRAY_H

/**
 * ic_array constructor.
 */
ic_array::ic_array()
{
  mItems = new sc_voidmap();

  mCurrIdx = 0;
  mCurr = NULL;
  mFinished = false;
  mAutoIndex.mValue = 0;
}

/**
 * ic_array destructor.
 */
ic_array::~ic_array()
{
  delete mItems;
}

/**
 * Appends an object to the end of the array, generating a new index.
 * @param obj Object to be appended.
 */
void ic_array::append(rc_var *obj, bool check)
{
  ic_string str = mAutoIndex.to_s();
  if(check)
  {
    while(get(&str) != NULL)
    {
      mAutoIndex.mValue++;
      str = mAutoIndex.to_s();
    }
  }

  set(&str, obj);
  mAutoIndex.mValue++;
}

/**
 * Sets a specific item of the array to a new object.
 * @param key Key of the array to be modified.
 * @param obj Object to be set.
 */
inline void ic_array::set(ic_string *key, rc_var *obj)
{
  mItems->set(key, (void *)obj);
}

/**
 * Sets a specific item of the array to a new object.
 * @param key Key of the array to be modified.
 * @param obj Object to be set.
 */
inline void ic_array::set(const char *key, rc_var *obj)
{
  mItems->set(key, (void *)obj);
}

/**
 * Clears a key out.
 * @param key Key to be cleared.
 */
inline void ic_array::unset(ic_string *key)
{
  mItems->remove(key);
}

/**
 * Clears a key out.
 * @param key Key to be cleared.
 */
inline void ic_array::unset(const char *key)
{
  mItems->remove(key);
}

/**
 * Clears the whole array.
 */
void ic_array::clear()
{
  mItems->clear();

  mCurrIdx = 0;
  mCurr = NULL;
  mFinished = false;
  mAutoIndex.mValue = 0;
}

/**
 * Returns an object by it's key.
 * @param key Key of the object.
 * @return Object.
 */
inline rc_var *ic_array::get(ic_string *key)
{
  // note that ic_array is not aware of the existance of rc_head and rc_core,
  // thus being unable to automatically create an undef object if the requested
  // key is not found. it should be done in the wrapper layer instead.
  return (rc_var *)mItems->get(key);
}

/**
* Returns an object by it's key.
* @param key Key of the object.
* @return Object.
*/
inline rc_var *ic_array::get(const char *key)
{
  ic_string str = key;
  return (rc_var *)mItems->get(&str);
}

/**
 * Loads the array from another array.
 * @param arr Array to be copied.
 */
void ic_array::copy(ic_array *arr)
{
  sc_voidmapitem *curr;
  arr->iter_rewind();
  while(curr = arr->iter_next())
    mItems->set(curr->mKey, curr->mValue);
}

/**
 * Get next object from array for iteration.
 * @return Current object.
 */
inline sc_voidmapitem *ic_array::iter_next()
{
  return mItems->iter_next();
}

/**
 * Get last object from array for iteration.
 * @return Current object.
 */
inline sc_voidmapitem *ic_array::iter_last()
{
  return mItems->mLast;
}

/**
 * Rewind iterator.
 */
inline void ic_array::iter_rewind()
{
  mItems->iter_rewind();
}

/**
 * Returns number of items in the array.
 * @return Number of items in the array.
 */
inline long ic_array::length()
{
  return mItems->length();
}

/**
 * ic_array -> boolean convertor.
 * @return bool
 */
inline bool ic_array::to_b()
{
  return mItems->length() ? true : false;
}

/**
 * ic_array -> int convertor.
 * @return long
 */
inline long ic_array::to_i()
{
  return mItems->length();
}

/**
 * ic_array -> string convertor.
 * @return char*
 */
inline const char *ic_array::to_s()
{
  return "Array";
}

#endif
