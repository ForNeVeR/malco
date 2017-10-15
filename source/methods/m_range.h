/**
 * @file m_range.h
 * @author impworks
 * Interval method header.
 * Defines all methods for range class.
 */

#ifndef M_RANGE_H
#define M_RANGE_H

/**
 * Range constructor.
 */
void range_op_create(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *first_var = head->rSRC.pop(), *second_var = head->rSRC.pop();
  ic_object *first = first_var->get(), *second = (second_var ? second_var->get() : NULL);

  if(second)
  {
    // supposed to be two integers
    if(head->pCore->class_type(first->pClass) == M_CLASS_INT)
    {
      if(head->pCore->class_type(second->pClass) == M_CLASS_INT)
      {
        ((ic_range *)obj->mData)->mStart = ((ic_int *)first->mData)->mValue;
        ((ic_range *)obj->mData)->mEnd   = ((ic_int *)second->mData)->mValue;
        ((ic_range *)obj->mData)->iter_rewind();
      }
      else
        head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "int", "new range"), M_EXC_ARGS);
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int", "new range"), M_EXC_ARGS);

    head->obj_unlink(second_var);
  }
  else
  {
    // supposed to be a string
    if(head->pCore->class_type(second->pClass) == M_CLASS_STRING)
    {
      ((ic_range *)obj->mData)->set(((ic_string *)first->mData)->get());
      ((ic_range *)obj->mData)->iter_rewind();
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string", "new range"), M_EXC_ARGS);
  }
}

/**
 * Compare range with bool.
 */
