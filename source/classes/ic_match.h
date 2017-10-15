/**
 * @file ic_match.h
 * @author impworks.
 * ic_match header.
 * Defines properties and methods of ic_match class.
 */

#ifndef IC_MATCH_H
#define IC_MATCH_H

/**
 * ic_match default constructor.
 */
ic_match::ic_match()
{
  pString = NULL;
  mCount  = -1;

  for(int idx = 0; idx < REGEX_MAX_MATCHES; idx++)
  {
    mMatches[idx * 2 + 1] = 0;
    mMatches[idx * 2]     = 0;
    mSubstrings[idx]      = NULL;
  }
}

/**
 * ic_match default destructor.
 */
ic_match::~ic_match()
{
  for(int idx = 0; idx < REGEX_MAX_MATCHES; idx++)
  {
    if(mSubstrings[idx]) delete[] mSubstrings[idx];
  }
}

/**
 * Returns the number of found substrings.
 * @return Found substrings count.
 */
inline long ic_match::count()
{
  return mCount;
}

/**
 * Returns a pointer to found substring.
 * @param Substring id
 * @return Substring (copied from source)
 */
const char *ic_match::get(int id)
{
  if(id < 0 || id > 9 || id > mCount) return NULL;

  if(!mSubstrings[id])
  {
    long new_len    = mMatches[id * 2 + 1] - mMatches[id * 2];
    mSubstrings[id] = new char[new_len + 1];
    if(!mSubstrings[id]) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
    strncpy(mSubstrings[id], pString + mMatches[id * 2], new_len);
    *(mSubstrings[id] + new_len) = '\0';
  }

  return mSubstrings[id];
}

/**
 * Save starting and ending points of a match to an array of longs.
 * @param id Substring id (0..9)
 * @param[out] where Pointer to long[2] to save values to.
 */
void ic_match::bounds(int id, long *where)
{
  if(!where || id < 0 || id > 9 || id > mCount) return;

  *where       = mMatches[id * 2];
  *(where + 1) = mMatches[id * 2 + 1];
}

#endif