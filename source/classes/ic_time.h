/**
 * @file ic_time.h
 * @author impworks.
 * ic_time header.
 * Defines properties and methods of ic_time class.
 */

#ifndef IC_TIME_H
#define IC_TIME_H

/**
 * ic_time constructor.
 * @param tstamp Default timestamp.
 */
ic_time::ic_time(long tstamp)
{
  mTimestamp = (time_t)tstamp;
  mStrBuf    = NULL;
  memset(&mTime, 0, sizeof(struct tm));
}

/**
 * ic_time destructor.
 */
ic_time::~ic_time()
{
  delete[] mStrBuf;
}

/**
 * Sets ic_time to current time.
 */
inline void ic_time::current()
{
  mTimestamp = time(NULL);
  mTime      = *localtime(&mTimestamp);
}

/**
 * Sets ic_time to given timestamp.
 * @param tstamp Desired timestamp.
 */
inline void ic_time::set(long tstamp)
{
  mTimestamp = (time_t)tstamp;
  mTime      = *localtime(&mTimestamp);
}

/**
 * Returns timestamp.
 * @return Timestamp as long value.
 */
inline long ic_time::get()
{
  return (long)mTimestamp;
}

/**
 * Formats time according to time string.
 * @param format Char string containing desired time format.
 * @return Pointer to string with formatted time.
 */
const char *ic_time::format(const char *format)
{
  long bufsize = 0, idx = 0;
  while(1)
  {
    const char *curr = format + idx, *next = curr + 1;
    if(*curr == '\0') break;

    if(*curr == '%')
    {
      // magic numbers galore below
      // i doubt there's a better way tho'
      switch(*next)
      {
        case 'a': bufsize += 3; break;
        case 'A': bufsize += 9; break;
        case 'b': bufsize += 3; break;
        case 'B': bufsize += 9; break;
        case 'c': bufsize += 24; break;
        case 'd': bufsize += 2; break;
        case 'H': bufsize += 2; break;
        case 'I': bufsize += 2; break;
        case 'j': bufsize += 3; break;
        case 'm': bufsize += 2; break;
        case 'M': bufsize += 2; break;
        case 'p': bufsize += 2; break;
        case 'S': bufsize += 2; break;
        case 'U': bufsize += 2; break;
        case 'w': bufsize++; break;
        case 'W': bufsize += 2; break;
        case 'x': bufsize += 8; break;
        case 'X': bufsize += 8; break;
        case 'y': bufsize += 2; break;
        case 'Y': bufsize += 4; break;
        case 'z': bufsize += 3; break;
        case '%': bufsize++; break;
      }
      idx++;
    }
    else
      bufsize++;

    idx++;
  }

  delete mStrBuf;

  mStrBuf = new char[bufsize + 2];
  if(!mStrBuf) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  strftime(mStrBuf, bufsize + 1, format, &mTime);
  return mStrBuf;
}

/**
 * ic_time -> boolean convertor.
 * @return bool
 */
inline bool ic_time::to_b()
{
  return mTimestamp ? true : false;
}

/**
 * ic_time -> float convertor.
 * @return double
 */
inline double ic_time::to_f()
{
  return (double)mTimestamp;
}

/**
 * ic_time -> int convertor.
 * @return long
 */
inline long ic_time::to_i()
{
  return get();
}

/**
 * ic_int -> string convertor (default formatting).
 * @return char *
 */
const char *ic_time::to_s()
{
  // 8 symbols for date, 8 for time, 1 for space and 1 for \0 => 18 in total
  delete mStrBuf;

  mStrBuf = new char[18];
  if(!mStrBuf) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  strftime(mStrBuf, 18, "%x %X", &mTime);
  return mStrBuf;
}

/**
 * Assign operator.
 * @param right Timestamp to be assigned.
 * @return Link to this.
 */
inline ic_time &ic_time::operator=(long right)
{
  set(right);
  return *this;
}

/**
 * Assign operator.
 * @param right ic_time to be assigned.
 * @return Link to this.
 */
inline ic_time &ic_time::operator=(ic_time &right)
{
  mTimestamp = right.mTimestamp;
  mTime      = right.mTime;
  return *this;
}

/**
 * Addition operator.
 * @param right Seconds to be added.
 * @return Link to this.
 */
inline ic_time &ic_time::operator+(long right)
{
  set((long)mTimestamp + right);
  return *this;
}

/**
 * Addition operator.
 * @param right ic_time to be added.
 * @return Link to this.
 */
inline ic_time &ic_time::operator+(ic_time &right)
{
  set((long)right.mTimestamp + (long)mTimestamp);
  return *this;
}

/**
 * Increment operator.
 * @return Link to this.
 */
inline ic_time &ic_time::operator++()
{
  set((long)++mTimestamp);
  return *this;
}

/**
 * Increment operator.
 * @return Link to this.
 */
inline ic_time &ic_time::operator++(int wtf)
{
  set((long)++mTimestamp);
  return *this;
}

/**
 * Subtraction operator.
 * @param right Seconds to be subtracted.
 * @return Link to this.
 */
inline ic_time &ic_time::operator-(long right)
{
  set((long)mTimestamp - right);
  return *this;
}

/**
 * Subtraction operator.
 * @param right ic_time to be subtracted.
 * @return Link to this.
 */
inline ic_time &ic_time::operator-(ic_time &right)
{
  set((long)right.mTimestamp - (long)mTimestamp);
  return *this;
}

/**
 * Decrement operator.
 * @return Link to this.
 */
inline ic_time &ic_time::operator--()
{
  set((long)--mTimestamp);
  return *this;
}

/**
 * Decrement operator.
 * @return Link to this.
 */
inline ic_time &ic_time::operator--(int wtf)
{
  set((long)--mTimestamp);
  return *this;
}

/**
 * Equality operator.
 * @param right ic_time to be compared.
 * @return bool
 */
inline bool ic_time::operator==(ic_time &right)
{
  return mTimestamp == right.mTimestamp;
}

/**
 * Inquality operator.
 * @param right ic_time to be compared.
 * @return bool
 */
inline bool ic_time::operator!=(ic_time &right)
{
  return mTimestamp != right.mTimestamp;
}

/**
 * Greater operator.
 * @param right ic_time to be compared.
 * @return bool
 */
inline bool ic_time::operator>(ic_time &right)
{
  return mTimestamp > right.mTimestamp;
}

/**
 * Greater or equal operator.
 * @param right ic_time to be compared.
 * @return bool
 */
inline bool ic_time::operator>=(ic_time &right)
{
  return mTimestamp >= right.mTimestamp;
}

/**
 * Less operator.
 * @param right ic_time to be compared.
 * @return bool
 */
inline bool ic_time::operator<(ic_time &right)
{
  return mTimestamp < right.mTimestamp;
}

/**
 * Less or equal operator.
 * @param right ic_time to be compared.
 * @return bool
 */
inline bool ic_time::operator<=(ic_time &right)
{
  return mTimestamp <= right.mTimestamp;
}

#endif
