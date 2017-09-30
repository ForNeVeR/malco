/**
 * @file ic_int.h
 * @author impworks.
 * ic_int header.
 * Defines properties and methods of ic_int class.
 */

#ifndef IC_INT_H
#define IC_INT_H

/**
 * ic_int constructor.
 * @param value Default value.
 */
ic_int::ic_int(long value)
{
  mValue = value;
  mStrBuf = NULL;
}

/**
 * ic_int destructor.
 */
ic_int::~ic_int()
{
   delete mStrBuf;
}

/**
 * ic_int -> boolean convertor.
 * @return bool
 */
inline bool ic_int::to_b()
{
  return mValue ? true : false;
}

/**
 * ic_int -> float convertor.
 * @return double
 */
inline double ic_int::to_f()
{
  return (double)mValue;
}

/**
 * ic_int -> string converter.
 * @return char*
 */
const char *ic_int::to_s()
{
  delete mStrBuf;

  auto val = std::to_string(mValue);
  mStrBuf = new char[val.size() + 1];
  std::strncpy(mStrBuf, val.c_str(), val.size());
  return mStrBuf;
}

#endif
