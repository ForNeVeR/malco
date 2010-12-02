/**
 * @file ic_range.h
 * @author impworks.
 * ic_range header.
 * Defines properties and methods of ic_range class.
 */

#ifndef IC_RANGE_H
#define IC_RANGE_H

/**
 * ic_range constructor.
 * @param start Interval start.
 * @param end Interval end.
 */

ic_range::ic_range(long start, long end)
{
  mStart = start;
  mEnd = end;
  mCurr = 0;
  mFinished = false;
}

/**
 * ic_range string constructor.
 * @param str String containing char range representation.
 */
ic_range::ic_range(const char *str)
{
  set(str);
}

/**
 * Sets range limits from char string.
 * @param str String containing char range representation.
 */
void ic_range::set(const char *str)
{
  mStart = atol(str);
  mEnd = atol(strstr(str, "..")+2);
  mCurr = 0;
  mFinished = false;
}

/**
 * Checks value for being in the range.
 * @param value Value to be checked.
 * @return bool
 */
inline bool ic_range::contains(long value)
{
  return mStart <= value && mEnd >= value;
}

/**
 * Checks value for being in the range.
 * @param value Value to be checked.
 * @return bool
 */
inline bool ic_range::contains(double value)
{
  return mStart <= value && mEnd >= value;
}

/**
 * Compares two ranges according to their lengths.
 * @param obj Interval to compare this to.
 * @return -1 if this is bigger, 0 if equal, 1 otherwise.
 */
inline int ic_range::compare(ic_range *obj)
{
  long first = labs(mStart - mEnd);
  long second = labs(obj->mStart - obj->mEnd);
  if(first == second)
    return 0;
  if(first > second)
    return -1;
  else
    return 1;
}

/**
 * Returns length of the range.
 */
inline long ic_range::length()
{
  return labs(mStart - mEnd);
}

/**
 * Gets next value from range for iteration.
 * @return Current value.
 */
long ic_range::iter_next()
{
  if(!mFinished)
  {
    long tmp = mStart + mCurr;
    mCurr++;
    if(tmp >= mEnd) mFinished = true;
    return tmp;
  }
  else
    return 0;
}

/**
 * Rewinds iterator.
 */
inline void ic_range::iter_rewind()
{
  mFinished = false;
  mCurr = 0;
}

/**
 * ic_range -> boolean convertor.
 * @return bool
 */
inline bool ic_range::to_b()
{
  return abs(mEnd - mStart) > 0 ? true : false;
}

/**
 * ic_range -> int convertor.
 * @return long
 */
inline long ic_range::to_i()
{
  return length();
}

/**
 * ic_range -> string convertor.
 * @return char*
 */
const char *ic_range::to_s()
{
  // 6 digits per number + 2 for dots + 1 for \0
  delete mStrBuf;

  // determine approx buffer size
  char bufsize = 15, idx=0;
  if(mStart > 100000 || mStart < -100000) bufsize += 5;
  if(mEnd > 100000 || mEnd < -100000) bufsize += 5;
  mStrBuf = new char[bufsize];
  if(!mStrBuf) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);

  // convert first number
  ltoa(mStart, mStrBuf, 10);

  // add dots
  for(idx=0; idx<20; idx++)
    if(*(mStrBuf+idx) == '\0') break;
  strcpy(mStrBuf + idx, "..");

  ltoa(mEnd, mStrBuf+idx+2, 10);

  return mStrBuf;
}

/**
 * Assign operator.
 * @param str String to assign.
 * @return Link to this.
 */
ic_range &ic_range::operator=(const char *str)
{
  set(str);
  return *this;
}

/**
 * Assign operator.
 * @param right ic_range to be assigned.
 * @return Link to this.
 */
ic_range &ic_range::operator=(ic_range &right)
{
  mStart = right.mStart;
  mEnd = right.mEnd;
  mCurr = right.mCurr;
  mFinished = right.mFinished;
  return *this;
}

/**
 * Equality operator.
 * @param right ic_range to be compared.
 * @return bool
 */
inline bool ic_range::operator==(ic_range &right)
{
  return compare(&right) == 0;
}

/**
 * Inquality operator.
 * @param right ic_range to be compared.
 * @return bool
 */
 inline bool ic_range::operator!=(ic_range &right)
{
  return compare(&right) != 0;
}

/**
 * Greater operator.
 * @param right ic_range to be compared.
 * @return bool
 */
inline bool ic_range::operator>(ic_range &right)
{
  return compare(&right) > 0;
}

/**
 * Greater or equal operator.
 * @param right ic_range to be compared.
 * @return bool
 */
inline bool ic_range::operator>=(ic_range &right)
{
  return compare(&right) >= 0;
}

/**
 * Less operator.
 * @param right ic_range to be compared.
 * @return bool
 */
inline bool ic_range::operator<(ic_range &right)
{
  return compare(&right) < 0;
}

/**
 * Less of equal operator.
 * @param right ic_range to be compared.
 * @return bool
 */
inline bool ic_range::operator<=(ic_range &right)
{
  return compare(&right) <= 0;
}

#endif
