/**
 * @file classes.h
 * @author impworks, ForNeVeR.
 * Main file for class definitions.
 * Defines all the built-in classes.
 */

#ifndef CLASSES_H
#define CLASSES_H

//****************************************************************
//*                                                              *
//*                   general predeclarations                    *
//*                                                              *
//****************************************************************

//--------------------------------
//       ic_ classes family
//--------------------------------
// internal classes

class ic_bool;
class ic_int;
class ic_float;

class ic_strbuffer;
class ic_string;

class ic_regex;
class ic_match;
class ic_time;
class ic_lambda;
class ic_range;
class ic_array;

class ic_file;
class ic_dir;
class ic_socket;

class ic_object;

//--------------------------------
//       sc_ classes family
//--------------------------------
// service classes that are solely used
// by radix virtual machine's internals

class sc_exception;

class sc_voiditem;
class sc_voidlist;
class sc_voidarray;

class sc_listitem;
class sc_list;
class sc_stack;
class sc_queue;

class sc_mapitem;
class sc_map;
class sc_voidmapitem;
class sc_voidmap;

template <typename T> class sc_anymapitem;
template <typename T> class sc_anymap;

class sc_ini;
class sc_file;
class sc_md5;
class sc_random;

//--------------------------------
//       rc_ classes family
//--------------------------------
// core classes represent radix itself
// and provide all-round logics

class rc_core;
class rc_lexem;
class rc_parser;
class rc_compiler;
class rc_rasm;

class rc_cmd;
class rc_tape;
class rc_head;
class rc_headstate;

class rc_stritem;
class rc_strtable;
class rc_deftable;

class rc_class;
class rc_method;
class rc_var;
class rc_vartable;

//--------------------------------
//          misc stuff
//--------------------------------

#define STR_MIN_CPC                 256

#define IO_READ                     1
#define IO_WRITE                    2
#define IO_RW                       3
#define FILE_APPEND                 4
#define FILE_REPLACE                8
#define FILE_BINARY                 16

#define PCRE_STATIC                 1
#define REGEX_MAX_MATCHES           10

typedef void(*native_func)(rc_head*);

//****************************************************************
//*                                                              *
//*                      class definitions                       *
//*                                                              *
//****************************************************************

/**
 * @class ic_bool
 * The boolean class.
 * Represents a boolean value and allows default conversions.
 */
class ic_bool
{
  public:
  bool mValue;            /**< Boolean value. */

  ic_bool(bool value=false);

  long to_i();
  double to_f();
  const char *to_s();
};


/**
 * @class ic_bool
 * The integer class.
 * Represents an integer value and allows default conversions.
 */
class ic_int
{
  private:
  char *mStrBuf;          /**< Inner string buffer used to prevent memory leaks in convertors. */

  public:
  long mValue;            /**< Long value. */

  ic_int(long value=0);
  ~ic_int();

  bool to_b();
  double to_f();
  const char *to_s();
};


/**
 * @class ic_float
 * The floating point number class.
 * Represents a float value and allows default conversions.
 */
class ic_float
{
  private:
  char *mStrBuf;          /**< Inner string buffer used to prevent memory leaks in convertors. */

  public:
  double mValue;          /**< Double value. */

  ic_float(double value=0);
  ~ic_float();

  bool to_b();
  long to_i();
  const char *to_s();
};


/**
 * @class ic_strbuffer
 * The buffer class for ic_string.
 * Stores a buffer of actual string information for ic_string.
 */
class ic_strbuffer
{
  friend class ic_string;
  friend class rc_head;
  friend class ic_file;
  friend class ic_socket;
  private:
  char *mBuf;                 /**< Pointer to string buffer. */
  long mLength;               /**< Buffer length. */
  long mCapacity;             /**< Buffer capacity. */
  ic_strbuffer *pPrev;        /**< Pointer to previous buffer. */
  ic_strbuffer *pNext;        /**< Pointer to previous buffer. */

  ic_strbuffer(long cpc);
  ~ic_strbuffer();
};


/**
 * @class ic_string
 * The string class.
 * Represents a flexible string of any length.
 * Allows to perform various operations on the string, such as concatenation,
 * search and replace, splitting, reversing and so.
 * Also is used to load files for later parsing.
 * @todo Make it 100% binary-safe.
 */
class ic_string
{
  friend class rc_head;
  friend class ic_file;
  friend class ic_socket;

  private:
  ic_strbuffer *mFirst;           /**< Pointer to first buffer. */
  ic_strbuffer *mLast;            /**< Pointer to first buffer. */
  int mNumBuffers;                /**< Number of buffers in the string. */
  long mLength;                   /**< Total length of the string. */
  long mCapacity;                 /**< Total capacity of the string. */

  public:
  ic_string();
  ic_string(long cpc);
  ic_string(const char *str, long new_len=0);
  ic_string(const ic_string &str);
  ~ic_string();
  static char* whitespace;

  char *get();
  void empty(long cpc=0);
  void set(const char *src, long new_len=0);
  void set(const ic_string *src, long new_len=0);
  void append(const char *src, long new_len=0);
  void append(const ic_string *src, long new_len=0);
  void append(char src);
  void prepend(const char *src, long new_len=0);
  void prepend(ic_string *src, long new_len=0);
  void prepend(char src);

  void reverse();
  long replace(const char *from, char *to, long max=0);
  long replace(ic_string *from, ic_string *to, long max=0);
  long replace(ic_regex *from, ic_string *to, long max=0);
  void translate(char *from, char *to, long fromlen=0, long tolen=0);
  void translate(ic_string *from, ic_string *to);
  void case_up();
  void case_down();
  void case_swap();
  int compare(const char *str, long len=0);
  int compare(ic_string *str, long len=0);

  char &char_at(long pos) const;
  ic_string *substr_get(long start, long len=0);
  void substr_set(long start, long len, const char *to);
  void substr_set(long start, long len, ic_string *to);
  long substr_first(const char *str, long offset=0);
  long substr_first(ic_string *str, long offset=0);
  long substr_last(const char *str, long offset=0);
  long substr_last(ic_string *str, long offset=0);
  long substr_count(const char *str) const;
  long substr_count(ic_string *str) const;

  sc_voidarray *split(const char *delimiter, long max=0);
  sc_voidarray *split(ic_string *delimiter, long max=0);
  sc_voidarray *split(ic_regex *regex, long max=0);

