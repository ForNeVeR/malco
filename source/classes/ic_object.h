/**
 * @file ic_object.h
 * @author impworks.
 * ic_object header.
 * Defines properties and methods of ic_object class.
 */

#ifndef IC_OBJECT_H
#define IC_OBJECT_H

/**
 * ic_object constructor.
 * @param root Class of the object.
 * @param data Pointer to ic_basic object for object to store.
 */
ic_object::ic_object(rc_class *root, void *data)
{
  mLinks = 1;
  pClass = root;
  mData = data;
  mFrozen = mTainted = false;

  mMembers = new sc_voidmap(root->mMembers);

  root->mNumObjs++;
}

/**
 * ic_object destructor.
 */
ic_object::~ic_object()
{
  delete mData;
  delete mMembers;

  pClass->mNumObjs--;
}

/**
 * Returns an ID of a built-in class, if the object stores one.
 * @return Class ID.
 */
#define OBJCLASS(cls) !strcmp(pClass->mName, cls)
char ic_object::class_id()
{
  if(OBJCLASS("undef")) return M_CLASS_UNDEF;
  if(OBJCLASS("bool")) return M_CLASS_BOOL;
  if(OBJCLASS("int")) return M_CLASS_INT;
  if(OBJCLASS("float")) return M_CLASS_FLOAT;
  if(OBJCLASS("string")) return M_CLASS_STRING;
  if(OBJCLASS("range")) return M_CLASS_RANGE;
  if(OBJCLASS("time")) return M_CLASS_TIME;
  if(OBJCLASS("method")) return M_CLASS_METHOD;
  if(OBJCLASS("class")) return M_CLASS_CLASS;
  if(OBJCLASS("array")) return M_CLASS_ARRAY;

  return M_CLASS_OTHER;
}
#undef OBJCLASS

/**
 * Mark object as tainted (for inline tainting).
 */
inline ic_object *ic_object::taint(bool tainted)
{
  mTainted = tainted;
  return this;
}

#endif
