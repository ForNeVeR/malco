/**
 * @file rc_deftable.h
 * @author impworks.
 * rc_head header.
 * Defines properties and methods of rc_deftable class.
 */

#ifndef RC_DEFTABLE_H
#define RC_DEFTABLE_H

/**
 * rc_deftable constructor.
 */
rc_deftable::rc_deftable(rc_core *core)
{
  pCore = core;
}

/**
 * Inner method declaring a single class from the table.
 * @param idx Row index in the table.
 */
void rc_deftable::declare_class(long id)
{
  // skip prefix letter and start with char #1
  long idx = 1;
  long len = 0;

  // read length and name
  char *str  = mTable.get(id);
  char *name = (char *)(str + idx);
  len        = strlen(name);
  idx += len + 1;

  // read length and parent name
  char *parent = (char *)(str + idx);
  len          = strlen(parent);
  idx += len + 1;

  // read length and parent name
  char *root = (char *)(str + idx);
  len        = strlen(root);
  idx += len + 1;

  // read props
  short props = *(short *)(str + idx);

  rc_class *cls = pCore->class_resolve(root);
  if(cls)
  {
    pCore->class_create(name, pCore->class_resolve(parent), props, cls);
  }
  else
    pCore->error_any(ic_string::format(M_ERR_NO_CLASS, root), M_EMODE_INIT);
}

/**
 * Inner method declaring a single method from the table.
 * @param idx Row index in the table.
 */
void rc_deftable::declare_method(long id)
{
  // skip prefix letter and start with char #1
  long idx = 1;
  long len = 0;

  // read length and name
  char *str  = mTable.get(id);
  char *name = (char *)(str + idx);
  len        = strlen(name);
  idx += len + 1;

  // read length and parent name
  char *parent = (char *)(str + idx);
  len          = strlen(parent);
  idx += len + 1;

  // props, point, min_args, max_args, splat
  short props = *(short *)(str + idx);
  idx += sizeof(short);
  long point = *(long *)(str + idx);
  idx += sizeof(long);
  long min_args = *(long *)(str + idx);
  idx += sizeof(long);
  long max_args = *(long *)(str + idx);
  idx += sizeof(long);
  bool splat = *(bool *)(str + idx);
  idx += sizeof(bool);

  rc_class *cls = pCore->class_resolve(parent);
  if(cls)
  {
    rc_method *method  = pCore->method_add(name, cls, point, props);
    method->mMaxArgs   = max_args;
    method->mMinArgs   = min_args;
    method->mSplatArgs = splat;

    // load argument names
    for(long idx2 = 0; idx2 < max_args; idx2++)
    {
      ic_string *curr = new ic_string((char *)(str + idx));
      idx += curr->length() + 1;

      method->mParams.add(curr);
    }
  }
  else
    pCore->error_any(ic_string::format(M_ERR_NO_CLASS, parent), M_EMODE_INIT);
}

/**
 * Inner method declaring a single property from the table.
 * @param idx Row index in the table.
 */
void rc_deftable::declare_property(long id)
{
  // skip prefix letter and start with char #1
  long idx = 1;
  long len = 0;

  // read length and name
  char *str  = mTable.get(id);
  char *name = (char *)(str + idx);
  len        = strlen(name);
  idx += len + 1;

  // read parent class name
  char *parent = (char *)(str + idx);
  len          = strlen(parent);
  idx += len + 1;

  // read props
  short props = *(short *)(str + idx);

  rc_class *cls = pCore->class_resolve(parent);
  if(cls)
    pCore->member_add(name, cls, props);
  else
    pCore->error_any(ic_string::format(M_ERR_NO_CLASS, parent), M_EMODE_INIT);
}

/**
 * Registers a class in the table.
 * @param name Class name.
 * @param parent Parent class name.
 * @param props Class properties.
 */
void rc_deftable::add_class(const char *name, const char *parent, short props, const char *root)
{
  long name_len = strlen(name), parent_len = strlen(parent), root_len = strlen(root);
  long pos = 0;

  // magic constant assumes prefix character to determine definition type
  // ( 'ñ' = class, 'm' = method, 'p' = property )
  // and two nulls after name and parent.
  char *buf = new char[name_len + parent_len + root_len + sizeof(short) + 3];

  // prefix
  *buf = 'c';
  pos++;

  // (name:char[])
  strcpy(buf + pos, name);
  pos += name_len + 1;

  // (parent:char[])
  strcpy(buf + pos, parent);
  pos += parent_len + 1;

  strcpy(buf + pos, root);
  pos += root_len + 1;

  // (props:short)
  *(short *)(buf + pos) = props;
  pos += sizeof(short);

  // register in the table
  mTable.add_existing(buf, pos);
}