void range_op_cmp_bool(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  bool left = ((ic_range *)obj->mData)->length() != 0, right = ((ic_bool *)item->mData)->mValue;
  char result = (left == right ? 0 : (left ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare range with range.
 */
void range_op_cmp_range(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  long left = ((ic_range *)obj->mData)->length(), right = ((ic_range *)item->mData)->length();
  char result = (left == right ? 0 : (left > right ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare range with string.
 */
void range_op_cmp_string(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  const char *left = ((ic_range *)obj->mData)->to_s();
  // note the -1 part to reverse values, as item is at rightside
  char result = ((ic_string *)item->mData)->compare(left) * (-1);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Relate int to range.
 */
void range_op_rel_int(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  bool result      = ((ic_range *)obj->mData)->contains(((ic_int *)item->mData)->mValue);
  head->rSRC.push(head->new_bool(result));
  head->obj_unlink(item_var);
}

/**
 * Relate float to range.
 */
void range_op_rel_float(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  bool result      = ((ic_range *)obj->mData)->contains(((ic_float *)item->mData)->mValue);
  head->rSRC.push(head->new_bool(result));
  head->obj_unlink(item_var);
}

/**
 * Returns range's length.
 */
void range_length(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  long len       = ((ic_range *)obj->mData)->length();
  head->rSRC.push(head->new_int(len, obj->mTainted));
}

/**
 * Sets the range to new values.
 */
void range_set_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();

  rc_var *first_var = head->rSRC.pop(), *second_var = head->rSRC.pop();
  ic_object *first = first_var->get(), *second = NULL;
  if(second_var) second = second_var->get();

  if(!obj->mFrozen)
  {
    if(head->pCore->class_type(first->pClass) == M_CLASS_STRING)
      ((ic_range *)obj->mData)->set(((ic_string *)first->mData)->get());
    else if(head->pCore->class_type(first->pClass) == M_CLASS_INT && second &&
            head->pCore->class_type(second->pClass) == M_CLASS_INT)
    {
      ((ic_range *)obj->mData)->mStart = ((ic_int *)first->mData)->mValue;
      ((ic_range *)obj->mData)->mEnd   = ((ic_int *)second->mData)->mValue;
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string or int", "set!"), M_EXC_ARGS);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);

  head->obj_unlink(first_var);
  if(second_var) head->obj_unlink(second_var);
}

/**
 * Returns next value from the range.
 */
void range_next(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_range *iv   = (ic_range *)obj->mData;
  if(!iv->mFinished)
  {
    long next = iv->iter_next();
    head->rSRC.push(head->new_int(next, obj->mTainted));
  }
}

/**
 * Rewinds range to it's first object.
 */
void range_rewind(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ((ic_range *)obj->mData)->iter_rewind();
}

/**
 * Checks if the range contains the value.
 */
void range_has(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  bool value;
  char cls = head->pCore->class_type(item->pClass);

  if(cls == M_CLASS_INT)
  {
    value = ((ic_range *)obj->mData)->contains(((ic_int *)item->mData)->mValue);
    head->rSRC.push(head->new_bool(value, obj->mTainted));
  }
  else if(cls == M_CLASS_FLOAT)
  {
    value = ((ic_range *)obj->mData)->contains(((ic_float *)item->mData)->mValue);
    head->rSRC.push(head->new_bool(value, obj->mTainted));
  }
  else if(cls == M_CLASS_RANGE)
  {
    value = ((ic_range *)obj->mData)->mStart <= ((ic_range *)item->mData)->mStart &&
            ((ic_range *)obj->mData)->mEnd >= ((ic_range *)item->mData)->mEnd;
    head->rSRC.push(head->new_bool(value, obj->mTainted));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int, float or range", "has"), M_EXC_ARGS);

  head->obj_unlink(item_var);
}

/**
 * Invokes the function for each value in range.
 */
void range_each(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_range *rg   = (ic_range *)obj->mData;
  rc_var *fx_var = head->rSRC.pop();
  ic_object *fx  = fx_var->get();

  if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
  {
    rg->iter_rewind();
    while(1)
    {
      if(rg->mFinished) break;

      head->rSRC.push(head->new_int(rg->iter_next(), obj->mTainted));
      head->method_invoke((rc_method *)fx->mData);
      head->cmd_clrsrc();
    }
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "method", "each"), M_EXC_ARGS);

  head->obj_unlink(fx_var);
}

/**
 * Gets or sets range's starting value.
 */
void range_start(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_range *rg   = (ic_range *)obj->mData;

  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = (item_var ? item_var->get() : NULL);

  // push current value
  head->rSRC.push(head->new_int(rg->mStart, obj->mTainted));

  // set new value
  if(item)
  {
    if(head->pCore->class_type(item->pClass) == M_CLASS_INT)
    {
      rg->mStart = ((ic_int *)item->mData)->mValue;
      rg->iter_rewind();
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int", "start"), M_EXC_ARGS);

    head->obj_unlink(item_var);
  }
}

/**
 * Gets or sets range's ending value.
 */
void range_end(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_range *rg   = (ic_range *)obj->mData;

  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = (item_var ? item_var->get() : NULL);

  // push current value
  head->rSRC.push(head->new_int(rg->mEnd, obj->mTainted));

  // set new value
  if(item)
  {
    if(head->pCore->class_type(item->pClass) == M_CLASS_INT)
    {
      rg->mEnd = ((ic_int *)item->mData)->mValue;
      rg->iter_rewind();
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int", "end"), M_EXC_ARGS);

    head->obj_unlink(item_var);
  }
}

/**
 * Returns an array of range values.
 */
void range_to_a(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  ic_range *iv    = (ic_range *)obj->mData;
  rc_var *arr_var = head->new_array();
  ic_object *arr  = arr_var->get();
  iv->iter_rewind();
  while(!iv->mFinished)
  {
    long next = iv->iter_next();
    ((ic_array *)arr->mData)->append(head->new_int(next, obj->mTainted), false);
  }

  head->rSRC.push(arr_var);
}

/**
 * Returns a boolean representation of the range.
 */
void range_to_b(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  bool value     = ((ic_range *)obj->mData)->to_b();
  head->rSRC.push(head->new_bool(value, obj->mTainted));
}

/**
 * Returns a float representation of the range.
 */
void range_to_f(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  double len     = (double)((ic_range *)obj->mData)->length();
  head->rSRC.push(head->new_float(len, obj->mTainted));
}

/**
 * Returns an integer representation of the range.
 */
void range_to_i(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  long len       = ((ic_range *)obj->mData)->length();
  head->rSRC.push(head->new_int(len, obj->mTainted));
}

/**
 * Returns a string representation of the range.
 */
void range_to_s(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  const char *str = ((ic_range *)obj->mData)->to_s();
  head->rSRC.push(head->new_string(str, obj->mTainted));
}

/**
 * Inspect object
 */
void range_inspect(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();

  ic_string *str = new ic_string("{range:");
  str->append(((ic_range *)obj->mData)->to_s());
  str->append("}");

  head->rSRC.push(head->new_string(str, obj->mTainted));
}

#endif