  void format(sc_voidarray strings);
  static ic_string *format(const char *str, ...);
  static ic_string *format_list(const char *str, va_list args);

  void ltrim();
  void rtrim();
  void trim();

  void file_load(const char *name);
  void file_save(const char *name, char mode=FILE_REPLACE);

  long to_i();
  double to_f();
  bool to_b() const;
  long length() const;
  long capacity() const;

  ic_string &operator<< (const ic_string &right);
  ic_string &operator<< (const char *right);
  ic_string &operator= (const ic_string &right);
  ic_string &operator= (const char *right);
  bool operator== (ic_string &right);
  bool operator== (const char *right);
  bool operator!= (ic_string &right);
  bool operator!= (const char *right);
  bool operator> (ic_string &right);
  bool operator> (const char *right);
  bool operator>= (ic_string &right);
  bool operator>= (const char *right);
  bool operator< (ic_string &right);
  bool operator< (const char *right);
  bool operator<= (ic_string &right);
  bool operator<= (const char *right);
  char &operator[] (long pos);
  operator char* ();

#if MALCO_DEBUG == 1
  void debug();
#endif
};


/**
 * @class ic_regex
 * The regular expression class.
 * Provides methods for regular expression compilation and matching.
 */
#include "external/pcre/include/pcre.h"

class ic_regex
{
  private:
  pcre *mRegEx;                               /**< Pointer to regex structure. */
  pcre_extra *mExtra;                         /**< Pointer to regex extra structure (generated by study method). */
  const char *pError;                         /**< Error message. */
  int mErrorOffset;                           /**< Offset at which regex couldn't compile. */

  int mOptions;                               /**< Options binary flags compressed into an int. */
  char *mPattern;                             /**< Regex pattern. */
  static const unsigned char *mLocaleTables;  /**< Locale tables. */

  public:
  static void init();

  ic_regex();
  ic_regex(const char *pattern);
  ic_regex(ic_regex *regex);
  ~ic_regex();

  void set(const char *pattern);
  ic_match *match(const char *str, int offset=0, long len=0);
  ic_match *match(ic_string *str, int offset=0);

  void study();

  bool to_b();
  char *to_s();

  const char *error();
  int error_pos();

  ic_regex &operator=(const char *right);
  ic_regex &operator=(ic_regex &right);
  bool operator== (ic_regex &right);
  bool operator!= (ic_regex &right);
};

/**
 * @class ic_match
 * The regular expression result class.
 * Provides info about successful regular expression match.
 * Shouldn't be created by the user, it's returned by ic_regex::match instead.
 * @todo implement
 */
class ic_match
{
  friend class ic_regex;

  private:
  long mCount;                                /**< Number of matches found. */
  int mMatches[REGEX_MAX_MATCHES*2];          /**< Found match bounds. */
  const char *pString;                        /**< String to be matched. */
  char *mSubstrings[REGEX_MAX_MATCHES];       /**< Pointers to requested substrings. */
  ic_match();

  public:
  ~ic_match();

  long count();
  const char *get(int id);
  void bounds(int id, long *where);
};


/**
 * @class ic_time
 * The date and time class.
 * Provides methods for time arithmetics, formatting and conversion.
 */
class ic_time
{
  private:
  char *mStrBuf;      /**< Inner string buffer used to prevent memory leaks in convertors. */

  public:
  time_t mTimestamp;  /**< Unix timestamp. */
  struct tm mTime;    /**< Time structure with info broken down across members. */

  ic_time(long tstamp = 0);
  ~ic_time();

  void current();
  void set(long tstamp);
  long get();

  const char *format(const char *format);

  bool to_b();
  long to_i();
  double to_f();
  const char *to_s();

  ic_time &operator=(long right);
  ic_time &operator=(ic_time &right);
  ic_time &operator+(long right);
  ic_time &operator+(ic_time &right);
  ic_time &operator++();
  ic_time &operator++(int wtf);
  ic_time &operator-(long right);
  ic_time &operator-(ic_time &right);
  ic_time &operator--();
  ic_time &operator--(int wtf);
  bool operator==(ic_time &right);
  bool operator!=(ic_time &right);
  bool operator>(ic_time &right);
  bool operator>=(ic_time &right);
  bool operator<(ic_time &right);
  bool operator<=(ic_time &right);
};


/**
 * @class ic_range
 * The range class.
 * Represents an range, allows iterating it and checking values
 * for being inside.
 */
class ic_range
{
  private:
  long mCurr;                           /**< Current iteration item. */
  char *mStrBuf;                        /**< Inner string buffer used to prevent memory leaks in convertors. */

  public:
  long mStart;                          /**< Interval starting value. */
  long mEnd;                            /**< Interval ending value. */
  bool mFinished;                       /**< A flag to indicate whether range iteration has finished. */

  ic_range(const char *str);
  ic_range(long start=0, long end=0);
  ~ic_range();

  void set(const char *str);
  bool contains(long value);
  bool contains(double value);
  int compare(ic_range *obj);

  long length();

  long iter_next();
  void iter_rewind();

  bool to_b();
  long to_i();
  const char *to_s();

  ic_range &operator=(const char *str);
  ic_range &operator=(ic_range &right);
  bool operator==(ic_range &right);
  bool operator!=(ic_range &right);
  bool operator>(ic_range &right);
  bool operator>=(ic_range &right);
  bool operator<(ic_range &right);
  bool operator<=(ic_range &right);
};


/**
 * @class ic_object
 * The object class.
 * Represents a general object in Malco.
 */
class ic_object
{
  friend class rc_core;
  friend class rc_head;
  friend class rc_var;
  private:
  long mLinks;                /**< Number of links to the object. */

  public:
  ic_object(rc_class *root, void *data);
  ~ic_object();

  sc_voidmap *mMembers;       /**< Object members. */
  bool mFrozen;               /**< Flag indicating the object is frozen (no further modifications allowed). */
  bool mTainted;              /**< Flag indicating the object is tainted. */
  void *mData;                /**< Object binary data (ic_*). */
  rc_class *pClass;           /**< Object class. */

  char class_id();
  ic_object *taint(bool tainted);
};


/**
 * @class ic_file
 * The stream class.
 * Represents a file object.
 */
