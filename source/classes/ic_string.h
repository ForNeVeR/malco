/**
 * @file ic_string.h
 * @author impworks.
 * ic_string and ic_strbuffer header.
 * Defines properties and methods of ic_string and ic_strbuffer classes.
 */

#ifndef IC_STRING_H
#define IC_STRING_H

#include <cstdint>

//--------------------------------
// ic_strbuffer
//--------------------------------

/**
 * ic_strbuffer constructor.
 * @param cpc Default capacity.
 */
ic_strbuffer::ic_strbuffer(long cpc)
{
  mBuf = new char[cpc+1];
  if(!mBuf) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  *mBuf = '\0';
  mLength = 0;
  mCapacity = cpc;
  pPrev = pNext = NULL;
}

/**
 * ic_strbuffer destructor.
 */
inline ic_strbuffer::~ic_strbuffer()
{
  delete [] mBuf;
}

//--------------------------------
// ic_string
//--------------------------------

char* ic_string::whitespace = " \t\n\r";

/**
 * ic_string default constructor.
 */
ic_string::ic_string()
{
  mFirst = new ic_strbuffer(STR_MIN_CPC);
  if(!mFirst) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  mLast = mFirst;
  mNumBuffers = 1;
  mLength = 0;
  mCapacity = STR_MIN_CPC;
}

/**
 * ic_string constructor with specified length.
 */
ic_string::ic_string(long cpc)
{
  mFirst = new ic_strbuffer(cpc);
  if(!mFirst) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  mLast = mFirst;
  mNumBuffers = 1;
  mLength = 0;
  mCapacity = cpc;
}

/**
 * ic_string char string constructor.
 * @param str Source string.
 * @param new_len Number of characters from str to be copied.
 */
ic_string::ic_string(const char *str, long new_len)
{
  if(!new_len) new_len = strlen(str);
  register long new_cpc = STR_MIN_CPC;
  while(new_cpc < new_len)
    new_cpc <<= 1;

  mFirst = new ic_strbuffer(new_cpc);

  mLast = mFirst;
  strncpy(mFirst->mBuf, str, new_len);
  *(mFirst->mBuf+new_len) = '\0';
  mFirst->mLength = mLength = new_len;
  mCapacity = mFirst->mCapacity = new_cpc;
  mNumBuffers = 1;
}

/**
 * ic_string copy constructor.
 * @param str Source ic_string as pointer.
 */
ic_string::ic_string(const ic_string &str)
{
  mFirst = new ic_strbuffer(str.capacity());
  if(!mFirst) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  mLast = mFirst;
  mNumBuffers = 1;
  set(&str);
}

/**
 * ic_string destructor.
 */
ic_string::~ic_string()
{
  ic_strbuffer *tmp, *curr=mFirst;
  for(int idx=0; idx<mNumBuffers; idx++)
  {
    if(curr)
    {
      tmp = curr->pNext;
      delete curr;
      curr = tmp;
    }
  }
}

/**
 * ic_string -> char* converter.
 * Flattens the string into a single buffer and returns pointer to it's first char.
 * @return Pointer to first buffer (continious char*).
 */
char *ic_string::get()
{
  // Note that you should NEVER NEVER NEVER delete pointer returned by this
  // function!
  if(mNumBuffers > 1)
  {
    // create new buffer
    register long new_cpc = STR_MIN_CPC;
    while(new_cpc < mLength) new_cpc <<= 1;
    char *buf = new char [new_cpc+1];
    if(!buf) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
    if(buf)
    {
      ic_strbuffer *curr = mFirst, *tmp;
      long new_len = 0;
      for(register int idx=0; idx<mNumBuffers; idx++)
      {
        // copy old data to buffer and release it
        strncpy(buf+new_len, curr->mBuf, curr->mLength);
        new_len += curr->mLength;
        tmp = curr->pNext;
        if(idx == 0)
          delete [] curr->mBuf;
        else
          delete curr;
        curr = tmp;
      }
      buf[mLength] = '\0';
      mFirst->mBuf = buf;
      mFirst->mLength = mLength;
      mFirst->mCapacity = mCapacity = new_cpc;
      mNumBuffers = 1;
    }
    mLast = mFirst;
  }

  return mFirst->mBuf;
}

/**
 * Clears ic_string.
 * @param cpc New default capacity.
 */
void ic_string::empty(long cpc)
{
  if(cpc < STR_MIN_CPC) cpc = STR_MIN_CPC;

  if(mNumBuffers > 1)
  {
    // remove buffers
    ic_strbuffer *curr = mFirst->pNext, *tmp;
    for(int idx=1; idx<mNumBuffers; idx++)
    {
      tmp = curr->pNext;
      delete curr;
      curr = tmp;
    }
    mLast = mFirst;
  }

  if(mFirst->mCapacity != cpc)
  {
    delete mFirst;
    mFirst = new ic_strbuffer(cpc);
  }
  else
  {
    mFirst->mBuf[0] = '\0';
    mFirst->mLength = 0;
  }

  mCapacity = cpc;
  mLength = 0;
}

/**
 * Copies string from source string into ic_string.
 * @param src Source string.
 * @param new_len Number of characters from str to be copied.
 */
void ic_string::set(const char *src, long new_len)
{
  if(!new_len) new_len = strlen(src);
  if(!new_len) return;

  register long new_cpc = STR_MIN_CPC;
  while(new_cpc < new_len) new_cpc <<= 1;
  empty(new_cpc);
  strncpy(mFirst->mBuf, src, new_len);
  *(mFirst->mBuf+new_len) = '\0';
  mFirst->mLength = new_len;
  mLength = new_len;
}

