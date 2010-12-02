/**
 * @file m_method.h
 * @author impworks
 * Method method header.
 * Defines all methods for method class.
 */

#ifndef M_METHOD_H
#define M_METHOD_H

void method_op_cmp_method(rc_head *head)
{
  // methods cannot be bigger or less, they're either 'equal' or 'undefined'
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  
  rc_method *left = (rc_method *)obj->mData, *right = (rc_method *)item->mData;
  if(    left->mNative == right->mNative
      && left->mProperties == right->mProperties
      && left->mExecPoint == right->mExecPoint
      && left->pNativeFunc == right->pNativeFunc
    )
    head->rSRC.push(head->new_int(0L));
  else
    head->rSRC.push(head->new_undef());
  
  head->obj_unlink(item_var);
}

void method_op_cmp_bool(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  // a method cannot be 'false'
  bool left = true, right = ((ic_bool *)item->mData)->mValue;
  char result = (left == right ? 0 : 1);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

void method_op_cmp_string(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  const char *str = ((rc_method *)obj->mData)->mName;
  // note the -1 part to reverse values, as item is at rightside
  char result = ((ic_string *)item->mData)->compare(str) * (-1);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Calls method.
 */
void method_call(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  head->method_invoke((rc_method *)obj->mData);
}

/**
 * Returns method's class.
 */
void method_class(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  head->rSRC.push(head->new_class(((rc_method *)obj->mData)->pClass, obj->mTainted));
}

/**
 * Returns an array of method's argument names.
 */
void method_args(rc_head *head)
{
  rc_method *method = (rc_method *)head->pCurrObj->get()->mData;
  ic_array *arr = new ic_array();
  for(long idx = 0; idx < method->mParams.length(); idx++)
    arr->append(head->new_string((ic_string *)method->mParams.get(idx)), false);

  head->rSRC.push(head->new_array(arr));
}

/**
 * Returns method's minimum number of arguments.
 */
void method_args_min(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  head->rSRC.push(head->new_int(((rc_method *)obj->mData)->mMinArgs, obj->mTainted));
}

/**
 * Returns method's maximum number of arguments.
 */
void method_args_max(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  head->rSRC.push(head->new_int(((rc_method *)obj->mData)->mMaxArgs, obj->mTainted));
}

/**
 * Converts method to string.
 */
void method_to_s(rc_head *head)
{
  head->rSRC.push(head->new_string("method"));
}

/**
 * Inspect object
 */
void method_inspect(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();

  ic_string *str = new ic_string("{method:");
  str->append(((rc_method *)obj->mData)->mName);
  str->append(" @ class:");
  str->append(((rc_method *)obj->mData)->pClass->mName);
  str->append("}");

  head->rSRC.push(head->new_string(str));
}

#endif
