/**
 * @file sc_void.h
 * @author impworks.
 * sc_voidlist and sc_voiditem header.
 * Defines properties and methods of sc_voidlist and sc_voiditem classes.
 */

#ifndef SC_VOIDLIST_H
#define SC_VOIDLIST_H

//--------------------------------
// sc_voiditem
//--------------------------------

/**
 * sc_voiditem constructor.
 * @param ptr Void pointer to store.
 */
sc_voiditem::sc_voiditem(void *ptr)
{
  mPtr = ptr;
  pPrev = pNext = NULL;
}

//--------------------------------
// sc_voidlist
//--------------------------------

/**
 * sc_voidlist constructor.
 */
sc_voidlist::sc_voidlist()
{
  mFirst = mLast = NULL;
  mLength = 0;
}

/**
 * sc_voidlist destructor.
 */
sc_voidlist::~sc_voidlist()
{
  register long idx;
  sc_voiditem *curr = mFirst, *tmp;
  for(idx=0; idx<mLength; idx++)
  {
    tmp = curr->pNext;
    delete curr;
    curr = tmp;
  }
}

/**
 * Adds item to void list.
 * @param ptr Pointer to be added.
 */
void sc_voidlist::add(void *ptr)
{
  sc_voiditem *curr = new sc_voiditem(ptr);
  if(!curr) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);

  if(mLength == 0)
  {
    mFirst = mLast = curr;
  }
  else
  {
    curr->pPrev = mLast;
    mLast->pNext = curr;
    mLast = curr;
  }

  mLength++;
}

/**
 * Removes a pointer from list.
 * @param ptr Pointer to be removed.
 */
void sc_voidlist::del(void *ptr)
{
  register long idx;
  sc_voiditem *curr = mFirst;
  for(idx=0; idx<mLength; idx++)
  {
    if(curr->mPtr == ptr)
    {
      if(curr != mFirst) curr->pPrev->pNext = curr->pNext;
      if(curr != mLast) curr->pNext->pPrev = curr->pPrev;
      delete curr;
      mLength --;
      return;
    }

    curr = curr->pNext;
  }
}

/**
 * Removes a pointer from list.
 * @param id Index of the item in list.
 */
void sc_voidlist::del(long id)
{
  register long idx;
  sc_voiditem *curr = mFirst;
  for(idx=0; idx<mLength; idx++)
  {
    if(idx == id)
    {
      if(curr != mFirst) curr->pPrev->pNext = curr->pNext;
      if(curr != mLast) curr->pNext->pPrev = curr->pPrev;
      delete curr;
      mLength --;
      return;
    }

    curr = curr->pNext;
  }
}

/**
 * Pops the last pointer from list.
 * @return Void pointer stored in the last element of the list.
 */
void *sc_voidlist::pop()
{
  if(mLength > 0)
  {
    void *result = mLast->mPtr;
    sc_voiditem *curr = mLast;
    if(mLength > 1)
    {
      mLast = mLast->pPrev;
      mLast->pNext = NULL;
    }
    else
    {
      mLast = mFirst = NULL;
    }

    delete curr;
    mLength--;
    return result;
  }
  else
    return NULL;
}

/**
 * Packs list into a consequent array of pointers.
 * @return Array of void pointers.
 */
sc_voidarray *sc_voidlist::pack()
{
  sc_voidarray *arr = new sc_voidarray();
  arr->mPtr = new void*[mLength];
  if(!arr->mPtr) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  arr->mLength = arr->mSize = mLength;
  register long idx;
  sc_voiditem *curr = mFirst;
  for(idx=0; idx<mLength; idx++)
  {
    arr->mPtr[idx] = curr->mPtr;
    curr = curr->pNext;
  }

  return arr;
}

/**
 * Returns a void pointer by it's index.
 * @param id Pointer index.
 * @return Void pointer at index.
 */
void * sc_voidlist::get(long id)
{
  sc_voiditem *curr = mFirst;

  for(long idx = 0; idx < id; idx++)
    curr = curr->pNext;

  return curr->mPtr;
}

#endif