/**
 * Copies string from another ic_string into ic_string.
 * @param src Source string.
 * @param new_len Number of characters from str to be copied.
 */
void ic_string::set(const ic_string *src, long new_len)
{
  empty(src->mFirst->mCapacity);
  append(src, new_len);
}

/**
 * Appends string from source string into ic_string.
 * @param src Source string.
 * @param new_len Number of characters from str to be appended.
 */
void ic_string::append(const char *src, long new_len)
{
  register long new_cpc = mLast->mCapacity << 1, idx=0, idx2=0;
  while(*(src+idx2) != '\0')
  {
    if(new_len != 0 && idx2 == new_len) break;

    if(mLast->mLength+idx == mLast->mCapacity-1)
    {
      // Finish up old buffer
      mLast->mLength = mLast->mCapacity-1;
      *(mLast->mBuf+mLast->mLength) = '\0';

      // Create a new buffer
      ic_strbuffer *new_buf = new ic_strbuffer(new_cpc);
      if(!new_buf) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
      mNumBuffers++;
      new_cpc <<= 1;
      mLast->pNext = new_buf;
      new_buf->pPrev = mLast;
      mLast = new_buf;

      // Reset buffer offset
      idx=0;
    }

    *(mLast->mBuf+mLast->mLength+idx) = *(src+idx2);
    idx++;
    idx2++;
  }

  mLast->mLength += idx;
  *(mLast->mBuf+mLast->mLength) = '\0';
  mLength += idx2;
}

/**
 * Appends string from another ic_string into ic_string.
 * @param src Source string.
 * @param new_len Number of characters from str to be copied.
 */
void ic_string::append(const ic_string *src, long new_len)
{
  long str_len = src->length();
  if(!new_len || new_len > str_len) new_len = str_len;
  register long left_len = new_len;

  ic_strbuffer *curr = src->mFirst;
  while(left_len > 0)
  {
    append(curr->mBuf, (left_len < curr->mLength ? left_len : curr->mLength));
    left_len -= curr->mLength;
    curr = curr->pNext;
  }
}

/**
 * Appends a character to the string.
 * @param src Character to append.
 * @todo Rewrite with optimization.
 */
void ic_string::append(char src)
{
  char str[2] = {src, 0};
  append(str, 1);
}

/**
 * Prepends string from another ic_string into ic_string.
 * @param src Source string.
 * @param new_len Number of characters from str to be copied.
 */
void ic_string::prepend(const char *str, long new_len)
{
  if(!new_len) new_len = strlen(str);
  if(mFirst->mLength + new_len < mFirst->mCapacity)
  {
    // string can be fitted into existing buffer
    memmove(mFirst->mBuf + new_len, mFirst->mBuf, mFirst->mLength+1);
    memcpy(mFirst->mBuf, str, new_len);
  }
  else
  {
    long new_cpc = STR_MIN_CPC;
    while(new_cpc < new_len) new_cpc <<= 1;
    ic_strbuffer *buf = new ic_strbuffer(new_cpc);
    memcpy(buf->mBuf, str, new_len);
    buf->pNext = mFirst;
    mFirst->pPrev = buf;
    mFirst = buf;
  }
}

/**
 * Prepends string from another ic_string into ic_string.
 * @param src Source string.
 * @param new_len Number of characters from str to be copied.
 * @todo Rewrite with optimization for normal ic_strings.
 */
void ic_string::prepend(ic_string *str, long new_len)
{
  if(!new_len || new_len > str->length()) new_len = str->length();
  prepend(str->get(), new_len);
}

/**
 * Prepends a character to the string.
 * @param src Character to prepend.
 * @todo Rewrite with optimization.
 */
void ic_string::prepend(char src)
{
  char str[2] = {src, 0};
  prepend(str, 1);
}

/**
 * Reverses the order of characters in the string.
 */
void ic_string::reverse()
{
  register long first_char=0, last_char=mLast->mLength-1;
  ic_strbuffer *first_buf=mFirst, *last_buf=mLast;
  long half_len = mLength >> 1;
  char tmp;
  for(long idx=0; idx<half_len; idx++)
  {
    // swap Nth from beginning and from end
    tmp = *(first_buf->mBuf + first_char);
    *(first_buf->mBuf + first_char) = *(last_buf->mBuf + last_char);
    *(last_buf->mBuf + last_char) = tmp;

    first_char++;
    last_char--;

    // move beginning buffer forward
    if(first_char == first_buf->mLength-1)
    {
      first_char = 0;
      first_buf = first_buf->pNext;
    }

    // move ending buffer backward
    if(last_char == -1)
    {
      last_buf = last_buf->pPrev;
      last_char = last_buf->mLength-1;
    }
  }
}

/**
 * Replaces substrings.
 * @param from String to search for.
 * @param to String to replace occurences with.
 * @param max Maximum number of replaces (0 for any number).
 * @return Number of occurences replaced.
 */