class ic_file
{
  public:
  FILE *mFile;                  /**< File object. */
  char *mName;                  /**< File path / name. */
  char mMode;                   /**< File access mode. */

  ic_file();
  ic_file(const char *name);
  ic_file(ic_string *name);
  ~ic_file();

  void choose(const char *name);
  void choose(ic_string *name);
  bool open(char mode);
  void close();
  ic_string *read(long size);
  bool write(const char *data, long size = 0);
  bool write(ic_string *data, long size = 0);

  bool exists();
  void seek(long pos, char mode);
  long tell();
  bool copy(const char *name);
  bool copy(ic_string *name);
  bool rename(const char *name);
  bool rename(ic_string *name);
  bool remove();
  bool truncate(long size);
  long length();

  bool to_b();
  long to_i();
  char *to_s() const;
};


/**
 * @class ic_dir
 * The directory class.
 * Represents a directory object that can scan folders for content.
 */
class ic_dir
{
  private:
  char *mName;
  long mLength;

  bool copy_r(const char *from, const char *to);
  bool remove_r(const char *name);

  public:
  ic_dir();
  ic_dir(const char *path);
  ic_dir(ic_string *path);

  bool copy(const char *name);
  bool copy(ic_string *name);
  bool rename(const char *name);
  bool rename(ic_string *name);
  bool create();
  bool remove();
  long length();
  
  ic_string *iter_next();
  void iter_rewind();
};


/**
 * @class ic_socket
 * The socket class.
 * Represents a network socket object.
 */
class ic_socket
{
  private:
  char *mName;                  /**< Socket path / name. */
  char mMode;                   /**< Socket access mode. */

  public:
  ic_socket();
  ic_socket(const char *name);
  ic_socket(ic_string *name);
  ~ic_socket();

  bool open(char mode);
  void close();
  ic_string *read(long size);
  bool write(const char *data, long size = 0);
  bool write(ic_string *data, long size = 0);
};


/**
 * @class ic_array
 * The array class.
 * Represents a homogenous dynamic array containing ic_objects.
 * Actually it is a wrapper around sc_voidmap class.
 */
class ic_array
{
  private:
  long mCurrIdx;                /**< Current iteration index. */
  rc_var *mCurr;                /**< Current object. */
  sc_voidmap *mItems;           /**< Array storage. */
  ic_int mAutoIndex;            /**< Automatic index generator. */

  public:
  bool mFinished;               /**< Flag indicating the iteration has finished. */

  ic_array();
  ~ic_array();

  void append(rc_var *obj, bool check = true);
  void set(ic_string *key, rc_var *obj);
  void set(const char *key, rc_var *obj);
  void unset(ic_string *key);
  void unset(const char *key);
  void clear();
  rc_var *get(ic_string *key);
  rc_var *get(const char *key);
  void copy(ic_array *arr);

  sc_voidmapitem *iter_next();
  sc_voidmapitem *iter_last();
  void iter_rewind();

  long length();

  bool to_b();
  long to_i();
  const char *to_s();
};


/**
 * @class sc_exception
 * The exception class.
 * Represents an exception and stores info about what actually happened.
 * Also generates a consistent error message.
 */
class sc_exception
{
  public:
  ic_string *mErrorMsg;         /**< Error message. */
  int mErrorType;               /**< Error level. */
  const char *mFile;            /**< Erroneous file name. */
  long mLine;                   /**< Erroneous line number. */

  sc_exception(const char *msg, int type, const char *file = NULL, long line = 0);
  sc_exception(ic_string *msg, int type, const char *file = NULL, long line = 0);
  ~sc_exception();
};


/**
 * @class sc_voiditem
 * Stores an item of sc_voidlist.
 * Contains a void pointer and is used by sc_voidlist as a bucket.
 */
class sc_voiditem
{
  friend class sc_voidlist;
  sc_voiditem *pPrev;         /**< Previous item in list. */
  sc_voiditem *pNext;         /**< Next item in list. */
  void *mPtr;                 /**< Pointer to whatever data. */

  sc_voiditem(void *ptr);
};


/**
 * @class sc_voidlist
 * The general type list class.
 * Represents a linked list that can later be packed into an array.
 * Is better for generating the list (cheap modification)
 */
class sc_voidlist
{
  public:
  sc_voiditem *mFirst;              /**< First item in list. */
  sc_voiditem *mLast;               /**< Last item in list. */
  long mLength;                     /**< Number of items in list. */

  sc_voidlist();
  ~sc_voidlist();

  void add(void *item);
  void del(void *item);
  void del(long id);
  void* get(long id);
  void* pop();

  sc_voidarray* pack();
};


/**
 * @class sc_voidarray
 * The general type array class.
 * Contains a packed array of void pointers.
 * Is better for iterating the list (cheap indexation)
 */
class sc_voidarray
{
  public:
  void **mPtr;                /**< Array of pointers. */
  sc_voidarray(int size = 0);
  sc_voidarray(const sc_voidarray &obj);
  ~sc_voidarray();

  void add(void *ptr);
  void del(long index);

  void *pop();
  void *&get(long index);
  void set(long index, void *ptr);
  void *&operator[](long index);
  long length() const;

  void resize(long size);

  private:
  long mLength;               /**< Number of items in array. */
  long mSize;                 /**< Number of items the array is allocated for (capacity). */

  friend sc_voidarray* sc_voidlist::pack();
  friend long ic_string::replace(ic_regex *from, ic_string *to, long max);
  friend class rc_tape;
  friend class rc_strtable;
};


/**
 * @class sc_listitem
 * Bucket for sc_list.
 */
class sc_listitem
{
  public:
  sc_listitem *mPrev;             /**< Next object in list. */
  sc_listitem *mNext;             /**< Previous object in list. */
  rc_var *pObj;                   /**< Pointer to object. */

  sc_listitem();
};


/**
 * @class sc_list
 * The object list class.
 * Basic class for object lists.
 */
class sc_list
{
  public:
  sc_listitem *mFirst;              /**< First object in list. */
  sc_listitem *mLast;               /**< Last object in list. */
  sc_listitem *mCurr;               /**< Current item for iterations. */
  long mLength;                     /**< Length of the list. */
  long mCurrIdx;                    /**< Current item's ID. */

  sc_list();
  rc_var * pop();
  void push(rc_var *ptr);

