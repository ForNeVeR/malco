/**
 * @file rc_strtable.h
 * @author impworks.
 * rc_stritem and rc_strtable header.
 * Defines properties and methods of rc_stritem and rc_strtable classes.
 */

#ifndef RC_STRTABLE_H
#define RC_STRTABLE_H

//--------------------------------
// rc_stritem
//--------------------------------

/**
 * rc_stritem constructor.
 */
rc_stritem::rc_stritem()
{
  mLength = 0;
  mString = NULL;
}

/**
 * rc_stritem destructor.
 */
rc_stritem::~rc_stritem()
{
  delete[] mString;
}

//--------------------------------
// rc_strtable
//--------------------------------

/**
 * rc_strtable constructor.
 */
rc_strtable::rc_strtable()
{
  mLastBuf = mLength = mLastLength = 0;
  mTable.resize(STRTABLE_CHUNK_SIZE);
  mTable.mPtr[0] = (void *)new rc_stritem[STRTABLE_BUF_SIZE];
  if(!mTable.mPtr[0]) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
}

/**
 * rc_strtable destructor.
 */
rc_strtable::~rc_strtable()
{
  for(long idx = 0; idx < mTable.length(); idx++)
    delete[](rc_stritem *) mTable.mPtr[idx];
}

/**
 * Inserts a new string into the table.
 * @param str String to be stored.
 * @param len Length of the string (0 to autocalculate).
 * @return The ID of the string in table.
 */
long rc_strtable::add(const char *str, long len)
{
  if(!len) len = strlen(str);

  // need to allocate new buffer
  if(mLastLength == STRTABLE_BUF_SIZE)
  {
    // need to allocate new chunk
    if(mLastBuf == mTable.length()) mTable.resize(mTable.length() + STRTABLE_CHUNK_SIZE);

    mLastBuf++;
    mLastLength           = 0;
    mTable.mPtr[mLastBuf] = new rc_stritem[STRTABLE_BUF_SIZE];
  }

  rc_stritem *curr = ((rc_stritem *)mTable.mPtr[mLastBuf]) + mLastLength;
  curr->mString    = new char[len + 1];
  strncpy(curr->mString, str, len);
  *(curr->mString + len) = '\0';
  curr->mLength          = len;

  mLength++;
  mLastLength++;

  return mLength - 1;
}

/**
 * Inserts a new string into the table.
 * @param str ic_string to be stored.
 * @param len Length of the string (0 to auto-calculate).
 * @return The ID of the string in table.
 */
long rc_strtable::add(ic_string *str, long len)
{
  return add(str->get(), (len ? len : str->length()));
}

/**
 * Inserts an existing string into the table. Does not reserve new memory.
 * @param str Char string to be stored.
 * @param len Length of the string (0 to auto-calculate).
 * @return The ID of the string in table.
 */
long rc_strtable::add_existing(char *str, long len)
{
  if(!len) len = strlen(str);

  // need to allocate new buffer
  if(mLastLength == STRTABLE_BUF_SIZE)
  {
    // need to allocate new chunk
    if(mLastBuf == mTable.length()) mTable.resize(mTable.length() + STRTABLE_CHUNK_SIZE);

    mLastBuf++;
    mLastLength           = 0;
    mTable.mPtr[mLastBuf] = new rc_stritem[STRTABLE_BUF_SIZE];
  }

  rc_stritem *curr = ((rc_stritem *)mTable.mPtr[mLastBuf]) + mLastLength;
  curr->mString    = str;
  curr->mLength    = len;

  mLength++;
  mLastLength++;

  return mLength - 1;
}

/**
 * Searches the table for a given string.
 * @param str String to be found.
 * @return Index of the string.
 */
long rc_strtable::find(const char *str)
{
  for(long idx = 0; idx < mTable.length(); idx++)
  {
    for(int idx2 = 0; idx2 < (idx == mTable.length() - 1 ? mLastLength : STRTABLE_BUF_SIZE); idx2++)
    {
      if(!strcmp(str, (((rc_stritem *)mTable.mPtr[idx])[idx2]).mString)) return idx * STRTABLE_BUF_SIZE + idx2;
    }
  }

  return -1;
}