long ic_string::replace(const char *from, char *to, long max)
{
  bool found_flag = false;
  long from_len = strlen(from), to_len = strlen(to);
  register long idx, count = 0;
  long piece_offset=0, found_offset=0, found_len=0, final_len=0;

  // test for idiotic cases
  if(max < 0) max = 0;

  // make sure string is flat
  get();

  if(from_len == to_len)
  {
    // no need to reallocate memory, work directly in mFirst->mBuf
    for(idx=0; idx<mLength; idx++)
    {
      if(*(mFirst->mBuf+idx) == *(from+found_len))
      {
        if(!found_flag)
        {
          found_offset = idx;
          found_flag = true;
        }
        found_len++;
      }
      else
      {
        if(found_len)
        {
          found_len = 0;
          found_flag = false;
        }
      }

      if(found_len == from_len)
      {
        strncpy(mFirst->mBuf+found_offset, to, to_len);
        found_len = 0;
        found_flag = false;
        count++;
        // count becomes 1 right before check, thus max = 0
        // means an unlimited number of matches
        if(count == max) break;
      }
    }
  }
  else
  {
    // reallocate memory
    long new_len = mLength-from_len*max+to_len*max+1, new_cpc = STR_MIN_CPC;
    while(new_cpc < new_len) new_cpc <<= 1;
    char *new_buf = new char[new_cpc+1];
    if(!new_buf) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);

    for(idx=0; idx<mLength; idx++)
    {
      // check another character
      if(*(mFirst->mBuf+idx) == *(from+found_len))
      {
        if(!found_flag)
        {
          found_offset = idx;
          found_flag = true;
        }
        found_len++;
      }
      else
      {
        if(found_len)
        {
          found_len = 0;
          found_flag = false;
        }
      }

      // match found
      if(found_len == from_len)
      {
        strncpy(new_buf+final_len, mFirst->mBuf+piece_offset, (found_offset-piece_offset));
        final_len += (found_offset-piece_offset);
        strcpy(new_buf+final_len, to);
        final_len += to_len;

        piece_offset = idx+1;
        found_len = 0;
        found_flag = false;
        count++;
        if(count == max) break;
      }
    }

    if(piece_offset < mLength)
    {
      strcpy(new_buf+final_len, mFirst->mBuf+piece_offset);
      final_len = new_len;
    }

    *(new_buf+final_len) = '\0';
    mLength = mFirst->mLength = new_len;
    mCapacity = mFirst->mCapacity = new_cpc;
    delete [] mFirst->mBuf;
    mFirst->mBuf = new_buf;
  }

  return count;
}

/**
 * Replaces substrings using ic_strings.
 * @param from String to search for.
 * @param to String to replace occurences with.
 * @param max Maximum number of replaces (0 for any number).
 * @return Number of occurences replaced.
 */
long ic_string::replace(ic_string *from, ic_string *to, long max)
{
  return replace(from->get(), to->get(), max);
}

/**
 * Replaces substrings using regular expression.
 * @param from Regex pattern to search for.
 * @param to String to replace occurences with.
 * @param max Maximum number of replaces (0 for any number).
 * @return Number of occurences replaced.
 */
long ic_string::replace(ic_regex *from, ic_string *to, long max)
{
  register long idx;
  long from_len = length(), to_len = to->length(), count=0, offset=0, to_offset, to_pos;
  long bounds[2];
  ic_match *found;
  char *from_buf, *to_buf;

  // compile to-string into an array of anchors (string ends) and indexes (\\i)
  sc_voidlist anchor_list, index_list;
  to_buf = to->get();
  for(idx=0; idx<to_len-1; idx++)
  {
    if(*(to_buf+idx) == '\\' && *(to_buf+idx+1) >= '0' && *(to_buf+idx+1) <= '9')
    {
      // current symbol is \ and is followed by a number
      anchor_list.add((void *)idx);
      index_list.add((void*)(*(to_buf+idx+1)-48));
    }
  }
  anchor_list.add((void *)to_len);
  sc_voidarray *anchors = anchor_list.pack(), *indexes = index_list.pack();

  // create source string and flush current one
  ic_string *base = new ic_string(*this);
  if(!base) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  from_buf = base->get();
  empty();

  // study regexp to make it faster
  from->study();
  // process string in a loop
  while(1)
  {
    found = from->match(from_buf, offset, from_len);
    if(!found) break;

    // append 'piece' (everything before match)
    found->bounds(0, bounds);
    if(offset != bounds[0])
      append(from_buf+offset, bounds[0]-offset);

    // append interpolated string
    to_offset = 0;
    for(idx=0; idx<anchors->mLength; idx++)
    {
      // append to-string's piece
      to_pos = (long)anchors->mPtr[idx];
      if(to_pos - to_offset > 0)
        append(to_buf + to_offset, to_pos - to_offset);
      // set offset to start of next string (\i = 2 bytes)
      to_offset = to_pos+2;

      // append back reference
      if(idx < indexes->mLength)
        append(found->get(reinterpret_cast<intptr_t>(indexes->mPtr[idx])));
    }

    offset = bounds[1];
    count++;
    delete found;

    if(count == max) break;
  }

  if(offset < from_len)
    append(from_buf+offset, from_len-offset);

  delete base;
  delete anchors;
  delete indexes;
  return count;
}

/**
 * Translates characters in the string.
 * @param from Source characters.
 * @param to Destination characters.
 */
void ic_string::translate(char *from, char *to, long fromlen, long tolen)
{
  register long idx, idx2, buf_offset=0;
  ic_strbuffer *curr = mFirst;
  if(!fromlen) fromlen = strlen(from);
  if(!tolen) tolen = strlen(to);

  // test for idiotic cases
  if(!fromlen) return;

  // scan string
  for(idx=0; idx<mLength; idx++, buf_offset++)
  {
    // buffer overflows
    if(buf_offset == curr->mLength)
    {
      buf_offset = 0;
      curr = curr->pNext;
    }

    char *currchar = curr->mBuf + buf_offset;

    // scan substring
    for(idx2=0; idx2<fromlen; idx2++)
    {
      if(*currchar == *(from + idx2))
        *currchar = *(to + MIN(idx2, tolen-1));
    }
  }
}