  void iter_rewind();
  rc_var *iter_next();

  rc_var &operator[](long idx);
  rc_var *get(long idx);
};


/**
 * @class sc_stack
 * The object stack class.
 * Stacks ic_objects. Is used as internal RVM stacks.
 */

class sc_stack: public sc_list
{
  public:
  void push(rc_var  *ptr);
};


/**
 * @class sc_queue
 * The object queue class.
 * Queues ic_objects. Is used as internal RVM queues (accumulators).
 */
class sc_queue: public sc_list
{
  public:
  void push(rc_var  *ptr);
};


/**
 * @class sc_mapitem
 * Stores a record of sc_map.
 * A sc_map tree node containing it's key and value as ic_strings.
 */
class sc_mapitem
{
  public:
  int mBalance;           /**< Balance of the current item. */
  sc_mapitem *mLink[2];   /**< Links to left and right children. */
  sc_mapitem *pRoot;      /**< Link to parent item. */
  ic_string *mKey;        /**< Key. */
  ic_string *mValue;      /**< Value. */
  sc_mapitem *pPrev;      /**< Pointer to previous item. */
  sc_mapitem *pNext;      /**< Pointer to next item. */

  sc_mapitem();
  sc_mapitem(const char *key, const char *val);
  ~sc_mapitem();
};


/**
 * @class sc_map
 * The hash table / map class.
 * Provides a hash table with both keys and values being ic_strings.
 * Is implemented as an AVL tree.
 */
class sc_map
{
  private:
  long mLength;  /**< Length of the map. */

  // tree rotation methods
  void adjust_balance(sc_mapitem *root, int dir, int bal);
  sc_mapitem *rotate_single(sc_mapitem *root, int dir);
  sc_mapitem *rotate_double(sc_mapitem *root, int dir);

  sc_mapitem *insert(const char *key, const char *val);
  sc_mapitem *insert_r(sc_mapitem *root, const char *key, const char *val, int *done, sc_mapitem **item);
  sc_mapitem *insert_balance(sc_mapitem *root, int dir);

  sc_mapitem *remove_r(sc_mapitem *root, const char *key, int *done);
  sc_mapitem *remove_balance(sc_mapitem *root, int dir, int *done);

  void clear_r(sc_mapitem *item);

  sc_mapitem *find(const char *key);

  public:
  sc_mapitem *mRoot;      /**< Root item of the tree. */
  sc_mapitem *mCurr;      /**< Current item for iteration. */
  sc_mapitem *mCurrKey;   /**< Current item for key-based iteration. */
  sc_mapitem *mFirst;     /**< First item of the linked list. */
  sc_mapitem *mLast;      /**< Last item of the linked list. */
  sc_map();
  sc_map(sc_map &map);
  ~sc_map();

  void set(const char *key, const char *value);
  void set(const char *key, ic_string *value);
  void set(ic_string *key, ic_string *value);
  char *get(const char *key);
  char *get(ic_string *key);
  void remove(const char *key);
  void remove(ic_string *key);

  void clear();

  void iter_rewind();
  sc_mapitem *iter_next();
  void key_rewind();
  sc_mapitem *key_next();

  long length();

  sc_voidarray *get_keys();

  ic_string &operator[](const char *key);
  ic_string &operator[](ic_string &key);
  ic_string &operator[](ic_string *key);

#if MALCO_DEBUG == 1
  void debug(sc_mapitem *item = NULL);
#endif
};



/**
 * @class sc_voidmapitem
 * Stores a record of sc_voidmap.
 * A sc_voidmap tree node containing it's key as ic_string and value as void *.
 */
class sc_voidmapitem
{
  public:
  int mBalance;               /**< Balance of the current item. */
  sc_voidmapitem *mLink[2];   /**< Links to left and right children. */
  sc_voidmapitem *pRoot;      /**< Link to parent item. */
  ic_string *mKey;            /**< Key. */
  void *mValue;               /**< Value. */
  sc_voidmapitem *pPrev;      /**< Pointer to previous item. */
  sc_voidmapitem *pNext;      /**< Pointer to next item. */

  sc_voidmapitem();
  sc_voidmapitem(const char *key, void *val);
  ~sc_voidmapitem();
};


/**
 * @class sc_voidmap
 * The hash table / map class.
 * Provides a hash table with keys being ic_strings and values being void *.
 * Is implemented as an AVL tree.
 */
class sc_voidmap
{
  private:
  long mLength;               /**< Length of the map. */

  // tree rotation methods
  void adjust_balance(sc_voidmapitem *root, int dir, int bal);
  sc_voidmapitem *rotate_single(sc_voidmapitem *root, int dir);
  sc_voidmapitem *rotate_double(sc_voidmapitem *root, int dir);

  sc_voidmapitem *insert(const char *key, void *val);
  sc_voidmapitem *insert_r(sc_voidmapitem *root, const char *key, void *val, int *done, sc_voidmapitem **item);
  sc_voidmapitem *insert_balance(sc_voidmapitem *root, int dir);

  sc_voidmapitem *remove_r(sc_voidmapitem *root, const char *key, int *done);
  sc_voidmapitem *remove_balance(sc_voidmapitem *root, int dir, int *done);

  void clear_r(sc_voidmapitem *item);

  public:
  sc_voidmapitem *mRoot;      /**< Root item of the tree. */
  sc_voidmapitem *mCurr;      /**< Current item for iteration. */
  sc_voidmapitem *mCurrKey;   /**< Current item for key-based iteration. */
  sc_voidmapitem *mFirst;     /**< First item of the linked list. */
  sc_voidmapitem *mLast;      /**< Last item of the linked list. */

  sc_voidmap();
  sc_voidmap(sc_voidmap &map);
  ~sc_voidmap();

  sc_voidmapitem *find(const char *key);
  void set(const char *key, void *value);
  void set(ic_string *key, void *value);
  void *get(const char *key);
  void *get(ic_string *key);
  void remove(const char *key);
  void remove(ic_string *key);

  void clear();

  void iter_rewind();
  sc_voidmapitem *iter_next();
  void key_rewind();
  sc_voidmapitem *key_next();

  long length();

  sc_voidarray *get_keys();

  void *operator[](const char *key);
  void *operator[](ic_string &key);
  void *operator[](ic_string *key);

#if MALCO_DEBUG == 1
  void debug(sc_voidmapitem *item = NULL);
#endif
};

