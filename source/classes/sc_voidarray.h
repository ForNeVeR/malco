/**
 * @file sc_voidarray.h
 * @author ForNeVeR, impworks.
 * sc_voidarray header.
 * Defines properties and methods of sc_voidarray class.
 */

#ifndef SC_VOIDARRAY_H
#define SC_VOIDARRAY_H

/**
 * sc_voidarray constructor.
 * @param size Desired size of the array.
 */
sc_voidarray::sc_voidarray(int size)
{
  mLength = mSize = 0;
  mPtr = NULL;
  if(size) resize(size);
}

/**
 * sc_voidarray copy constructor.
 * @param obj Source sc_voidarray.
 */
sc_voidarray::sc_voidarray(const sc_voidarray &obj)
{
  mLength = mSize = obj.mLength;
  mPtr = new void *[mSize];
  if(!mPtr) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  std::memcpy(mPtr, obj.mPtr, mSize * sizeof(void *));
}

/**
 * sc_voidarray destructor.
 */
sc_voidarray::~sc_voidarray()
{
  delete [] mPtr;
}

/**
 * Adds a pointer to the array.
 * @param ptr Pointer to be added.
 */
void sc_voidarray::add(void *ptr)
{
  resize(mLength + 1);
  mPtr[mLength++] = ptr;
}

/**
 * Removes a pointer to the array.
 * @param index Index of the pointer to be removed.
 */
void sc_voidarray::del(long index)
{
  if(index < mLength - 1)
    std::memmove((void *) (mPtr + index), (void *) (mPtr + index + 1), mLength - index - 1);
  mLength--;
}

/**
 * Pops an item from the array.
 * @return Last item of the array.
 */
inline void *sc_voidarray::pop()
{
  if(!mLength) return NULL;

  void *item = mPtr[mLength - 1];
  mLength--;
  return item;
}

/**
 * Gets a pointer from the array.
 * @param index Index of the pointer.
 * @return Void pointer.
 */
inline void *&sc_voidarray::get(long index)
{
  if(index >= mLength)
    resize(index+1);
    
  return mPtr[index];
}

/**
 * Sets a pointer in the array.
 * @param index Index of the pointer.
 * @param ptr Pointer to be saved.
 */
inline void sc_voidarray::set(long index, void *ptr)
{
  if(index >= mLength)
    resize(index+1);

  mLength = index + 1;
  mPtr[index] = ptr;
}

/**
 * Indexing operator.
 * @param index Index of the pointer in array.
 * @return Void pointer.
 */
inline void *&sc_voidarray::operator[](long index)
{
  return get(index);
}

/**
 * Returns length of the array.
 * @return Length of the array.
 */
long sc_voidarray::length() const
{
  return mLength;
}

/**
 * Resizes array to desired size (to reserve space).
 * @param size Desired size in bytes.
 * @todo Allow shrinking array.
 */
void sc_voidarray::resize(long size)
{
  if(size <= mLength || size < mSize)
    return;

  void **new_ptr = new void *[size];
  if(!new_ptr) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);

  if(mPtr)
  {
    std::memcpy((void *) new_ptr, (void *) mPtr, mLength * sizeof(void *));
    delete[] mPtr;
  }

  // fill non-requested space with nulls
  for(long idx = mSize; idx < size; idx++)
    new_ptr[idx] = NULL;

  mSize = size;
  mPtr = new_ptr;
}

#endif // SC_VOIDARRAY_H
