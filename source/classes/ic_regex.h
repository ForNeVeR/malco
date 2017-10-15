/**
 * @file ic_regex.h
 * @author impworks.
 * ic_regex header.
 * Defines properties and methods of ic_regex class.
 */

#ifndef IC_REGEX_H
#define IC_REGEX_H

#include <memory>
#include <regex>

/**
 * ic_regex constructor.
 */
ic_regex::ic_regex()
{
  mPattern     = NULL;
  mErrorOffset = -1;
  mOptions     = 0;
}

/**
 * ic_regex string constructor.
 * @param pattern Pointer to source string to be compiled.
 */
ic_regex::ic_regex(const char *pattern)
{
  mPattern     = NULL;
  mErrorOffset = -1;
  mOptions     = 0;

  set(pattern);
}

/**
 * ic_regex copy constructor.
 * @param regex Pointer to source regex.
 */
ic_regex::ic_regex(ic_regex *regex)
{
  mPattern     = NULL;
  mErrorOffset = -1;
  mOptions     = 0;

  set(regex->mPattern);
}

/**
 * ic_regex destructor.
 */
ic_regex::~ic_regex()
{
}

/**
 * Assigns a new regular expression.
 * @param pattern Pointer to source string to be compiled.
 */
void ic_regex::set(const char *pattern)
{
  set(pattern, std::regex::flag_type{});
}

/**
 * Assigns a new regular expression.
 * @param pattern Pointer to source string to be compiled.
 * @param flags Initial flags of the regex.
 */
void ic_regex::set(const char *pattern, std::regex::flag_type flags)
{
  // validate pattern
  register long idx;
  long new_len = strlen(pattern);
  char chr;
  if(*pattern != '/')
  {
    mErrorOffset = -1;
    pError       = "Illegal regex syntax";
    return;
  }

  // scan for modifiers and closing slash
  for(idx = new_len - 1; idx >= 0; idx--)
  {
    chr = *(pattern + idx);
    if(chr == 'i')
    {
      flags |= std::regex::icase;
      continue;
    }

    if(chr == 'x')
    {
      flags |= std::regex::extended;
      continue;
    }

    if(chr == '/') break;

    // error occured
    mErrorOffset = -1;
    pError       = "Illegal regex modifier";
    return;
  }

  if(idx == 0)
  {
    mErrorOffset = -1;
    pError       = "Ending slash not found";
    return;
  }

  if(idx == 1)
  {
    mErrorOffset = -1;
    pError       = "Regex is empty";
    return;
  }

  // store data
  delete[] mPattern;

  mPattern = new char[new_len + 1];
  if(!mPattern) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  strcpy(mPattern, pattern);
  mOptions = flags;

  // compile regular expression
  const auto tmp_buf = std::string(pattern + 1, idx - 1);
  try
  {
    mRegEx = std::regex(tmp_buf, flags);
  }
  catch(const std::regex_error &error)
  {
    pError = error.what();
    ERROR(M_ERR_BAD_REGEXP, M_EMODE_ERROR);
  }
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
  if(mPattern == nullptr) ERROR(M_ERR_BAD_REGEXP, M_EMODE_ERROR);

  if(!len) len = strlen(str);

  auto match = std::unique_ptr<ic_match>(new ic_match());

  auto search = std::cregex_iterator(str + offset, str + len, mRegEx);
  auto end    = std::cregex_iterator();
  auto count  = 0;
  for(auto i = 0; i < REGEX_MAX_MATCHES && search != end; ++i, ++search)
  {
    ++count;
    auto begin                 = offset + search->position();
    auto end                   = begin + search->length();
    match->mMatches[i * 2]     = begin;
    match->mMatches[i * 2 + 1] = end;
  }

  match->mCount  = count;
  match->pString = str;
  return match.release();
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
  if(mPattern == nullptr) return;

  if(!(mRegEx.flags() & std::regex::optimize))
  {
    set(mPattern, std::regex::optimize);
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
  mPattern     = NULL;
  pError       = NULL;
  mErrorOffset = -1;
  mOptions     = 0;

  set(right.mPattern);

  return *this;
}

/**
 * Equality operator.
 * @param right String to be compared.
 * @return bool
 */
bool ic_regex::operator==(ic_regex &right)
{
  return strcmp(mPattern, right.mPattern) == 0;
}

/**
 * Equality operator.
 * @param right String to be compared.
 * @return bool
 */
bool ic_regex::operator!=(ic_regex &right)
{
  return strcmp(mPattern, right.mPattern) != 0;
}

#endif