/**
 * @class sc_anymapitem
 * Templated map item class.
 */
template <typename T>
class sc_anymapitem
{
  public:
  int mBalance;               /**< Balance of the current item. */
  sc_anymapitem *mLink[2];    /**< Links to left and right children. */
  sc_anymapitem *pRoot;       /**< Link to parent item. */
  ic_string *mKey;            /**< Key. */
  T *mValue;                  /**< Value. */
  sc_anymapitem *pPrev;       /**< Pointer to previous item. */
  sc_anymapitem *pNext;       /**< Pointer to next item. */

  sc_anymapitem();
  sc_anymapitem(const char *key, T *val);
  ~sc_anymapitem();
};


/**
 * @class sc_anymap
 */
template <typename T>
class sc_anymap
{
private:
  long mLength;               /**< Length of the map. */

  // tree rotation methods
  void adjust_balance(sc_anymapitem<T> *root, int dir, int bal);
  sc_voidmapitem *rotate_single(sc_anymapitem<T> *root, int dir);
  sc_voidmapitem *rotate_double(sc_anymapitem<T> *root, int dir);

  sc_voidmapitem *insert(const char *key, void *val);
  sc_voidmapitem *insert_r(sc_anymapitem<T> *root, const char *key, T *val, int *done, sc_anymapitem<T> **item);
  sc_voidmapitem *insert_balance(sc_anymapitem<T> *root, int dir);

  sc_voidmapitem *remove_r(sc_anymapitem<T> *root, const char *key, int *done);
  sc_voidmapitem *remove_balance(sc_anymapitem<T> *root, int dir, int *done);

  void clear_r(sc_anymapitem<T> *item);

  sc_anymapitem<T> *find(const char *key);

public:
  sc_anymapitem<T> *mRoot;      /**< Root item of the tree. */
  sc_anymapitem<T> *mCurr;      /**< Current item for iteration. */
  sc_anymapitem<T> *mCurrKey;   /**< Current item for key-based iteration. */
  sc_anymapitem<T> *mFirst;     /**< First item of the linked list. */
  sc_anymapitem<T> *mLast;      /**< Last item of the linked list. */

  sc_anymap();
  sc_anymap(sc_anymap &map);
  ~sc_anymap();

  void set(const char *key, T *value);
  void set(ic_string *key, T *value);
  T *get(const char *key);
  T *get(ic_string *key);
  void remove(const char *key);
  void remove(ic_string *key);

  void clear();

  void iter_rewind();
  sc_anymapitem<T>  *iter_next();
  void key_rewind();
  sc_anymapitem<T> *key_next();

  long length();

  sc_voidarray *get_keys();

  T *operator[](const char *key);
  T *operator[](ic_string &key);
  T *operator[](ic_string *key);

#if MALCO_DEBUG == 1
  void debug(sc_anymapitem<T> *item = NULL);
#endif
};

/**
 * @class sc_ini_section
 * Stores a section for sc_ini.
 * Stores a map of parameters and values inside a specific INI section.
 */
class sc_ini_section
{
  public:
  sc_ini_section(const char *name);
  ~sc_ini_section();
  sc_map params;
  char *mName;
};


/**
 * @class sc_ini
 * The INI-file parser class.
 * Allows loading, parsing and saving configuration files (*.INI).
 * Is used for loading Malco's default values.
 */
class sc_ini
{
  public:
  sc_ini();
  sc_ini(const char *filename);
  ~sc_ini();

  void file_load(const char *filename);
  void file_save(const char *filename = NULL);

  sc_voidarray *get_sections();
  sc_voidarray *get_params(const char *section_name = NULL);
  ic_string *get_value(const char *param_name, const char *section_name = NULL, const char *default_value = "");

  void set_value(const char *param_name, const char *value, const char *section_name = NULL);
  void del_param(const char *param_name, const char *section_name = NULL);
  void del_section(const char *section_name = NULL);

  private:
  char *mFilename;

  long find_section(const char *section_name);
  sc_ini_section *add_section(const char *section_name);

  sc_voidarray sections; // array of pointers to sc_ini_section objects.
  long section_count;
  char *last_section_name;

  sc_ini(const sc_ini &); // non-copyable!
};


/**
 * @class sc_md5
 * The MD5 hashing class.
 * Provides methods for generating MD5 hashes from strings and files.
 */
typedef unsigned uint32_t;

class sc_md5
{
  private:
  uint32_t mI[2];
  uint32_t mBuf[4];
  unsigned char mIn[64];        /**< Input buffer. */
  unsigned char mDigest[16];    /**< Internal MD5 digest. */
  char mReadable[33];           /**< Human-readable MD5 digest. */

  void init();
  void update(const char *buf, int len);
  void transform(uint32_t *buf, uint32_t *in);
  void finish();
  char *make_readable();

  public:
  char *string(ic_string *str);
  char *string(const char *str, int len);
  char *file(ic_string *file);
  char *file(const char *str);
};

/**
 * @class sc_file
 * Basic file operations class.
 * Provides methods for removing, renaming and testing files.
 */
#include <sys/stat.h>
class sc_file
{
  private:
  static char mStrBuf[16];

  public:
  static long time(const char *name);
  static long time(ic_string *name);

  static bool exists(const char *name);
  static bool exists(ic_string *name);

  static bool rename(const char *oldname, const char *newname);
  static bool rename(ic_string *name, ic_string *newname);

  static bool remove(const char *name);
  static bool remove(ic_string *name);

  static char *checksum(const char *name);
  static char *checksum(ic_string *name);
};

/**
 * @class sc_random
 * Pseudorandom number generator based on the Mersenne twister algorithm.
 */
class sc_random
{
  public:
  static void seed(int new_seed);

  static int get_next();
  static int get_next(int max_value);
  static int get_next(int min_value, int max_value);

  private:
  static int state[624]; // 624 is magic number
  static int index;

  static void generate();
};


/**
 * @class rc_core
 * The Radix core class.
 * Provides all methods of Radix VM's core.
 */
class rc_core
{
  public:
  int mTask;                    /**< Task ID (what Malco was called for). */
  int mRunMode;                 /**< Current run mode. */
  int mErrorMode;               /**< Current error reporting mode. */
  int mState;                   /**< Current state of execution. */
  clock_t mStartTime;           /**< Radix start of execution. */
  ic_string *mFile;             /**< Root file to be executed. */