/**
* Translates characters in the string.
* @param from Source characters string.
* @param to Destination characters string.
* @todo Rewrite so that it doesn't require get().
*/
void ic_string::translate(ic_string *from, ic_string *to)
{
  translate(from->get(), to->get(), from->length(), to->length());
}

/**
 * Converts string to uppercase.
 */
void ic_string::case_up()
{
  register long idx, buf_offset=0;
  ic_strbuffer *curr = mFirst;
  for(idx=0; idx<mLength; idx++, buf_offset++)
  {
    // buffer overflows
    if(buf_offset == curr->mLength)
    {
      buf_offset = 0;
      curr = curr->pNext;
    }

    *(curr->mBuf+buf_offset) = toupper(*(curr->mBuf+buf_offset));
  }
}

/**
 * Converts string to lowercase.
 */
void ic_string::case_down()
{
  register long idx, buf_offset=0;
  ic_strbuffer *curr = mFirst;
  for(idx=0; idx<mLength; idx++, buf_offset++)
  {
    // buffer overflows
    if(buf_offset == curr->mLength)
    {
      buf_offset = 0;
      curr = curr->pNext;
    }

    *(curr->mBuf+buf_offset) = tolower(*(curr->mBuf+buf_offset));
  }
}

/**
 * Converts string to uppercase.
 */
void ic_string::case_swap()
{
  register long idx, buf_offset=0;
  ic_strbuffer *curr = mFirst;
  for(idx=0; idx<mLength; idx++, buf_offset++)
  {
    // buffer overflows
    if(buf_offset == curr->mLength)
    {
      buf_offset = 0;
      curr = curr->pNext;
    }

    char *ch = curr->mBuf+buf_offset;
    if(isupper(*ch))
      *ch = tolower(*ch);
    else
      *ch = toupper(*ch);
  }
}

/**
 * Compares strings for equality.
 * @param str String to compare with.
 * @param len Length of substring from str to compare (0 for whole string).
 * @return -1 if (this) is bigger, 0 if equal, 1 otherwise.
 */
int ic_string::compare(const char *str, long len)
{
  register long idx, buf_offset=0;
  ic_strbuffer *curr = mFirst;
  if(!len) len = mLength;

  for(idx=0; idx<len; idx++, buf_offset++)
  {
    // buffer overflows
    if(buf_offset == curr->mLength)
    {
      buf_offset = 0;
      curr = curr->pNext;
    }

    // casual case
    if((*(curr->mBuf+buf_offset) < *(str+idx)))
      return 1;

    // second string is smaller
    if(*(str+idx) == '\0')
      return 1;

    if((*(curr->mBuf+buf_offset) > *(str+idx)))
      return -1;
  }

  if(*(str+idx) != '\0')
    return -1;

  return 0;
}

/**
 * Compares ic_strings for equality.
 * @param str String to compare with.
 * @param len Length of substring from str to compare (0 for whole string).
 * @return -1 if (this) is bigger, 0 if equal, 1 otherwise.
 */
int ic_string::compare(ic_string *str, long len)
{
  long str_len = MIN(mLength, str->length());
  long done_len = 0, cmp_len = 0, offset1 = 0, offset2 = 0;
  ic_strbuffer *buf1 = mFirst, *buf2 = str->mFirst;

  while(done_len < str_len)
  {
    // attempt to compare minimum size of buffers
    cmp_len = MIN(buf1->mLength-offset1, buf2->mLength-offset2);
    if(len > 0 && done_len + cmp_len > len) cmp_len = done_len - len;
    int result = memcmp(buf1->mBuf, buf2->mBuf, cmp_len);
    if(result != 0) return -result;

    // wrap first buffer if needed
    if(offset1+cmp_len == buf1->mLength)
    {
      buf1 = buf1->pNext;
      offset1 = 0;
    }
    else
      offset1 += cmp_len;

    // wrap second buffer if needed
    if(offset2+cmp_len == buf2->mLength)
    {
      buf2 = buf2->pNext;
      offset2 = 0;
    }
    else
      offset2 += cmp_len;

    done_len += cmp_len;
  }

  if(mLength < str->length()) return -1;
  if(mLength > str->length()) return 1;

  return 0;
}

/**
 * Returns a link to a single character.
 * @param pos Character index in the string.
 * @return Link to the character.
 */
char &ic_string::char_at(long pos) const
{
  ic_strbuffer *curr = mFirst;
  register long idx=pos;
  while(idx > curr->mLength)
  {
    idx -= curr->mLength;
    curr = curr->pNext;
  }
  return *(curr->mBuf+idx);
}

/**
 * Returns a substring.
 * @param start The position in the string to start from.
 * @param len The length of the substring.
 * @return Resulting substring.
 */
ic_string* ic_string::substr_get(long start, long len)
{
  ic_strbuffer *curr = mFirst;
  if(start < 0) start += mLength;
  if(len == 0) len = mLength-start;

  // test for idiotic cases
  if(start < 0) start = 0;
  if(start + len > mLength) len = mLength - start;

  ic_string *str = new ic_string();
  if(!str) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);

  while(start >= curr->mLength)
  {
    start -= curr->mLength;
    curr = curr->pNext;
  }

  while(len > 0)
  {
    // calculate length of string extracted from current buffer
    long read_len = len < curr->mLength - start ? len : curr->mLength - start;
    str->append(curr->mBuf + start, read_len);
    len -= read_len;
    start = 0;
    curr = curr->pNext;
  }

  return str;
}

