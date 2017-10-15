/**
 * @file sc_list.h
 * @author impworks.
 * sc_list, sc_queue and sc_stack header.
 * Defines properties and methods of sc_list, sc_queue and sc_stack classes.
 */

#ifndef SC_LIST_H
#define SC_LIST_H

//--------------------------------
// sc_listitem
//--------------------------------

sc_listitem::sc_listitem()
{
  mNext = mPrev = NULL;
  pObj          = NULL;
}


//--------------------------------
// sc_list
//--------------------------------

/**
 * sc_list constructor.
 */
inline sc_list::sc_list()
{
  mFirst = mLast = mCurr = NULL;
  mLength = mCurrIdx = 0;
}

/**
 * Pops value from the right end of the list.
 * @return Object at the right end of the list.
 */
rc_var *sc_list::pop()
{
  if(mLength > 0)
  {
    rc_var *obj = mLast->pObj;
    if(mLength == 1)
    {
      delete mLast;
      mLast = mFirst = NULL;
    }
    else
    {
      mLast = mLast->mPrev;
      delete mLast->mNext;
      mLast->mNext = NULL;
    }

    mLength--;
    return obj;
  }

  return NULL;
}

/**
 * Pushes an item.
 * This function is not implemented here, inheriting classes should implement
 * it.
 */
void sc_list::push(rc_var *ptr)
{
  // nothing here
}


/**
 * Rewinds iteration over the list.
 */
inline void sc_list::iter_rewind()
{
  mCurr = mFirst;
}


/**
 * Returns current item and advances iterator to next one.
 * @return Current item.
 */
rc_var *sc_list::iter_next()
{
  if(!mCurr) return NULL;

  rc_var *obj = mCurr->pObj;
  mCurr       = mCurr->mNext;
  return obj;
}

/**
 * Access a specific object in the list.
 */
rc_var &sc_list::operator[](long idx)
{
  if(idx >= 0 && idx < mLength)
  {
    // either of these will definitely work
    while(mCurrIdx < idx)
    {
      mCurrIdx--;
      mCurr = mCurr->mNext;
    }

    while(mCurrIdx > idx)
    {
      mCurrIdx--;
      mCurr = mCurr->mPrev;
    }

    return *mCurr->pObj;
  }
  else
    ERROR(M_ERR_BAD_INNER_INDEX, M_EXC_INTERNAL);
}

/**
 * Retrieve a specific element of the list
 */
inline rc_var *sc_list::get(long idx)
{
  return &((*this)[idx]);
}


//--------------------------------
// sc_stack
//--------------------------------

/**
 * Pushes value to the right end of the list.
 * @param ptr Object to be pushed.
 */
void sc_stack::push(rc_var *ptr)
{
  sc_listitem *tmp = new sc_listitem();
  tmp->pObj        = ptr;

  if(mLength == 0)
  {
    mFirst = mLast = tmp;
  }
  else
  {
    mLast->mNext = tmp;
    tmp->mPrev   = mLast;
    mLast        = tmp;
  }

  mLength++;
}


//--------------------------------
// sc_queue
//--------------------------------

/**
 * Pushes value to the left end of the list.
 * @param ptr Object to be pushed.
 */
void sc_queue::push(rc_var *ptr)
{
  sc_listitem *tmp = new sc_listitem();
  tmp->pObj        = ptr;

  if(mLength == 0)
  {
    mFirst = mLast = mCurr = tmp;
  }
  else
  {
    mFirst->mPrev = tmp;
    tmp->mNext    = mFirst;
    mFirst        = tmp;
  }

  mLength++;
}

#endif