  sc_ini *mSetup;               /**< Configuration. */
  rc_parser *mParser;           /**< Source code parser pointer. */
  rc_compiler *mCompiler;       /**< Compiler pointer. */
  rc_rasm *mRasm;               /**< Radix assembler pointer. */

  ic_string *mSource;           /**< Source script. */
  rc_head *mHead;               /**< Execution head. */
  rc_tape *mTape;               /**< Execution tape. */
  rc_strtable *mStrTable;       /**< Constant string table. */
  rc_class *pClassRoot;         /**< Root class for object hierarchy. */
  
  sc_voidmap *mPlugins;         /**< Core plugins. */

  struct rc_classcache
  {
    rc_class *pObject;
    rc_class *pUndef;
    rc_class *pBool;
    rc_class *pInt;
    rc_class *pFloat;
    rc_class *pString;
    rc_class *pRange;
    rc_class *pRegex;
    rc_class *pMatch;
    rc_class *pTime;
    rc_class *pFile;
    rc_class *pDir;
    rc_class *pSocket;
    rc_class *pMethod;
    rc_class *pClass;
    rc_class *pArray;
    rc_class *pException;
  } mClassCache;                /**< Cache for built-in class pointers. */

  rc_core();
  ~rc_core();

  void init();
  int process(int argc, char *argv[]);
  void equip();

  // general core commands
  rc_class *class_create(const char *name, rc_class *parent = NULL, short properties = 0, rc_class *root = NULL);
  rc_class *class_resolve(const char *name, rc_class *root = NULL);
  bool class_is_heir(rc_class *final, rc_class *base);
  sc_voidarray *class_parents(rc_class *cls);
  int class_type(rc_class *cls);

  rc_method *method_add(const char *name, rc_class *root, long start, short properties);
  rc_method *method_add(const char *name, rc_class *root, native_func func, short properties);
  rc_method *op_resolve(const char *name, rc_class *ax, rc_class *bx);

  void member_add(const char *name, rc_class *root, short properties);
  
  // error reporting
  void error(const char *msg);
  void error(ic_string *msg);
  void error(sc_exception ex);
  void error(const char *msg, int mode, rc_head *head);
  void error_any(ic_string *msg, int mode);

  private:
  int cmdline_task(const char *task);

  // malco tasks
  int task_compile(const char *file);
  int task_run(const char *file);
  int task_run_bc(const char *file);
  int task_eval(const char *code);
  int task_version();
  int task_credits();

  void plugins_load();
};


/**
 * @class rc_head
 * The Radix execution head class.
 * Represents an "execution head" class that traverses the bytecode "tape" and executes commands one by one.
 * It is defined as a separate class to make Malco easier to convert to multithreading when the time comes.
 */
class rc_head
{
  public:
  char mFile[255];              /**< Current source file. */
  long mLine;                   /**< Current source line. */
  long mOffset;                 /**< Current command index. */
  rc_cmd *pCmd;                 /**< Current command. */

  rc_core *pCore;
  long mStatLines;              /**< Number of lines processed. */
  long mStatFiles;              /**< Number of files processed. */
  long mStatCommands;           /**< Number of commands processed. */
  clock_t mStartTime;           /**< Time of execution. */

  // register
  rc_var *rAX;                  /**< AX register. */

  // accumulators
  sc_queue rSRC;                /**< IN accumulator. */
  sc_queue rDST;                /**< Assign variable queue. */
  sc_stack rUS;                 /**< User stack. */
  sc_voidarray rCS;             /**< Call stack. */
  sc_voidlist rSS;              /**< Safe zone stack. */

  // location info
  sc_voidarray *mVars;          /**< Variable table for current execution point. */
  rc_var *pCurrObj;             /**< Current object for casual execution. */
  rc_class *pCurrClass;         /**< Current class / namespace for static execution. */
  rc_class *pTmpClass;          /**< Temporarily selected class for calling stuff from sub-namespaces. */

  bool mExternalScope;          /**< Flag indicating that scope should not be removed. */

  rc_head(rc_core *core);
  ~rc_head();

  // main commands
  int playback(bool main = true);
  void execute();

  void state_save();
  void state_load();
  rc_method *method_resolve(const char *name, rc_class *root);
  void method_invoke(const char *name, rc_var *object = NULL, bool report=true);
  void method_invoke(rc_method *method, rc_var *object = NULL);

  void nsp_select(const char *name);
  void nsp_flush();

  void var_save(rc_var *var, ic_object *obj);
  void var_save(rc_var *var, rc_var *obj);
  ic_object *var_get(rc_var *var);

  rc_var *scope_get(long id);
  rc_var *bare_id_resolve(const char *name);
  rc_var *member_resolve(const char *name, rc_var *obj, rc_class *cls);

  void members_release(rc_var *obj);
  
  void warning(const char *msg, ...);
  void notice(const char *msg, ...);

  void exception(const char *msg, int type = M_EXC_INTERNAL);
  void exception(ic_string *msg, int type = M_EXC_INTERNAL);

  // general object commands
  ic_string *obj_inspect(rc_var *obj);
  rc_var *obj_create(rc_class *objclass);
  rc_var *obj_clone(rc_var *obj);
  void obj_unlink(rc_var *obj);
  void obj_unlink(ic_object *obj);

  // conversion commands
  rc_var *convert_bool(rc_var *var);
  rc_var *convert_string(rc_var *var);
  rc_var *convert_int(rc_var *var);

  // built-in object creation commands
  void *new_basic(char type);
  rc_var *new_undef(bool tainted = false);
  rc_var *new_bool(bool value, bool tainted = false);
  rc_var *new_bool(ic_bool *value, bool tainted = false);
  rc_var *new_int(long value, bool tainted = false);
  rc_var *new_int(ic_int *value, bool tainted = false);
  rc_var *new_float(double value, bool tainted = false);
  rc_var *new_float(ic_float *value, bool tainted = false);
  rc_var *new_string(const char *string, bool tainted = false);
  rc_var *new_string(ic_string *string, bool tainted = false);
  rc_var *new_regex(ic_regex *regex, bool tainted = false);
  rc_var *new_match(ic_match *match, bool tainted = false);
  rc_var *new_range(long start, long end, bool tainted = false);
  rc_var *new_range(ic_range *range, bool tainted = false);
  rc_var *new_time(long stamp, bool tainted = false);
  rc_var *new_time(ic_time *time, bool tainted = false);
  rc_var *new_method(rc_method *method, bool tainted = false);
  rc_var *new_class(rc_class *cls, bool tainted = false);
  rc_var *new_array(ic_array *arr, bool tainted = false);
  rc_var *new_array(bool tainted = false);
  rc_var *new_exception(const char *msg, int type, bool tainted = false);
  rc_var *new_exception(ic_string *msg, int type, bool tainted = false);

