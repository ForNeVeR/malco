/**
 * @file rc_tape.h
 * @author impworks.
 * rc_tape header.
 * Defines properties and methods of rc_tape class.
 */

#ifndef RC_TAPE_H
#define RC_TAPE_H

/**
 * rc_tape constructor.
 */
rc_tape::rc_tape()
{
  mLength = mLastLength = mLastBuf = 0;
  mTape.resize(TAPE_CHUNK_SIZE);
  mTape.mPtr[0] = (void*)new rc_cmd[TAPE_BUF_SIZE];
  if(!mTape.mPtr[0]) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
}

/**
 * rc_tape destructor.
 */
rc_tape::~rc_tape()
{
  for(long idx=0; idx <= mLastBuf; idx++)
  {
    if(mTape.mPtr[idx])
      delete [] (rc_cmd*)mTape.mPtr[idx];
  }
}

/**
 * Adds a number of commands to the tape (used by compiler).
 * @param cmd Pointer to command (or array of commands).
 * @param num Number of commands in an array (default = 1).
 */
void rc_tape::add(rc_cmd *cmd, long num)
{
  mLength += num;
  if(mLastLength + num <= TAPE_BUF_SIZE)
  {
    // copy `num` commands starting from `cmd` to the end of the last buffer
    memcpy(((rc_cmd *)(mTape.mPtr[mLastBuf]))+mLastLength, cmd, sizeof(rc_cmd)*num);
    mLastLength += num;
  }
  else
  {
    long count = num;
    // fill last buffer with as many commands as possible
    memcpy(((rc_cmd *)(mTape.mPtr[mLastBuf]))+mLastLength, cmd, sizeof(rc_cmd)*(TAPE_BUF_SIZE-mLastLength));
    cmd+=(TAPE_BUF_SIZE-mLastLength);
    num-=(TAPE_BUF_SIZE-mLastLength);
    mLastBuf++;

    // create as many buffers as needed to store commands
    while(num > 0)
    {
      // resize voidarray as needed
      if(mLastBuf == mTape.length()-1)
        mTape.resize(mTape.length() + TAPE_CHUNK_SIZE);

      // copy data buffer by buffer
      mTape.mPtr[mLastBuf] = new rc_cmd[TAPE_BUF_SIZE];
      if(!mTape.mPtr[mLastBuf]) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
      memcpy(mTape.mPtr[mLastBuf], cmd, sizeof(rc_cmd)*MIN(num, TAPE_BUF_SIZE));
      num-=MIN(num, TAPE_BUF_SIZE);
      cmd+=TAPE_BUF_SIZE;
      mLastBuf++;
    }

    mLastBuf--;
    mLastLength = (mLastLength + count) % TAPE_BUF_SIZE;
    if(!mLastLength) mLastLength = TAPE_BUF_SIZE;
  }
}


/**
 * Select a random command on the tape.
 * @param idx Index of the command to select.
 */
rc_cmd *rc_tape::select(long idx)
{
  if(idx < mLength)
  {
    long buf=0;
    while(idx > TAPE_BUF_SIZE)
    {
      idx-=TAPE_BUF_SIZE;
      buf++;
    }
    return ((rc_cmd*)(mTape.mPtr[buf]))+idx;
  }
  else
    return NULL;
}

/**
 * Returns the length of the tape.
 * @return Length of the tape.
 */
inline long rc_tape::length()
{
  return mLength;
}

/**
 * Loads tape from a file.
 * @param name Name of the file.
 * @param mode Rewrite or append to tape.
 */
void rc_tape::file_load(const char *name)
{
  FILE *f = fopen(name, "rb");
  if(f)
  {
    // clear current tape
    for(long idx=0; idx < mLastBuf; idx++)
    {
      if(mTape.mPtr[idx])
        delete [] (rc_cmd *)(mTape.mPtr[idx]);
    }
    mLength = mLastBuf = mLastLength = 0;

    // determine file length
    fseek(f, 0, SEEK_SET);
    fread(&mLength, sizeof(long), 1, f);
    mLastBuf = (long)floorf((float)mLength / (float)TAPE_BUF_SIZE);
    mLastLength = mLength % TAPE_BUF_SIZE;
    if(!mLastLength) mLastLength = TAPE_BUF_SIZE;

    // load tape
    mTape.resize(mLastBuf + (TAPE_CHUNK_SIZE - mLastBuf % TAPE_CHUNK_SIZE));
    for(long idx=0; idx <= mLastBuf; idx++)
    {
      mTape.mPtr[idx] = new rc_cmd[TAPE_BUF_SIZE];
      if(!mTape.mPtr[idx]) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
      fread(mTape.mPtr[idx], sizeof(rc_cmd), TAPE_BUF_SIZE, f);
    }
    fclose(f);
  }
  else
    ERROR(M_ERR_NO_SOURCE, M_EMODE_ERROR);
}

/**
 * Saves tape as a file.
 * @param name Name of the file.
 * @param mode Rewrite or append to file.
 */
void rc_tape::file_save(const char *name)
{
  register long idx;
  FILE *f = fopen(name, "wb");
  if(f)
  {
    fseek(f, 0, SEEK_SET);
    fwrite(&mLength, sizeof(long), 1, f);
    for(idx=0; idx<=mLastBuf; idx++)
      fwrite(mTape.mPtr[idx], sizeof(rc_cmd), TAPE_BUF_SIZE, f);
    fclose(f);
  }
  else
    ERROR(M_ERR_NO_SOURCE, M_EMODE_ERROR);
}

/**
 * Selects a random command on the tape.
 * @param idx Index of the command.
 * @return Pointer to the command with given index.
 */
inline rc_cmd *rc_tape::operator[](long idx)
{
  return select(idx);
}

#if MALCO_DEBUG == 1
/**
 * Outputs debug info.
 */
void rc_tape::debug()
{
  printf("%i\n", mLength);
  for(long idx=0; idx<=mLastBuf; idx++)
  {
    printf("buf %i\n", idx);
    rc_cmd *arr = (rc_cmd*)mTape.mPtr[idx];
    for(long idx2=0; idx2 < (idx < mLastBuf ? TAPE_BUF_SIZE : mLastLength); idx2++)
    {
      rc_cmd *curr = (rc_cmd *)(arr+idx2);
      printf("  cmd #%i: type %i, mod %i, addr %i\n", idx2, curr->mCmd, curr->mModifier, curr->mParam.val);
    }

    printf("\n\n");
  }
}
#endif

#endif