/**
 * Replaces a substring.
 * @param start The position in the string to start from.
 * @param len The length of the substring.
 * @param to The string to replace found substring with.
 */
void ic_string::substr_set(long start, long len, const char *to)
{
  long to_len = strlen(to), to_offset = 0;
  ic_strbuffer *curr = mFirst;
  register long idx, buf_offset=start;

  if(start < 0)
    start += mLength;

  // test for idiotic cases
  if(to == NULL) return;
  if(start < 0 || len < 0) return;
  if(start + len > mLength) return;

  if(to_len == len)
  {
    // paste string over existing structure
    while(buf_offset > curr->mLength)
    {
      buf_offset -= curr->mLength;
      curr = curr->pNext;
    }

    for(idx=0; idx<len; idx++)
    {
      // buffer overflow
      if(buf_offset == curr->mLength)
      {
        buf_offset = 0;
        curr = curr->pNext;
      }

      *(curr->mBuf+buf_offset) = *(to+to_offset);
      to_offset++;
      buf_offset++;
    }
  }
  else
  {
    long new_len = mLength-len+to_len, new_cpc = STR_MIN_CPC;
    while(new_cpc < new_len) new_cpc <<= 1;

    get();
    char *new_buf = new char[new_cpc+1];
    if(!new_buf) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
    strncpy(new_buf, mFirst->mBuf, start);
    strcpy(new_buf+start, to);
    strncpy(new_buf+start+to_len, mFirst->mBuf+start+len, mLength-len);
    *(new_buf+new_len) = '\0';
    delete [] mFirst->mBuf;
    mFirst->mBuf = new_buf;

    mFirst->mLength = mLength = new_len;
    mFirst->mCapacity = mCapacity = new_cpc;
  }
}

/**
 * Replaces a substring with another ic_string.
 * @param start The position in the string to start from.
 * @param len The length of the substring.
 * @param to The string to replace found substring with.
 */
void ic_string::substr_set(long start, long len, ic_string *str)
{
  substr_set(start, len, str->get());
}

/**
 * Finds the location of a substring (search from beginning).
 * @param str String to search for.
 * @param offset Number of characters to skip.
 * @return Position of the found substring, or -1 if not found.
 */
long ic_string::substr_first(const char *str, long offset)
{
  bool found_flag = false;
  long sub_len = strlen(str), found_offset = 0, buf_offset = offset, sub_offset = 0;
  register long idx;
  ic_strbuffer *curr = mFirst;

  // test for idiotic cases
  if(str == NULL) return -1;
  if(!sub_len || !mLength) return -1;
  if(offset + sub_len >= mLength) return -1;

  while(buf_offset > curr->mLength)
  {
    // get offset from current buffer's start
    buf_offset -= curr->mLength;
    curr = curr->pNext;
  }

  for(idx=offset; idx<mLength; idx++, buf_offset++)
  {
    // buffer has ended
    if(buf_offset == curr->mLength)
    {
      buf_offset = 0;
      curr = curr->pNext;
    }

    // test for match
    if(*(curr->mBuf+buf_offset) == *(str+sub_offset))
    {
      // if first char matches, store starting point
      if(!found_flag)
      {
        found_offset = idx;
        found_flag = true;
      }
      sub_offset++;
    }
    else
    {
      if(sub_offset)
      {
        // attempt failed in the middle
        sub_offset = 0;
        found_flag = false;
      }
    }

    // match completed
    if(sub_offset == sub_len) return found_offset;
  }

  return -1;
}

/**
 * Finds the location of an ic_string substring (search from beginning).
 * @param str String to search for.
 * @param offset Number of characters to skip.
 * @return Position of the found substring, or -1 if not found.
 */
long ic_string::substr_first(ic_string *str, long offset)
{
  return substr_first(str->get(), offset);
}

/**
 * Finds the location of a substring (search from end).
 * @param str String to search for.
 * @param offset Number of characters to skip.
 * @return Position of the found substring, or -1 if not found.
 */
long ic_string::substr_last(const char *str, long offset)
{
  long sub_len = strlen(str), buf_offset = offset, sub_offset = 0;
  register long idx;
  ic_strbuffer *curr = mLast;

  // test for idiotic cases
  if(str == NULL) return -1;
  if(!sub_len || !mLength) return -1;
  if(offset + sub_len >= mLength) return -1;

  while(buf_offset > curr->mLength)
  {
    // get offset from current buffer's end
    buf_offset -= curr->mLength;
    curr = curr->pPrev;
  }

  for(idx=mLength-offset; idx>=0; idx--, buf_offset++)
  {
    // buffer has ended
    if(buf_offset == curr->mLength)
    {
      buf_offset = 0;
      curr = curr->pPrev;
    }

    // test for match
    if(*(curr->mBuf+curr->mLength-buf_offset-1) == *(str+sub_len-sub_offset-1))
    {
      sub_offset++;
    }
    else
    {
      sub_offset = 0;
    }

    // match completed
    if(sub_offset == sub_len) return idx-1;
  }

  return -1;
}

/**
 * Finds the location of an ic_string substring (search from end).
 * @param str String to search for.
 * @param offset Number of characters to skip.
 * @return Position of the found substring, or -1 if not found.
 */