  // data transfer commands
  void cmd_loadax();
  void cmd_loadbx();
  void cmd_saveax();
  void cmd_savebx();
  void cmd_xchg();
  void cmd_assign();
  void cmd_unsplassign();

  // basic operations
  void cmd_add();
  void cmd_sub();
  void cmd_div();
  void cmd_mul();
  void cmd_mod();
  void cmd_pow();
  void cmd_shl();
  void cmd_shr();
  void cmd_band();
  void cmd_bor();
  void cmd_bxor();
  void cmd_and();
  void cmd_or();
  void cmd_xor();
  void cmd_index();

  // unary operations
  void cmd_inc();
  void cmd_dec();
  void cmd_neg();

  // comparison, jumps
  void cmd_eq();
  void cmd_eq_strict();
  void cmd_rel();
  void cmd_less();
  void cmd_less_eq();
  void cmd_greater();
  void cmd_greater_eq();
  void cmd_cmp();
  void cmd_jtrue();
  void cmd_jfalse();
  void cmd_jmp();

  // stack and accumulator commands
  void cmd_pushus();
  void cmd_popus();
  void cmd_pushsrc();
  void cmd_popsrc();
  void cmd_pushdst();
  void cmd_popdst();
  void cmd_splat();
  void cmd_unsplat();
  void cmd_movesrc();
  void cmd_movedst();
  void cmd_clrsrc();
  void cmd_clrdst();

  // object manipulation
  void cmd_new();
  void cmd_call();
  void cmd_return();
  void cmd_nsp();
  void cmd_bindlambda();

  // interpreter behavior modificators
  void cmd_include();
  void cmd_require();
  void cmd_gc();
  void cmd_setpty();
  void cmd_setfile();
  void cmd_setline();
  void cmd_throw();
  void cmd_try();
  void cmd_tried();

  // class table generator commands
  void cmd_regclass();
  void cmd_regproperty();
  void cmd_regmethod();

  // debug commands
  void cmd_inspect();
  
  // sub-functions
  void sub_math_op(const char *name);
  void sub_logical_op(char name);
  char sub_compare(bool strict = false);
  char sub_compare(rc_var *left, rc_var *right, bool strict = false);
  bool sub_value(rc_var *var);
};

/**
 * @class rc_headstate
 * The class to store head state.
 * Stores registers of a head for restoring them after method invocation.
 */
class rc_headstate
{
  public:
  rc_var *rAX;                  /**< AX register state. */

  long mOffset;                 /**< State's command offset. */

  sc_voidarray *mVars;          /**< Variable table for state's execution point. */
  rc_class *pStateClass;        /**< State's local namespace for execution. */
  rc_class *pStateTmpClass;     /**< State's class / namespace for execution. */
  rc_var *pStateObj;            /**< State's object for execution. */

  bool mExternalScope;          /**< State's flag indicating that local scope should not be deleted. */
};


/**
 * @class rc_lexem
 * The Radix parser lexem class.
 */
class rc_lexem
{
  public:
  char mType;                         /**< Lexem type. */
  long mLine;                         /**< Lexem line. */
  long mOffset;                       /**< Lexem position in line. */
  long mParam;                        /**< Item ID in constant string table. */

  rc_lexem(char type, long line, long offset, long param = 0);
};


/**
 * @class rc_parser
 * The Radix parser class.
 * Parses source code and generates a stream of lexems. Also generates constant value tables.
 */
class rc_parser
{
  public:
  sc_voidarray mLexems;               /**< Lexem stream. */
  long mLength;                       /**< Total length of the tape. */
  long mLastLength;                   /**< Length of the tape's last buffer. */
  long mLastBuf;                      /**< Number of buffers used in tape. */

  rc_parser();
  ~rc_parser();
};


/**
 * @class rc_compiler
 * The Radix compiler class.
 * Compiles lexem stream into Radix bytecode.
 * Also provides basic optimization.
 */
class rc_compiler
{
  public:
};


/**
 * @class rc_optimizer
 * The Radix optimizer class.
 * Optimizes the bytecode (if possible).
 */
class rc_optimizer
{
  public:
};


/**
 * @class rc_command
 * The RVM command class.
 * Stores information about a single command: it's type, table modifier and operand address.
 */
class rc_cmd
{
  public:
  unsigned char mCmd;         /**< Command code. */
  unsigned char mModifier;    /**< Parameter modifier. */
  union
  {
    long addr;
    double val;
  } mParam;                   /**< Parameter. */
};


/**
 * @class rc_tape
 * The RVM command tape class.
 * Stores all the commands of the current executed script as buffers.
 */
#define TAPE_BUF_SIZE       1024
#define TAPE_CHUNK_SIZE     8
class rc_tape
{
  private:
  sc_voidarray mTape;                 /**< Array of pointers. */
  long mLength;                       /**< Total length of the tape. */
  long mLastLength;                   /**< Length of the tape's last buffer. */
  long mLastBuf;                      /**< Number of buffers used in tape. */

  public:
  rc_tape();
  ~rc_tape();

  void clear();
  void add(rc_cmd *cmd, long num = 1);

  rc_cmd *select(long idx);

  long length();

  void file_load(const char *name);
  void file_save(const char *name);

  rc_cmd *operator[](long idx);

#if MALCO_DEBUG == 1
  void debug();
#endif
};


/**
 * @class rc_stritem
 * The RVM string table item class.
 * Stores a constant string and it's length.
 */
class rc_stritem
{
  friend class rc_strtable;
  private:
  char *mString;                      /**< String. */
  long mLength;                       /**< String precomputed length. */

  rc_stritem();
  ~rc_stritem();
};


