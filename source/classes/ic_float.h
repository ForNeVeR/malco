/**
 * @file ic_float.h
 * @author impworks.
 * ic_float header.
 * Defines properties and methods of ic_float class.
 */

#ifndef IC_FLOAT_H
#define IC_FLOAT_H

/**
 * ic_float constructor.
 * @param value Default value.
 */
ic_float::ic_float(double value)
{
  mValue = value;
  mStrBuf = NULL;
}

/**
 * ic_float destructor.
 */
ic_float::~ic_float()
{
  delete mStrBuf;
}

/**
 * ic_float -> boolean converter.
 * @return bool
 */
inline bool ic_float::to_b()
{
  return mValue ? true : false;
}

/**
 * ic_float -> int converter.
 * @return long
 */
inline long ic_float::to_i()
{
  return (long)mValue;
}

/**
 * ic_float -> string converter.
 * @return char*
 */
const char *ic_float::to_s()
{
  delete mStrBuf;

  mStrBuf = new char[32];
  char len = sprintf(mStrBuf, "%.10f", mValue) - 1;

  // trim off trailing zeroes
  while(len > 1 && mStrBuf[len] == '0' && mStrBuf[len-1] != '.')
  {
    mStrBuf[len] = '\0';
    len--;
  }

  return mStrBuf;
}

#endif
