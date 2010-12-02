/**
 * @file ic_regex.h
 * @author impworks.
 * ic_regex header.
 * Defines properties and methods of ic_regex class.
 */

#ifndef IC_REGEX_H
#define IC_REGEX_H

const unsigned char* ic_regex::mLocaleTables = NULL;

/**
 * Initializes regex by creating tables (static).
 */
void ic_regex::init()
{
  mLocaleTables = (const unsigned char*)pcre_maketables();
}

/**
 * ic_regex constructor.
 */
ic_regex::ic_regex()
{
  if(!mLocaleTables) init();
  mRegEx = NULL;
  mExtra = NULL;
  mPattern = NULL;
  mErrorOffset = -1;
  mOptions = 0;
}

/**
 * ic_regex string constructor.
 * @param pattern Pointer to source string to be compiled.
 */
ic_regex::ic_regex(const char *pattern)
{
  if(!mLocaleTables) init();
  mRegEx = NULL;
  mExtra = NULL;
  mPattern = NULL;
  mErrorOffset = -1;
  mOptions = 0;

  set(pattern);
}

/**
 * ic_regex copy constructor.
 * @param regex Pointer to source regex.
 */
ic_regex::ic_regex(ic_regex* regex)
{
  if(!mLocaleTables) init();
  mRegEx = NULL;
  mExtra = NULL;
  mPattern = NULL;
  mErrorOffset = -1;
  mOptions = 0;

  set(regex->mPattern);
}

/**
 * ic_regex destructor.
 */
ic_regex::~ic_regex()
{
  delete mRegEx;
  delete mExtra;
}

/**
 * Assigns a new regular expression.
 * @param pattern Pointer to source string to be compiled.
 */
void ic_regex::set(const char *pattern)
{
  // validate pattern
  register long idx;
  long new_len = strlen(pattern);
  int options = 0;
  char chr;
  if(*pattern != '/')
  {
    mErrorOffset = -1;
    pError = "Illegal regex syntax";
    return;
  }

  // scan for modifiers and closing slash
  for(idx=new_len-1; idx>=0; idx--)
  {
    chr = *(pattern+idx);
    if(chr == 'i')
    {
      options |= PCRE_CASELESS;
      continue;
    }

    if(chr == 's')
    {
      options |= PCRE_DOTALL;
      continue;
    }

    if(chr == 'm')
    {
      options |= PCRE_MULTILINE;
      continue;
    }

    if(chr == 'x')
    {
      options |= PCRE_EXTENDED;
      continue;
    }

    if(chr == '/') break;

    // error occured
    mErrorOffset = -1;
    pError = "Illegal regex modifier";
    return;
  }

  if(idx == 0)
  {
    mErrorOffset = -1;
    pError = "Ending slash not found";
    return;
  }

  if(idx == 1)
  {
    mErrorOffset = -1;
    pError = "Regex is empty";
    return;
  }

  // store data
  delete [] mPattern;
  
  mPattern = new char[new_len+1];
  if(!mPattern) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  strcpy(mPattern, pattern);
  mOptions = options;

  // compile regular expression
  delete mRegEx;
  delete mExtra;
  mExtra = NULL;

  char *tmp_buf = new char[idx+1];
  if(!tmp_buf) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  strncpy(tmp_buf, pattern+1, idx-1);
  tmp_buf[idx-1]='\0';
  mRegEx = pcre_compile(tmp_buf, options, &pError, &mErrorOffset, mLocaleTables);
  if(pError) ERROR(M_ERR_BAD_REGEXP, M_EMODE_ERROR);
  delete [] tmp_buf;
}

/**
 * Collects matches of a string.
 * @param str String to be matched.
 * @param offset Offset from the start of the string.
 * @param len Number of characters in the string to account for.
 * @return Number of matches found.
 */
ic_match *ic_regex::match(const char *str, int offset, long len)
{
  if(!mRegEx)
    ERROR(M_ERR_BAD_REGEXP, M_EMODE_ERROR);

  if(!len) len = strlen(str);

  ic_match *match = new ic_match();

  match->mCount = pcre_exec(mRegEx, mExtra, str, len, offset, 0, match->mMatches, REGEX_MAX_MATCHES*2);
  if(match->mCount < 0)
  {
    delete match;
    return NULL;
  }
  else
  {
    match->pString = str;
    return match;
  }
}

/**
 * Collects matches of an ic_string.
 * @param str String to be matched.
 * @param offset Offset from the start of the string.
 * @return Number of matches found.
 */
ic_match *ic_regex::match(ic_string *str, int offset)
{
  return match(str->get(), offset, str->length());
}

/**
 * Studies regex to speed up looped matching.
 */
inline void ic_regex::study()
{
  if(!mRegEx) return;

  if(!mExtra)
  {
    const char *errptr;
    mExtra = pcre_study(mRegEx, 0, &errptr);
    if(errptr) mExtra = NULL;
  }
}

/**
 * Returns pointer to error message.
 * @return Error message.
 */
inline const char *ic_regex::error()
{
  return pError;
}

/**
 * Returns pointer to string position that caused the error.
 * @return Error position.
 */
inline int ic_regex::error_pos()
{
  return mErrorOffset;
}

/**
 * ic_regex -> boolean converter.
 * @return bool
 */
inline bool ic_regex::to_b()
{
  return strlen(mPattern) > 0;
}

/**
 * ic_regex -> string converter.
 * @return chat *
 */
inline char *ic_regex::to_s()
{
  return mPattern;
}

/**
 * Assign operator.
 * @param right String to be compiled to a regex.
 * @return Link to this.
 */
ic_regex &ic_regex::operator=(const char *right)
{
  set(right);
  return *this;
}

/**
 * Assign operator.
 * @param right Source regex.
 * @return Link to this.
 */
ic_regex &ic_regex::operator=(ic_regex &right)
{
  if(!mLocaleTables) init();
  mRegEx = NULL;
  mExtra = NULL;
  mPattern = NULL;
  pError = NULL;
  mErrorOffset = -1;
  mOptions = 0;

  set(right.mPattern);

  return *this;
}

/**
 * Equality operator.
 * @param right String to be compared.
 * @return bool
 */
bool ic_regex::operator== (ic_regex &right)
{
  return strcmp(mPattern, right.mPattern) == 0;
}

/**
 * Equality operator.
 * @param right String to be compared.
 * @return bool
 */
bool ic_regex::operator!= (ic_regex &right)
{
  return strcmp(mPattern, right.mPattern) != 0;
}

#endif
