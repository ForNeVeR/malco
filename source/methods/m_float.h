/**
 * @file m_float.h
 * @author impworks
 * Float method header.
 * Defines all methods for float class.
 */

#ifndef M_FLOAT_H
#define M_FLOAT_H

/**
 * Float constructor.
 */
void float_op_create(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = (item_var ? item_var->get() : NULL);

  if(item)
  {
    char cls = head->pCore->class_type(item->pClass);
    if(cls == M_CLASS_BOOL)
    {
      // true = 1, false = 0
      ((ic_float *)obj->mData)->mValue = ((ic_bool *)item->mData)->mValue ? 1.0 : 0.0;
    }
    else if(cls == M_CLASS_INT)
    {
      ((ic_float *)obj->mData)->mValue = (double)((ic_int *)item->mData)->mValue;
    }
    else if(cls == M_CLASS_FLOAT)
    {
      ((ic_float *)obj->mData)->mValue = ((ic_float *)item->mData)->mValue;
    }
    else if(cls == M_CLASS_STRING)
    {
      // to float
      ((ic_float *)obj->mData)->mValue = ((ic_string *)item->mData)->to_f();
    }
    else if(cls != M_CLASS_UNDEF)
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "bool, int, float or string", "new float"), M_EXC_ARGS);
  }

  head->obj_unlink(item_var);
}

/**
 * Add int to float.
 */
void float_op_add_int(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right = right_var->get();
  double result = ((ic_float *)obj->mData)->mValue + (((ic_int *)right->mData)->mValue);
  head->rSRC.push(head->new_float(result));
  head->obj_unlink(right_var);
}

/**
 * Add float to float.
 */
void float_op_add_float(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right = right_var->get();
  double result = ((ic_float *)obj->mData)->mValue + (((ic_float *)right->mData)->mValue);
  head->rSRC.push(head->new_float(result));
  head->obj_unlink(right_var);
}

/**
 * Subtract int from float.
 */
void float_op_sub_int(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right = right_var->get();
  double result = ((ic_float *)obj->mData)->mValue - (((ic_int *)right->mData)->mValue);
  head->rSRC.push(head->new_float(result));
  head->obj_unlink(right_var);
}

/**
 * Subtract float from float.
 */
void float_op_sub_float(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right = right_var->get();
  double result = ((ic_float *)obj->mData)->mValue - (((ic_float *)right->mData)->mValue);
  head->rSRC.push(head->new_float(result));
  head->obj_unlink(right_var);
}

/**
 * Multiply int by float.
 */
void float_op_mul_int(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right = right_var->get();
  double result = ((ic_float *)obj->mData)->mValue * (((ic_int *)right->mData)->mValue);
  head->rSRC.push(head->new_float(result));
  head->obj_unlink(right_var);
}

/**
 * Multiply float by float.
 */
void float_op_mul_float(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right = right_var->get();
  double result = ((ic_float *)obj->mData)->mValue * (((ic_float *)right->mData)->mValue);
  head->rSRC.push(head->new_float(result));
  head->obj_unlink(right_var);
}

/**
 * Divide float by int.
 */
void float_op_div_int(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right = right_var->get();
  if(((ic_int *)right->mData)->mValue != 0)
  {
    double result = ((ic_float *)obj->mData)->mValue / (((ic_int *)right->mData)->mValue);
    head->rSRC.push(head->new_int(result));
  }
  else
  {
    head->rSRC.push(head->new_undef());
    head->warning(M_ERR_DIV_BY_ZERO);
  }
  head->obj_unlink(right_var);
}

/**
 * Divide float by float.
 */
void float_op_div_float(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right = right_var->get();
  if(((ic_int *)right->mData)->mValue != 0)
  {
    double result = ((ic_float *)obj->mData)->mValue / (((ic_float *)right->mData)->mValue);
    head->rSRC.push(head->new_int(result));
  }
  else
  {
    head->rSRC.push(head->new_undef());
    head->warning(M_ERR_DIV_BY_ZERO);
  }
  head->obj_unlink(right_var);
}