/**
 * Searches the table for a given string.
 * @param str String to be found.
 * @return Index of the string.
 */
inline long rc_strtable::find(ic_string *str)
{
  return find(str->get());
}

/**
 * Clears the string table.
 */
void rc_strtable::clear()
{
  // clear data
  for(long idx = 0; idx < mTable.length(); idx++)
    delete[](rc_cmd *) mTable.mPtr[idx];

  // fill new data
  mLastBuf = mLength = mLastLength = 0;
  mTable.resize(STRTABLE_CHUNK_SIZE);
  mTable.mPtr[0] = (void *)new rc_stritem[STRTABLE_BUF_SIZE];
  if(!mTable.mPtr[0]) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
}

/**
 * Returns the string at current index.
 * @param idx Index of the string in the table.
 * @return Pointer to string.
 */
char *rc_strtable::get(long idx)
{
  if(idx < mLength)
  {
    long buf = 0;
    while(idx > STRTABLE_BUF_SIZE)
    {
      idx -= STRTABLE_BUF_SIZE;
      buf++;
    }
    return (((rc_stritem *)mTable.mPtr[buf])[idx]).mString;
  }
  else
    return NULL;
}

/**
 * Returns string table length.
 */
inline long rc_strtable::length()
{
  return mLength;
}

/**
 * Loads the string table from a file.
 * @param name Name of the file to be loaded.
 */
void rc_strtable::file_load(const char *name)
{
  FILE *f = fopen(name, "rb");
  if(f)
  {
    // clear current table
    for(long idx = 0; idx < mTable.length(); idx++)
    {
      if(mTable.mPtr[idx]) delete[](rc_stritem *)(mTable.mPtr[idx]);
    }
    mLength = mLastBuf = mLastLength = 0;

    // load new table
    fseek(f, 0, SEEK_SET);
    fread(&mLength, sizeof(long), 1, f);
    mLastBuf    = (long)floorf((float)mLength / (float)STRTABLE_BUF_SIZE);
    mLastLength = mLength % STRTABLE_BUF_SIZE;
    if(!mLastLength) mLastLength = STRTABLE_BUF_SIZE;

    // load tape
    mTable.resize(mLastBuf + (STRTABLE_CHUNK_SIZE - mLastBuf % STRTABLE_CHUNK_SIZE));
    for(long idx = 0; idx <= mLastBuf; idx++)
    {
      mTable.mPtr[idx] = new rc_stritem[STRTABLE_BUF_SIZE];
      for(int idx2 = 0; idx2 < STRTABLE_BUF_SIZE; idx2++)
      {
        if(idx == mLastBuf && idx2 == mLastLength) break;
        rc_stritem *curr = ((rc_stritem *)mTable.mPtr[idx]) + idx2;
        fread(&(curr->mLength), sizeof(long), 1, f);
        curr->mString = new char[curr->mLength + 1];
        fread(curr->mString, sizeof(char), curr->mLength + 1, f);
      }
    }
    fclose(f);
  }
  else
    ERROR(M_ERR_NO_STRINGTABLE, M_EMODE_ERROR);
}

/**
 * Saves the string table into a file.
 * @param name Name of the file to be replaced.
 */
void rc_strtable::file_save(const char *name)
{
  FILE *f = fopen(name, "wb");
  if(f)
  {
    fwrite(&mLength, sizeof(long), 1, f);
    for(long idx = 0; idx <= mLastBuf; idx++)
    {
      for(int idx2 = 0; idx2 < STRTABLE_BUF_SIZE; idx2++)
      {
        if(idx == mLastBuf && idx2 == mLastLength) break;
        rc_stritem *curr = ((rc_stritem *)mTable.mPtr[idx]) + idx2;
        fwrite(&(curr->mLength), sizeof(long), 1, f);
        fwrite(curr->mString, sizeof(char), curr->mLength + 1, f);
      }
    }
    fclose(f);
  }
  else
    ERROR(M_ERR_NO_STRINGTABLE, M_EMODE_ERROR);
}

/**
 * Returns the string at current index.
 * @param idx Index of the string in the table.
 * @return Pointer to string.
 */
inline char *rc_strtable::operator[](long idx)
{
  return get(idx);
}

#endif
