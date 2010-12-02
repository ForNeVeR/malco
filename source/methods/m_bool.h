/**
 * @file m_bool.h
 * @author impworks
 * Bool method header.
 * Defines all methods for bool class.
 */

#ifndef M_BOOL_H
#define M_BOOL_H

/**
 * Boolean constructor.
 */
void bool_op_create(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = (item_var ? item_var->get() : NULL);

  if(item)
  {
    char cls = head->pCore->class_type(item->pClass);
    if(cls == M_CLASS_BOOL)
    {
      // "A boolean is a boolean."
      //           - Capt. Obvious
      ((ic_bool *)obj->mData)->mValue = ((ic_bool *)item->mData)->mValue;
    }
    else if(cls == M_CLASS_INT)
    {
      // 0 = false, else true
      ((ic_bool *)obj->mData)->mValue = ((ic_int *)item->mData)->mValue != 0;
    }
    else if(cls == M_CLASS_FLOAT)
    {
      // 0.0 = false, else true
      ((ic_bool *)obj->mData)->mValue = ((ic_float *)item->mData)->mValue != 0.0;
    }
    else if(cls == M_CLASS_STRING)
    {
      // 'true' = true, anything else is false
      ((ic_bool *)obj->mData)->mValue = ((ic_string *)item->mData)->compare("true") == 0;
    }
    else if(cls != M_CLASS_UNDEF)
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "bool, int, float or string", "new bool"), M_EXC_ARGS);
  }

  head->obj_unlink(item_var);
}

/**
 * Binary 'and' operator.
 */
void bool_op_band_bool(rc_head *head)
{
  head->sub_logical_op('a');
}

/**
 * Binary 'or' operator.
 */
void bool_op_bor_bool(rc_head *head)
{
  head->sub_logical_op('o');
}

/**
 * Binary 'xor' operator.
 */
void bool_op_bxor_bool(rc_head *head)
{
  head->sub_logical_op('x');
}

/**
 * Compare boolean value to another boolean.
 */
void bool_op_cmp_bool(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  bool left = ((ic_bool *)obj->mData)->mValue, right = ((ic_bool *)item->mData)->mValue;
  char result = (left == right ? 0 : (left ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare boolean value to anything.
 */
void bool_op_cmp_object(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  if(item_var->get()->pClass == head->pCore->mClassCache.pUndef)
    head->rSRC.push(head->new_undef());
  else
  {
    bool left = ((ic_bool *)obj->mData)->mValue, right = head->sub_value(item_var);
    char result = (left == right ? 0 : (left ? -1 : 1));
    head->rSRC.push(head->new_int(result));
  }
  head->obj_unlink(item_var);
}

/**
 * Toggles bool's state.
 */
void bool_toggle_do(rc_head *head)
{
  ic_object *curr = head->pCurrObj->get();
  if(!curr->mFrozen)
  {
    ic_bool *obj = (ic_bool *)curr->mData;
    obj->mValue = !(obj->mValue);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Returns the boolean object itself.
 */
void bool_to_b(rc_head *head)
{
  head->pCurrObj->mLinks++;
  head->rSRC.push(head->pCurrObj);
}

/**
 * Returns a float representation of a bool.
 */
void bool_to_f(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  double val = ((ic_bool *)obj->mData)->to_f();
  head->rSRC.push(head->new_float(val, obj->mTainted));
}

/**
 * Returns an integer representation of a bool.
 */
void bool_to_i(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  long val = ((ic_bool *)obj->mData)->to_i();
  head->rSRC.push(head->new_int(val, obj->mTainted));
}

/**
 * Returns a string representation of a bool.
 */
void bool_to_s(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  const char *str = ((ic_bool *)obj->mData)->to_s();
  head->rSRC.push(head->new_string(str, obj->mTainted));
}

/**
 * Inspect object
 */
void bool_inspect(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();

  ic_string *str = new ic_string("{bool:");
  str->append(((ic_bool *)obj->mData)->to_s());
  str->append("}");

  head->rSRC.push(head->new_string(str));
}

#endif