long ic_string::substr_last(ic_string *str, long offset)
{
  return substr_last(str->get(), offset);
}

/**
 * Counts occurences of the substring.
 * @param str String to search for.
 * @return Number of occurences.
 */
long ic_string::substr_count(const char *str) const
{
  long sub_len = strlen(str), found_count = 0, buf_offset = 0, sub_offset = 0;
  register long idx;
  ic_strbuffer *curr = mFirst;

  // test for idiotic cases
  if(str == NULL) return 0;
  if(!sub_len || !mLength) return 0;

  for(idx=0; idx<mLength; idx++, buf_offset++)
  {
    // buffer has ended
    if(buf_offset == curr->mLength)
    {
      buf_offset = 0;
      curr = curr->pNext;
    }

    // test for match
    if(*(curr->mBuf+buf_offset) == *(str+sub_offset))
    {
      sub_offset++;
    }
    else
    {
      if(sub_offset) sub_offset = 0;
    }

    // match completed
    if(sub_offset == sub_len)
    {
      sub_offset = 0;
      found_count++;
    }
  }

  return found_count;
}

/**
 * Counts occurences of the ic_string substring.
 * @param str String to search for.
 * @return Number of occurences.
 */
long ic_string::substr_count(ic_string *str) const
{
  return substr_count(str->get());
}

/**
 * Trims whitespaces from left.
 */
void ic_string::ltrim()
{
  ic_strbuffer *curr = mFirst, *new_buf;
  register long idx, buf_offset = 0;
  long tmp_len = mLength, new_len, new_cpc = STR_MIN_CPC;
  char chr;

  for(idx=0; idx<tmp_len; idx++, buf_offset++)
  {
    if(buf_offset == curr->mLength)
    {
      mLength -= curr->mLength;
      mCapacity -= curr->mCapacity;
      mNumBuffers --;

      buf_offset = 0;
      curr = curr->pNext;
      delete curr->pPrev;
      curr->pPrev = NULL;
    }

    chr = *(curr->mBuf+buf_offset);
    if(!strchr(whitespace, chr)) break;
  }

  new_len = curr->mLength-buf_offset;
  while(new_cpc < new_len) new_cpc <<= 1;
  new_buf = new ic_strbuffer(new_cpc);
  if(!new_buf) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  new_buf->mLength = new_len;
  strcpy(new_buf->mBuf, curr->mBuf+buf_offset);
  mLength -= buf_offset;
  mCapacity = mCapacity - curr->mCapacity + new_cpc;

  // relink
  if(curr->pNext)
  {
    curr->pNext->pPrev = new_buf;
    new_buf->pNext = curr->pNext;
  }
  delete curr;
  if(mFirst == mLast)
    mFirst = mLast = new_buf;
  else
    mFirst = new_buf;
}

/**
 * Trims whitespaces from right.
 */
void ic_string::rtrim()
{
  ic_strbuffer *curr = mLast;
  long tmp_len = mLength;
  register long idx, buf_offset = mLast->mLength;
  char chr;

  for(idx=0; idx<tmp_len; idx++, buf_offset--)
  {
    if(buf_offset == -1)
    {
      curr = curr->pPrev;
      buf_offset = curr->mLength;

      // remove block
      mLength -= curr->pNext->mLength;
      mCapacity -= curr->pNext->mCapacity;
      mNumBuffers--;
      delete curr->pNext;
      curr->pNext = NULL;
    }

    chr = *(curr->mBuf+buf_offset);
    if(!strchr(whitespace, chr)) break;
  }

  buf_offset++;
  mLength = mLength-curr->mLength+buf_offset;
  curr->mLength = buf_offset;
  *(curr->mBuf+buf_offset) = '\0';
  if(mFirst == mLast)
    mFirst = mLast = curr;
  else
    mLast = curr;
}

/**
 * Trims whitespaces from both ends.
 */
void ic_string::trim()
{
  rtrim();
  ltrim();
}

/**
 * Splits the string using a delimiter.
 * @param delimiter The delimiter to split the string with.
 * @param max Number of maximum splits (0 for any number).
 * @return sc_voidarray of max+1 size containing pointers to ic_strings.
 */
sc_voidarray *ic_string::split(const char *delimiter, long max)
{
  bool found_flag = false;
  register long idx;
  long piece_offset=0, piece_len = 0, found_offset=0, found_len=0, delim_len = strlen(delimiter), found_count = 0;
  sc_voidlist items;
  ic_string *new_str;

  get();

  for(idx=0; idx<mLength; idx++)
  {
    if(*(mFirst->mBuf+idx) == *(delimiter+found_len))
    {
      if(!found_flag)
      {
        found_offset = idx;
        found_flag = true;
      }
      found_len++;
    }
    else
    {
      if(found_len)
      {
        piece_len += found_len;
        found_len = 0;
        found_flag = false;
      }
      piece_len++;
    }

    if(found_len == delim_len)
    {
      if(piece_len)
        new_str = new ic_string(mFirst->mBuf+piece_offset, piece_len);
      else
        new_str = new ic_string("");

      items.add((void *)new_str);
      piece_offset = idx+1;
      piece_len = 0;
      found_len = 0;
      found_flag = false;
      found_count++;
    }

    if(max > 0 && found_count == max)
    {
      piece_len = mLength - piece_offset;
      break;
    }
  }

  if(piece_offset > 0)
  {
    // string after last delimiter
    if(piece_len)
      new_str = new ic_string(mFirst->mBuf+piece_offset, piece_len);
    else
      new_str = new ic_string("");
  }
  else
  {
    new_str = new ic_string(*this);
  }

  if(!new_str) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
  items.add((void *)new_str);

  return items.pack();
}