/**
 * @class rc_strtable
 * The RVM string table class.
 * Stores all constant strings for current script.
 */
#define STRTABLE_BUF_SIZE       128
#define STRTABLE_CHUNK_SIZE     8
class rc_strtable
{
  private:
  sc_voidarray mTable;                /**< Array of pointers. */
  long mLength;                       /**< Total length of the table. */
  long mLastLength;                   /**< Length of the table's last buffer. */
  long mLastBuf;                      /**< Number of buffers used in table. */

  public:
  rc_strtable();
  ~rc_strtable();

  long add(const char *str, long len = 0);
  long add(ic_string *str, long len = 0);
  long add_existing(char *str, long len = 0);
  long find(const char *str);
  long find(ic_string *str);

  void clear();

  char *get(long idx);

  long length();
  void file_load(const char *name);
  void file_save(const char *name);

  char *operator[](long idx);
};


/**
 * @class rc_deftable
 * The RVM definition table class.
 * Contains information about classes, methods and properties in compiled form.
 */
class rc_deftable
{
  private:
  rc_strtable mTable;               /**< Underlying string table to store formatted data. */
  rc_core *pCore;                   /**< Pointer to core. */

  void declare_class(long idx);
  void declare_method(long idx);
  void declare_property(long idx);

  public:
  rc_deftable(rc_core *core);

  void add_class(const char *name, const char *parent, short props, const char *root);
  void add_method(const char *name, const char *cls, short props, long point, long min_args, long max_args, bool splat, sc_voidarray *names);
  void add_property(const char *name, const char *cls, short props);

  void clear();

  void declare(rc_core *core);

  void file_load(const char *name);
  void file_save(const char *name);
};


/**
 * @class rc_class
 * The RVM class class.
 * Stores information about a class for RVM to operate with.
 * The information includes tables of methods, members, etc.
 */
class rc_class
{
  public:
  char *mName;                  /**< Class name. */
  char *mFullName;              /**< Full class name, from root to current class. */
  rc_class *pParent;            /**< Pointer to parent class. */
  rc_class *pRoot;              /**< Pointer to root class. */
  long mNumObjs;                /**< Total count of objects of this class. */
  short mProperties;            /**< Bit set of class properties. */

  sc_voidmap mClasses;          /**< Subclass table (rc_class). */
  sc_voidmap mMethods;          /**< Method table (rc_method). */
  sc_voidmap mMembers;          /**< Members table (rc_var). */
  sc_voidmap mStaticMembers;    /**< Static members table (rc_var). */
  int mDataType;                /**< Indicates ic_basic type if this class derives from a basic class. */
};


/**
 * @class rc_method
 * The method class.
 * Represents a single method of a class or a lambda.
 * To be used by RVM when the specified method is called.
 */
class rc_method
{
  public:
  const char *mName;              /**< Method name. */
  rc_class *pClass;               /**< Class this method belongs to. */
  short mProperties;              /**< Bit set of method properties. */
  sc_voidarray mParams;           /**< Parameter names. */

  int mMinArgs;                   /**< Minimum number of acceptable arguments. */
  int mMaxArgs;                   /**< Maximum number of acceptable arguments. */
  bool mSplatArgs;                /**< A flag indicating unlimited number of arguments (they're splatted). */

  bool mNative;                   /**< A flat indicating whether method is written in Malco or in C++. */
  long mExecPoint;                /**< Start of method's code (Malco mode). */
  native_func pNativeFunc;        /**< Pointer to native function (Native mode). */

  sc_voidarray *pExternalScope;   /**< Used in case method is a lambda and is executed in parent scope. */

  void setup(int min, int max = 0, bool splat = false, ...);
  void op();
};


/**
 * @class rc_var
 * The variable class.
 * Represents a single variable holding an object.
 * Variables are object containers, implementing link logic.
 */
class rc_var
{
  public:
  //char *mName;          /**< Variable caption. */
  void *pObj;           /**< Object the variable points at. */
  short mProperties;    /**< Bit set of variable properties. */
  long mLinks;

  //rc_var *pBaseVar;     /**< Base variable (if this variable is a property). */
  //rc_class *pBaseClass; /**< Class the variable belongs to. */

  rc_var();
  rc_var(ic_object *obj);
  rc_var(rc_var *obj);

  ic_object *get();
};


/**
 * @class rc_rasm
 * The Radix assembler class.
 * Assembles RASM file into Radix bytecode.
 */
class rc_rasm
{
  public:
  rc_rasm(rc_core *core, const ic_string *code, const char *filename);
  ~rc_rasm();

  void assemble(const ic_string *path);

  // Routines to get information on error cases:
  int get_error_line_number() const;
  ic_string *get_error_filename();

  private:
  rc_core *pCore;                 // Core instance.

  rc_tape mTape;                  // Tape for compiled code.
  rc_deftable mDefTable;          // Definition table.
  rc_strtable mStrTable;          // String table.
  
  ic_string mCode;                // Source code.
  ic_string mFileName;            // Source file name.
  int mLineNumber;                // Source line number.
  sc_voidarray mVariables;        // Local variable names (as char *).
  sc_voidmap mLabelMap;           // label_name => (void *)command_number.
  sc_voidarray mClassNameStack;   // Stack for class names (as char *).
  ic_string *mCurrentMethodName;  // Last not closed method name.

  void process_line(ic_string *line);
  ic_string *get_current_class();
  long find_variable(ic_string *name);

  static ic_string *clean_line(ic_string *line);
  static int get_command_code(ic_string *command);
  
  static bool is_command(ic_string *line);
  static bool is_label(ic_string *line);

  static sc_voidarray split_into_primitives(ic_string *line);

  static ic_string *extract_command_name(ic_string *line);
  static short extract_modifiers(ic_string *line);

  static bool has_parameter(ic_string *line, int index=0);
  static ic_string *extract_raw_parameter(ic_string *line, int index=0);
  static int extract_int_parameter(ic_string *line, int index=0);
  static double extract_float_parameter(ic_string *line, int index=0);
  static ic_string *extract_string_parameter(ic_string *line, int index=0);

  static bool is_parameter_int(ic_string *line, int index=0);
  static bool is_parameter_float(ic_string *line, int index=0);
  static bool is_parameter_string(ic_string *line, int index=0);

  static void solve_escape_seq(ic_string *line);
};

#endif
