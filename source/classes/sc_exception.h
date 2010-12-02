/**
 * @file sc_exception.h
 * @author impworks.
 * sc_exception header.
 * Defines properties and methods of sc_exception class.
 */

#ifndef SC_EXCEPTION_H
#define SC_EXCEPTION_H

/**
 * sc_exception constructor.
 * @param msg Error message.
 * @param level Error level.
 * @param file File where error occurred.
 * @param line Line where error occurred.
 */
sc_exception::sc_exception(const char *msg, int type, const char *file, long line)
{
  mErrorMsg = new ic_string(msg);
  mErrorType = type;
  mFile = file;
  mLine = line;
}

/**
* sc_exception constructor.
* @param msg Error message.
* @param level Error level.
* @param file File where error occurred.
* @param line Line where error occurred.
*/
sc_exception::sc_exception(ic_string *msg, int type, const char *file, long line)
{
  mErrorMsg = msg;
  mErrorType = type;
  mFile = file;
  mLine = line;
}

/**
 * sc_exception destructor.
 */
sc_exception::~sc_exception()
{
  delete mErrorMsg;
}

#endif