/**
 * Splits the string using an ic_string delimiter.
 * @param delimiter The delimiter to split the string with.
 * @param max Number of maximum splits (0 for any number).
 * @return sc_voidarray of max+1 size containing pointers to ic_strings.
 */
sc_voidarray *ic_string::split(ic_string *delimiter, long max)
{
  return split(delimiter->get(), max);
}

/**
 * Splits the string using an ic_regex delimiter.
 * @param delimiter The regex to split the string with.
 * @param max Number of maximum splits (0 for any number).
 * @return sc_voidarray of max+1 size containing pointers to ic_strings.
 */
sc_voidarray *ic_string::split(ic_regex *delimiter, long max)
{
  long offset=0, count=0;
  long bounds[2];
  ic_string *new_str;
  ic_match *found;
  sc_voidlist items;

  get();

  // study regexp to make it faster
  delimiter->study();
  while(1)
  {
    found = delimiter->match(mFirst->mBuf, offset);
    if(found == 0) break;

    found->bounds(0, bounds);
    if(bounds[0] > offset)
      new_str = new ic_string(mFirst->mBuf+offset, bounds[0]-offset);
    else
      new_str = new ic_string();

    if(!new_str) ERROR(M_ERR_NO_MEMORY, M_EMODE_ERROR);
    items.add((void *)new_str);
    offset = bounds[1];

    count++;
    delete found;
    if(count == max) break;
  }

  if(offset > 0)
  {
    new_str = new ic_string(mFirst->mBuf+offset, mLength-offset);
    items.add((void*)new_str);
  }
  else
  {
    items.add((void*)this);
  }

  return items.pack();
}

/**
 * Interpolates a string, replacing %s' with strings from the array.
 * %s stands for 'substitution', not 'string', thus there's no %d, %i, etc.
 * @param string Array of void pointers to strings.
 */
void ic_string::format(sc_voidarray strings)
{
  long idx, bufidx = 0, stridx = 0, new_cpc = mLength;
  sc_voidarray lengths;

  // make sure string is flat
  get();

  // assume the worst case: all values from array are substituted
  lengths.resize(strings.length());
  for(idx=0; idx < strings.length(); idx++)
  {
    long len = strlen((char *)strings[idx]);
    lengths.set(idx, (void *)len);
    new_cpc += len - 2;
  }

  while(new_cpc < STR_MIN_CPC) new_cpc <<= 1;
  char *buf = new char[new_cpc+1], next;

  for(idx=0; idx < mLength; idx++)
  {
    if(*(mFirst->mBuf + idx) == '%')
    {
      next = *(mFirst->mBuf + idx + 1);
      if(next == 's')
      {
        strcpy(mFirst->mBuf+idx, (char *)strings[stridx]);
        bufidx+=(long)lengths[stridx];
        stridx++;
        idx++;
      }
      else
      {
        *(buf + bufidx) = '%';
        bufidx++;
        if(next == '%') idx++;
      }
    }
    else
    {
      *(buf + bufidx) = *(mFirst->mBuf + idx);
      bufidx++;
    }
  }

  mLength = mFirst->mLength = bufidx;
  mCapacity = mFirst->mCapacity = new_cpc;
  delete [] mFirst->mBuf;
  mFirst->mBuf = buf;
}

/**
 * Static formatting constructor.
 * @param str String in printf-like format.
 */
inline ic_string *ic_string::format(const char *str, ...)
{
  va_list args;
  va_start(args, str);
  ic_string *newstr = ic_string::format_list(str, args);
  va_end(args);
  return newstr;
}

/**
 * Static formatting constructor
 * @param str String in printf-like format.
 * @param args Argument list.
 */
ic_string *ic_string::format_list(const char *str, va_list args)
{
  ic_string *newstr = new ic_string(str);
  ic_int *num = new ic_int();
  ic_float *fl = new ic_float();
  long pos = 0;
  bool todo;

  newstr->get();

  do
  {
    todo = false;
    pos = newstr->substr_first("%", pos);
    if(pos != -1)
    {
      // found a string
      if(newstr->char_at(pos+1) == 's')
      {
        newstr->substr_set(pos, 2, va_arg(args, const char*));
        todo = true;
      }
      // found an integer
      else if(newstr->char_at(pos+1) == 'i')
      {
        num->mValue = va_arg(args, long);
        newstr->substr_set(pos, 2, num->to_s());
        todo = true;
      }
      // found a float
      else if(newstr->char_at(pos+1) == 'f')
      {
        fl->mValue = va_arg(args, double);
        newstr->substr_set(pos, 2, fl->to_s());
        todo = true;
      }
    }
  }
  while(todo);

  delete num;
  delete fl;
  return newstr;
}

/**
 * Loads a file into itself.
 * @param name Name of the file to be loaded.
 */
void ic_string::file_load(const char *name)
{
  register long new_cpc = STR_MIN_CPC;
  FILE *f = fopen(name, "rb");
  if(f)
  {
    // determine file length
    fseek(f, 0, SEEK_END);
    long file_len = ftell(f);
    while(new_cpc < file_len) new_cpc <<= 1;

    // load data
    empty(new_cpc);
    fseek(f, 0, SEEK_SET);
    fread(mFirst->mBuf, file_len, 1, f);
    fclose(f);

    // clean up
    *(mFirst->mBuf+file_len) = '\0';
    mFirst->mLength = mLength = file_len;
    mCapacity = new_cpc;
  }
  else
    ERROR(M_ERR_IO_NO_FILE, M_EMODE_ERROR);
}