/**
 * Registers a method in the table.
 * @param name Method name.
 * @param cls Class name.
 * @param props Method properties.
 * @param min_args Minimal number of arguments.
 * @param max_args Maximal number of arguments.
 * @param splat Flag saying method accepts any number of arguments.
 * @param names Array of method names as char pointers, zero-ended,
 * names->length() = max_args.
 * @param point Code execution point, relative to current script.
 */
void rc_deftable::add_method(const char *name, const char *cls, short props, long point, long min_args, long max_args,
                             bool splat, sc_voidarray *names)
{
  long idx, pos;
  long total    = 0;
  long *lengths = new long[names->length()];
  long name_len = strlen(name), cls_len = strlen(cls);

  // determine each length
  for(idx = 0; idx < names->length(); idx++)
  {
    lengths[idx] = strlen((char *)names->get(idx)) + 1;
    total += lengths[idx];
  }

  char *buf = new char[name_len + cls_len + sizeof(short) + sizeof(long) * 2 + sizeof(bool) + total + 3];

  // prefix
  *buf = 'm';
  pos  = 1;

  // (name:char[])
  strcpy(buf + pos, name);
  pos += name_len + 1;

  // (parent:char[])
  strcpy(buf + pos, cls);
  pos += cls_len + 1;

  // (props:short) (point:long) (min_args:long) (max_args:long) (splat:bool)
  *(short *)(buf + pos) = props;
  pos += sizeof(short);
  *(long *)(buf + pos) = point;
  pos += sizeof(long);
  *(long *)(buf + pos) = min_args;
  pos += sizeof(long);
  *(long *)(buf + pos) = max_args;
  pos += sizeof(long);
  *(bool *)(buf + pos) = splat;
  pos += sizeof(bool);

  // argument names
  for(idx = 0; idx < names->length(); idx++)
  {
    strcpy(buf + pos, (char *)names->get(idx));
    pos += name_len + 1;
  }

  // register in the table
  delete[] lengths;
  mTable.add_existing(buf, pos);
}

/**
 * Registers a property in the table.
 * @param name Property name.
 * @param cls Class name.
 * @param props Property properties.
 */
void rc_deftable::add_property(const char *name, const char *cls, short props)
{
  long name_len = strlen(name), cls_len = strlen(cls);
  long pos = 0;

  // magic constant assumes prefix character to determine definition type
  // ( 'ñ' = class, 'm' = method, 'p' = property ),
  // a trailing null character and two null bytes after name and class.
  char *buf = new char[name_len + cls_len + sizeof(long) * 2 + sizeof(short) + 3];

  // prefix
  *buf = 'p';
  pos++;

  // (name_len:long) (name:char[name_len])
  strcpy(buf + pos, name);
  pos += name_len + 1;

  // (cls_len:long) (cls:char[cls_len])
  strcpy(buf + pos, cls);
  pos += cls_len + 1;

  // (props:short)
  *(short *)(buf + pos) = props;
  pos += sizeof(short);

  // register in the table
  mTable.add_existing(buf, pos);
}

/**
 * Clears the definition table.
 */
inline void rc_deftable::clear()
{
  mTable.clear();
}

/**
 * Declares the definitions from the table to core.
 * @param core Core pointer.
 */
void rc_deftable::declare(rc_core *core)
{
  for(long idx = 0; idx < mTable.length(); idx++)
  {
    switch(mTable.get(idx)[0])
    {
      case 'c': declare_class(idx); break;
      case 'm': declare_method(idx); break;
      case 'p': declare_property(idx); break;
    }
  }
}

/**
 * Loads table from file.
 * @param name File name.
 */
inline void rc_deftable::file_load(const char *name)
{
  try
  {
    mTable.file_load(name);
  }
  catch(...)
  {
    ERROR(M_ERR_NO_DEFTABLE, M_EMODE_ERROR);
  }
}

/**
 * Saves table to file.
 * @param name File name.
 */
inline void rc_deftable::file_save(const char *name)
{
  try
  {
    mTable.file_save(name);
  }
  catch(...)
  {
    ERROR(M_ERR_NO_DEFTABLE, M_EMODE_ERROR);
  }
}

#endif