/**
 * @file m_int.h
 * @author impworks
 * Int method header.
 * Defines all methods for int class.
 */

#ifndef M_INT_H
#define M_INT_H

/**
 * Default int constructor.
 */
void int_op_create(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = (item_var ? item_var->get() : NULL);

  if(item)
  {
    char cls = head->pCore->class_type(item->pClass);
    if(cls == M_CLASS_BOOL)
    {
      // true = 1, false = 0
      ((ic_int *)obj->mData)->mValue = ((ic_bool *)item->mData)->mValue ? 1 : 0;
    }
    else if(cls == M_CLASS_INT)
    {
      ((ic_int *)obj->mData)->mValue = ((ic_int *)item->mData)->mValue;
    }
    else if(cls == M_CLASS_FLOAT)
    {
      ((ic_int *)obj->mData)->mValue = (long)((ic_float *)item->mData)->mValue;
    }
    else if(cls == M_CLASS_STRING)
    {
      // to integer
      ((ic_int *)obj->mData)->mValue = ((ic_string *)item->mData)->to_i();
    }
    else if(cls != M_CLASS_UNDEF)
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "bool, int, float or string", "new int"), M_EXC_ARGS);
  }

  head->obj_unlink(item_var);
}

/**
 * Adds an integer.
 */
void int_op_add_int(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  long result       = ((ic_int *)obj->mData)->mValue + (((ic_int *)right->mData)->mValue);
  head->rSRC.push(head->new_int(result));
  // head->obj_unlink(right_var);
}

/**
 * Adds a float.
 */
void int_op_add_float(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  double result     = ((ic_int *)obj->mData)->mValue + (((ic_float *)right->mData)->mValue);
  head->rSRC.push(head->new_float(result));
  head->obj_unlink(right_var);
}

/**
 * Subtracts an integer.
 */
void int_op_sub_int(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  long result       = ((ic_int *)obj->mData)->mValue - (((ic_int *)right->mData)->mValue);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(right_var);
}

/**
 * Subtracts an integer.
 */
void int_op_sub_float(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  double result     = ((ic_int *)obj->mData)->mValue - (((ic_float *)right->mData)->mValue);
  head->rSRC.push(head->new_float(result));
  head->obj_unlink(right_var);
}

/**
 * Multiplies by integer.
 */
void int_op_mul_int(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  long result       = ((ic_int *)obj->mData)->mValue * (((ic_int *)right->mData)->mValue);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(right_var);
}

/**
 * Multiplies by float.
 */
void int_op_mul_float(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  double result     = ((ic_int *)obj->mData)->mValue * (((ic_float *)right->mData)->mValue);
  head->rSRC.push(head->new_float(result));
  head->obj_unlink(right_var);
}

/**
 * Divides by integer.
 */