/**
 * Raise float to integer power.
 */
void float_op_pow_int(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right = right_var->get();
  double result = pow((double)((ic_float *)obj->mData)->mValue, (double)(((ic_int *)right->mData)->mValue));
  head->rSRC.push(head->new_float(result));
  head->obj_unlink(right_var);
}

/**
 * Raise float to float power.
 */
void float_op_pow_float(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right = right_var->get();
  double result = pow((double)((ic_float *)obj->mData)->mValue, (double)(((ic_float *)right->mData)->mValue));
  head->rSRC.push(head->new_float(result));
  head->obj_unlink(right_var);
}

/**
 * Add one.
 */
void float_op_inc(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(!obj->mFrozen)
    ((ic_float *)obj->mData)->mValue++;
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Subtract one.
 */
void float_op_dec(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(!obj->mFrozen)
    ((ic_float *)obj->mData)->mValue--;
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Compare float to int.
 */
void float_op_cmp_int(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  double left = ((ic_float *)obj->mData)->mValue, right = (double)((ic_int *)item->mData)->mValue;
  char result = (left == right ? 0 : (left > right ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare float to float.
 */
void float_op_cmp_float(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  double left = ((ic_float *)obj->mData)->mValue, right = ((ic_float *)item->mData)->mValue;
  char result = (left == right ? 0 : (left > right ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare float to string.
 */
void float_op_cmp_string(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  const char* left = ((ic_float *)obj->mData)->to_s();
  // note the -1 part to reverse values, as item is at rightside
  char result = ((ic_string *)item->mData)->compare(left) * (-1);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare float to bool.
 */
void float_op_cmp_bool(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  bool left = ((ic_float *)obj->mData)->mValue != 0.0, right = ((ic_bool *)item->mData)->mValue;
  char result = (left == right ? 0 : (left ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Check if float is in the range.
 */
void float_op_rel_range(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  bool result = ((ic_range *)item->mData)->contains(((ic_float *)obj->mData)->mValue);
  head->rSRC.push(head->new_bool(result));
  head->obj_unlink(item_var);
}


/**
 * Rounds float up.
 */
void float_ceil(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  double val = ((ic_float *)obj->mData)->mValue;
  head->rSRC.push(head->new_int(ceil(val), obj->mTainted));
}

/**
 * Rounds float down.
 */
void float_floor(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  double val = ((ic_float *)obj->mData)->mValue;
  head->rSRC.push(head->new_int(floor(val), obj->mTainted));
}

/**
 * Retuns the value after dot.
 */
void float_decimal(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  double val = ((ic_float *)obj->mData)->mValue;
  head->rSRC.push(head->new_int(val - floor(val), obj->mTainted));
}

/**
 * Returns a boolean representation of a float.
 */
void float_to_b(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  bool val = ((ic_float *)obj->mData)->to_b();
  head->rSRC.push(head->new_bool(val, obj->mTainted));
}

/**
 * Returns the float object itself.
 */
void float_to_f(rc_head *head)
{
  head->pCurrObj->mLinks++;
  head->rSRC.push(head->pCurrObj);
}

/**
 * Returns an integer representation of a float.
 */
void float_to_i(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  long val = ((ic_float *)obj->mData)->to_i();
  head->rSRC.push(head->new_int(val, obj->mTainted));
}

/**
 * Returns a string representation of a float.
 */
void float_to_s(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  const char *str = ((ic_float *)obj->mData)->to_s();
  head->rSRC.push(head->new_string(str, obj->mTainted));
}

/**
 * Inspect object
 */
void float_inspect(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();

  ic_string *str = new ic_string("{float:");
  str->append(((ic_float *)obj->mData)->to_s());
  str->append("}");

  head->rSRC.push(head->new_string(str));
}

#endif
