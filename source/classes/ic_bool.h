/**
 * @file ic_bool.h
 * @author impworks.
 * ic_bool header.
 * Defines properties and methods of ic_bool class.
 */

#ifndef IC_BOOL_H
#define IC_BOOL_H

/**
 * ic_bool constructor.
 * @param value Default value.
 */
ic_bool::ic_bool(bool value)
{
  mValue = value;
}

/**
 * ic_bool -> int convertor.
 * @return long
 */
inline long ic_bool::to_i()
{
  return mValue ? 1 : 0;
}

/**
 * ic_bool -> float convertor.
 * @return double
 */
inline double ic_bool::to_f()
{
  return mValue ? 1.0 : 0.0;
}

/**
 * ic_bool -> string convertor.
 * @return char*
 */
const char *ic_bool::to_s()
{
  return mValue ? "true" : "false";
}

#endif