/**
 * Saves itself into a file.
 * @param name Name of the file to be modified.
 * @param mode File access mode (FILE_REPLACE or FILE_APPEND).
 */
void ic_string::file_save(const char *name, char mode)
{
  register long idx;
  FILE *f = fopen(name, (mode == FILE_REPLACE ? "w" : "a"));
  if(f)
  {
    ic_strbuffer *curr = mFirst;
    for(idx=0; idx<mNumBuffers; idx++)
    {
      fwrite(curr->mBuf, curr->mLength, 1, f);
      curr = curr->pNext;
    }
    fclose(f);
  }
  else
    ERROR(M_ERR_IO_NO_FILE, M_EMODE_ERROR);
}

/**
 * ic_string -> int converter.
 * @return long
 */
inline long ic_string::to_i()
{
  if(this->mLength <= 16) return atol(get());
  else return 0;
}

/**
 * ic_string -> float converter.
 * @return double
 */
inline double ic_string::to_f()
{
  if(this->mLength <= 16) return atof(get());
  else return 0;
}

/**
 * ic_string -> boolean converter.
 * @return bool
 */
inline bool ic_string::to_b() const
{
  return mLength ? true : false;
}

/**
 * Returns ic_string's actual length.
 * @return Length of the string.
 */
inline long ic_string::length() const
{
  return mLength;
}

/**
 * Returns total amount of memory consumed by ic_string.
 * @return Total size of all ic_string's buffers.
 */
inline long ic_string::capacity() const
{
  return mCapacity;
}

/**
 * Append operator.
 * @param right String to be appended.
 * @return Link to this.
 */
inline ic_string &ic_string::operator<<(const ic_string &right)
{
  append(&right);
  return *this;
}

/**
 * Append operator.
 * @param right String to be appended.
 * @return Link to this.
 */
inline ic_string &ic_string::operator<<(const char *right)
{
  append(right);
  return *this;
}

/**
 * Assign operator.
 * @param right String to be assigned.
 * @return Link to this.
 */
inline ic_string &ic_string::operator=(const ic_string &right)
{
  set(&right);
  return *this;
}

/**
 * Assign operator.
 * @param right String to be assigned.
 * @return Link to this.
 */
inline ic_string &ic_string::operator=(const char *right)
{
  set(right);
  return *this;
}

/**
 * Equality operator.
 * @param right String to be compared.
 * @return bool
 */
inline bool ic_string::operator==(ic_string &right)
{
  return compare(&right) == 0;
}

/**
 * Equality operator.
 * @param right String to be compared.
 * @return bool
 */
inline bool ic_string::operator==(const char *right)
{
  return compare(right) == 0;
}

/**
 * Inequality operator.
 * @param right String to be compared.
 * @return bool
 */
inline bool ic_string::operator!=(ic_string &right)
{
  return compare(&right) != 0;
}

/**
 * Inequality operator.
 * @param right String to be compared.
 * @return bool
 */
inline bool ic_string::operator!=(const char *right)
{
  return compare(right) != 0;
}

/**
 * Greater operator.
 * @param right String to be compared.
 * @return bool
 */
inline bool ic_string::operator> (ic_string &right)
{
  return compare(&right) > 0;
}

/**
 * Greater operator.
 * @param right String to be compared.
 * @return bool
 */
inline bool ic_string::operator> (const char *right)
{
  return compare(right) > 0;
}

/**
 * Greater or equal operator.
 * @param right String to be compared.
 * @return bool
 */
inline bool ic_string::operator>= (ic_string &right)
{
  return compare(&right) >= 0;
}

/**
 * Greater or equal operator.
 * @param right String to be compared.
 * @return bool
 */
inline bool ic_string::operator>= (const char *right)
{
  return compare(right) >= 0;
}

/**
 * Less operator.
 * @param right String to be compared.
 * @return bool
 */
inline bool ic_string::operator< (ic_string &right)
{
  return compare(&right) < 0;
}

/**
 * Less operator.
 * @param right String to be compared.
 * @return bool
 */
inline bool ic_string::operator< (const char *right)
{
  return compare(right) < 0;
}

/**
 * Less or equal operator.
 * @param right String to be compared.
 * @return bool
 */
inline bool ic_string::operator<= (ic_string &right)
{
  return compare(&right) <= 0;
}

/**
 * Less or equal operator.
 * @param right String to be compared.
 * @return bool
 */
inline bool ic_string::operator<= (const char *right)
{
  return compare(right) <= 0;
}

/**
 * Indexing operator.
 * @param pos Position of char in string.
 * @return Link to char at specified position.
 */
inline char& ic_string::operator[](long pos)
{
  return char_at(pos);
}

/**
 * Typecast to char* operator.
 * @return Pointer to char buffer.
 */
ic_string::operator char *()
{
  return this->get();
}

#if MALCO_DEBUG == 1
/**
 * Displays debug info about the string.
 */
void ic_string::debug()
{
  ic_strbuffer *curr = mFirst;
  for(int idx=0; idx<mNumBuffers; idx++)
  {
    printf("buf %i: [%s] (%li/%li)\n\n", idx, curr->mBuf, curr->mLength, curr->mCapacity);
    curr = curr->pNext;
  }
}
#endif

#endif
