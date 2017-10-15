/**
 * @file m_class.h
 * @author impworks
 * Class method header.
 * Defines all methods for class class.
 */

#ifndef M_CLASS_H
#define M_CLASS_H

/**
 * Class constructor.
 */
void class_op_create(rc_head *head)
{
  head->exception(M_ERR_CLASS_CONSTRUCTOR, M_EXC_SCRIPT);
}

void class_op_cmp_class(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();

  // we think that 'parent' is always bigger than 'child'
  // you can think of 'bigger' as 'more global, abstract'
  rc_class *left = (rc_class *)obj->mData, *right = (rc_class *)item->mData;
  if(left == right)
    head->rSRC.push(head->new_int(0L));
  else if(head->pCore->class_is_heir(left, right))
    head->rSRC.push(head->new_int(1));
  else if(head->pCore->class_is_heir(right, left))
    head->rSRC.push(head->new_int(-1));
  else
    head->rSRC.push(head->new_undef());

  head->obj_unlink(item_var);
}

void class_op_cmp_bool(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  // a class cannot be 'false'
  bool left = true, right = ((ic_bool *)item->mData)->mValue;
  char result = (left == right ? 0 : 1);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

void class_op_cmp_string(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  const char *str  = ((rc_class *)obj->mData)->mName;
  // note the -1 part to reverse values, as item is at rightside
  char result = ((ic_string *)item->mData)->compare(str) * (-1);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Returns class' name.
 */
void class_name(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  char *name     = ((rc_class *)obj->mData)->mName;
  head->rSRC.push(head->new_string(name, obj->mTainted));
}

/**
 * Returns an array of class' method names.
 */
void class_methods(rc_head *head)
{
  ic_object *obj      = head->pCurrObj->get();
  ic_array *arr       = new ic_array();
  sc_voidmap *methods = &((rc_class *)obj->mData)->mMethods;
  sc_voidmapitem *item;
  methods->iter_rewind();
  while(item = methods->iter_next())
  {
    if(item->mKey->char_at(0) != '#') arr->append(head->new_string(item->mKey), false);
  }

  head->rSRC.push(head->new_array(arr, obj->mTainted));
}

/**
 * Returns an array of class' method names.
 */
void class_operators(rc_head *head)
{
  ic_object *obj      = head->pCurrObj->get();
  ic_array *arr       = new ic_array();
  sc_voidmap *methods = &((rc_class *)obj->mData)->mMethods;
  sc_voidmapitem *item;
  methods->iter_rewind();
  while(item = methods->iter_next())
  {
    if(item->mKey->char_at(0) == '#') arr->append(head->new_string(item->mKey->substr_get(1)), false);
  }

  head->rSRC.push(head->new_array(arr, obj->mTainted));
}

/**
 * Returns an array of class' object's members.
 */
void class_members(rc_head *head)
{
  ic_object *obj      = head->pCurrObj->get();
  ic_array *arr       = new ic_array();
  sc_voidmap *methods = &((rc_class *)obj->mData)->mMembers;
  sc_voidmapitem *item;
  methods->iter_rewind();
  while(item = methods->iter_next())
    arr->append(head->new_string(item->mKey), false);

  head->rSRC.push(head->new_array(arr, obj->mTainted));
}

/**
 * Returns an array of class' object's members.
 */
void class_members_static(rc_head *head)
{
  ic_object *obj      = head->pCurrObj->get();
  ic_array *arr       = new ic_array();
  sc_voidmap *methods = &((rc_class *)obj->mData)->mStaticMembers;
  sc_voidmapitem *item;
  methods->iter_rewind();
  while(item = methods->iter_next())
    arr->append(head->new_string(item->mKey), false);

  head->rSRC.push(head->new_array(arr, obj->mTainted));
}

/**
 * Returns number of created objects of this class.
 */
void class_count(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  long count     = ((rc_class *)obj->mData)->mNumObjs;
  head->rSRC.push(head->new_int(count, obj->mTainted));
}

/**
 * Returns class' parent class.
 */
void class_parent(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_class *parent = ((rc_class *)obj->mData)->pParent;
  if(parent) head->rSRC.push(head->new_class(parent, obj->mTainted));
}

/**
 * Returns class' name.
 */
void class_to_s(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  char *name     = ((rc_class *)obj->mData)->mName;
  head->rSRC.push(head->new_string(name, obj->mTainted));
}

/**
 * Inspect object
 */
void class_inspect(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_string *str = new ic_string("{class:");
  str->append(((rc_class *)obj->mData)->mName);
  str->append("}");

  head->rSRC.push(head->new_string(str));
}


#endif
