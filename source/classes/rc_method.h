/**
 * @file rc_method.h
 * @author impworks.
 * rc_method header.
 * Defines properties and methods of rc_method class.
 */

#ifndef RC_METHOD_H
#define RC_METHOD_H

void rc_method::setup(int min, int max, bool splat, ...)
{
  pExternalScope = NULL;
  mMinArgs = min;
  mMaxArgs = MAX(max, min);
  mSplatArgs = splat;

  va_list names;
  va_start(names, splat);
  for(long idx = 0; idx < max; idx++)
    mParams.add((void *)new ic_string(va_arg(names, char *)));
  va_end(names);
}

void rc_method::op()
{
  pExternalScope = NULL;
  mMinArgs = 1;
  mMaxArgs = 1;
  mSplatArgs = false;
  mParams.add((void *)new ic_string("value"));
}

#endif