void int_op_div_int(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  if(((ic_int *)right->mData)->mValue != 0)
  {
    long result = ((ic_int *)obj->mData)->mValue / (((ic_int *)right->mData)->mValue);
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
 * Divides by float.
 */
void int_op_div_float(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  if(((ic_float *)right->mData)->mValue != 0)
  {
    double result = ((ic_int *)obj->mData)->mValue / (((ic_float *)right->mData)->mValue);
    head->rSRC.push(head->new_float(result));
  }
  else
  {
    head->rSRC.push(head->new_undef());
    head->warning(M_ERR_DIV_BY_ZERO);
  }
  head->obj_unlink(right_var);
}

/**
 * Raises to integer power.
 */
void int_op_pow_int(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  long result       = (long)pow((double)((ic_int *)obj->mData)->mValue, (double)(((ic_int *)right->mData)->mValue));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(right_var);
}

/**
 * Raises to float power.
 */
void int_op_pow_float(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  double result     = pow((double)((ic_int *)obj->mData)->mValue, (double)(((ic_float *)right->mData)->mValue));
  head->rSRC.push(head->new_float(result));
  head->obj_unlink(right_var);
}

/**
 * Takes remainder of integer division.
 */
void int_op_mod_int(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  long result       = ((ic_int *)obj->mData)->mValue % (((ic_int *)right->mData)->mValue);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(right_var);
}

/**
 * Shifts left.
 */
void int_op_shl_int(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  long result       = ((ic_int *)obj->mData)->mValue << (((ic_int *)right->mData)->mValue);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(right_var);
}

/**
 * Shifts right.
 */
void int_op_shr_int(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  long result       = ((ic_int *)obj->mData)->mValue >> (((ic_int *)right->mData)->mValue);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(right_var);
}

/**
 * Performs logical AND.
 */
void int_op_band_int(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  long result       = ((ic_int *)obj->mData)->mValue & (((ic_int *)right->mData)->mValue);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(right_var);
}

/**
 * Performs logical OR.
 */
void int_op_bor_int(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  long result       = ((ic_int *)obj->mData)->mValue | (((ic_int *)right->mData)->mValue);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(right_var);
}

/**
 * Performs logical XOR.
 */
void int_op_bxor_int(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *right_var = head->rSRC.pop();
  ic_object *right  = right_var->get();
  long result       = ((ic_int *)obj->mData)->mValue ^ (((ic_int *)right->mData)->mValue);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(right_var);
}

/**
 * Increments the integer.
 */
void int_op_inc(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(!obj->mFrozen)
    ((ic_int *)obj->mData)->mValue++;
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Decrements the integer.
 */
void int_op_dec(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(!obj->mFrozen)
    ((ic_int *)obj->mData)->mValue--;
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Compare int to int.
 */
void int_op_cmp_int(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  long left = ((ic_int *)obj->mData)->mValue, right = ((ic_int *)item->mData)->mValue;
  char result = (left == right ? 0 : (left > right ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare int to float.
 */
void int_op_cmp_float(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  double left = ((ic_int *)obj->mData)->mValue, right = ((ic_float *)item->mData)->mValue;
  char result = (left == right ? 0 : (left > right ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare int to string.
 */
void int_op_cmp_string(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  const char *str  = ((ic_int *)obj->mData)->to_s();
  // note the -1 part to reverse values, as item is at rightside
  char result = ((ic_string *)item->mData)->compare(str) * (-1);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare int to bool.
 */
void int_op_cmp_bool(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  bool left = ((ic_int *)obj->mData)->mValue != 0, right = ((ic_bool *)item->mData)->mValue;
  char result = (left == right ? 0 : (left ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Check if int is inside the range.
 */
void int_op_rel_range(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  bool result      = ((ic_range *)item->mData)->contains(((ic_int *)obj->mData)->mValue);
  head->rSRC.push(head->new_bool(result));
  head->obj_unlink(item_var);
}

/**
 * Calls the function N times.
 */
void int_times(rc_head *head)
{
  rc_var *fxvar   = head->rSRC.pop();
  ic_object *fx   = fxvar->get();
  ic_object *curr = head->pCurrObj->get();
  if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
  {
    long limit = abs(((ic_int *)curr->mData)->mValue);
    for(long idx = 0; idx < limit; idx++)
    {
      head->rSRC.push(head->new_int(idx));
      head->method_invoke((rc_method *)fx->mData);
      head->cmd_clrsrc();
    }
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "method", "times"), M_EXC_ARGS);

  head->obj_unlink(fxvar);
}

/**
 * Calls the function while current is lower then given number.
 */
void int_upto(rc_head *head)
{
  ic_object *curr = head->pCurrObj->get();
  rc_var *tovar = head->rSRC.pop(), *fxvar = head->rSRC.pop();
  ic_object *to = tovar->get(), *fx = fxvar->get();
  if(head->pCore->class_type(to->pClass) == M_CLASS_INT)
  {
    if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
    {
      long limit = ((ic_int *)to->mData)->mValue;
      for(long idx = ((ic_int *)curr->mData)->mValue; idx < limit; idx++)
      {
        head->rSRC.push(head->new_int(idx));
        head->method_invoke((rc_method *)fx->mData);
        head->cmd_clrsrc();
      }
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "method", "upto"), M_EXC_ARGS);
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int", "upto"), M_EXC_ARGS);

  head->obj_unlink(tovar);
  head->obj_unlink(fxvar);
}

/**
 * Calls the function while current is greater then given number.
 */
void int_downto(rc_head *head)
{
  ic_object *curr = head->pCurrObj->get();
  rc_var *tovar = head->rSRC.pop(), *fxvar = head->rSRC.pop();
  ic_object *to = tovar->get(), *fx = fxvar->get();
  if(head->pCore->class_type(to->pClass) == M_CLASS_INT)
  {
    if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
    {
      long limit = ((ic_int *)to->mData)->mValue;
      for(long idx = ((ic_int *)curr->mData)->mValue; idx > limit; idx--)
      {
        head->rSRC.push(head->new_int(idx));
        head->method_invoke((rc_method *)fx->mData);
        head->cmd_clrsrc();
      }
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "method", "downto"), M_EXC_ARGS);
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int", "downto"), M_EXC_ARGS);

  head->obj_unlink(tovar);
  head->obj_unlink(fxvar);
}

/**
 * Calls the function while current is different from given number.
 */
void int_to(rc_head *head)
{
  ic_object *curr = head->pCurrObj->get();
  rc_var *tovar = head->rSRC.pop(), *fxvar = head->rSRC.pop();
  ic_object *to = tovar->get(), *fx = fxvar->get();
  if(head->pCore->class_type(to->pClass) == M_CLASS_INT)
  {
    if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
    {
      long limit = ((ic_int *)to->mData)->mValue;
      for(long idx = ((ic_int *)curr->mData)->mValue; idx != limit; (idx < limit ? idx++ : idx--))
      {
        head->rSRC.push(head->new_int(idx));
        head->method_invoke((rc_method *)fx->mData);
        head->cmd_clrsrc();
      }
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "method", "to"), M_EXC_ARGS);
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int", "to"), M_EXC_ARGS);

  head->obj_unlink(tovar);
  head->obj_unlink(fxvar);
}

/**
 * Returns a 1-char string by the value of the integer.
 */
void int_char(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  long val       = ((ic_int *)obj->mData)->mValue;
  if(val >= 0 && val <= 255)
  {
    char str[2] = " ";
    str[0]      = (char)val;
    str[1]      = '\0';
    head->rSRC.push(head->new_string(str, obj->mTainted));
  }
}

/**
 * Returns a boolean representation of an int.
 */
void int_to_b(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  bool val       = ((ic_int *)obj->mData)->to_b();
  head->rSRC.push(head->new_bool(val, obj->mTainted));
}

/**
 * Returns a float representation of an int.
 */
void int_to_f(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  double val     = ((ic_int *)obj->mData)->to_f();
  head->rSRC.push(head->new_float(val, obj->mTainted));
}

/**
 * Returns the integer object itself.
 */
void int_to_i(rc_head *head)
{
  head->pCurrObj->mLinks++;
  head->rSRC.push(head->pCurrObj);
}

/**
 * Returns a string representation of an int.
 */
void int_to_s(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  const char *str = ((ic_int *)obj->mData)->to_s();
  head->rSRC.push(head->new_string(str, obj->mTainted));
}

/**
 * Inspect object
 */
void int_inspect(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();

  ic_string *str = new ic_string("{int:");
  str->append(((ic_int *)obj->mData)->to_s());
  str->append("}");

  head->rSRC.push(head->new_string(str));
}

#endif
