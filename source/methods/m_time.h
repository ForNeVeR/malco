/**
 * @file m_time.h
 * @author impworks
 * Time method header.
 * Defines all methods for time class.
 */

#ifndef M_TIME_H
#define M_TIME_H

/**
 * Time constructor.
 */
void time_op_create(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = (item_var ? item_var->get() : NULL);

  if(item)
  {
    char cls = head->pCore->class_type(item->pClass);
    if(cls == M_CLASS_INT)
    {
      ((ic_time *)obj->mData)->set(((ic_int *)item->mData)->mValue);
    }
    else if(cls == M_CLASS_STRING)
    {
      // to integer
      ((ic_time *)obj->mData)->set(((ic_string *)item->mData)->to_i());
    }
    else if(cls != M_CLASS_UNDEF)
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or string", "new time"), M_EXC_ARGS);
  }

  head->obj_unlink(item_var);
}

/**
 * Adds int to time (as seconds).
 */
void time_op_add_int(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  long currtime    = ((ic_time *)obj->mData)->mTimestamp;
  long bonustime   = ((ic_int *)item->mData)->mValue;
  ic_time *newtime = new ic_time(currtime + bonustime);
  head->rSRC.push(head->new_time(newtime));
  head->obj_unlink(item_var);
}

/**
 * Adds time to time.
 */
void time_op_add_time(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  long currtime    = ((ic_time *)obj->mData)->mTimestamp;
  long bonustime   = ((ic_time *)item->mData)->mTimestamp;
  ic_time *newtime = new ic_time(currtime + bonustime);
  head->rSRC.push(head->new_time(newtime));
  head->obj_unlink(item_var);
}

/**
 * Subtracts int from time (as seconds).
 */
void time_op_sub_int(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  long currtime    = ((ic_time *)obj->mData)->mTimestamp;
  long bonustime   = ((ic_int *)item->mData)->mValue;
  ic_time *newtime = new ic_time(currtime - bonustime);
  head->rSRC.push(head->new_time(newtime));
  head->obj_unlink(item_var);
}

/**
 * Subtract time from time.
 */
void time_op_sub_time(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  long currtime    = ((ic_time *)obj->mData)->mTimestamp;
  long bonustime   = ((ic_time *)item->mData)->mTimestamp;
  ic_time *newtime = new ic_time(currtime - bonustime);
  head->rSRC.push(head->new_time(newtime));
  head->obj_unlink(item_var);
}

/**
 * Multiply time by int.
 */
void time_op_mul_int(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  long currtime    = ((ic_time *)obj->mData)->mTimestamp;
  long bonustime   = ((ic_int *)item->mData)->mValue;
  ic_time *newtime = new ic_time(currtime * bonustime);
  head->rSRC.push(head->new_time(newtime));
  head->obj_unlink(item_var);
}

/**
 * Divide time by int.
 */
void time_op_div_int(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  long currtime    = ((ic_time *)obj->mData)->mTimestamp;
  long bonustime   = ((ic_int *)item->mData)->mValue;
  ic_time *newtime = new ic_time(currtime / bonustime);
  head->rSRC.push(head->new_time(newtime));
  head->obj_unlink(item_var);
}

/**
 * Add one second.
 */
void time_op_inc(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_time &time  = *(ic_time *)obj->mData;
  time++;
  head->pCurrObj->mLinks++;
  head->rSRC.push(head->pCurrObj);
}

/**
 * Subtract one second.
 */
void time_op_dec(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_time &time  = *(ic_time *)obj->mData;
  time--;
  head->pCurrObj->mLinks++;
  head->rSRC.push(head->pCurrObj);
}

/**
 * Compare time to time.
 */
void time_op_cmp_time(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  long left = ((ic_time *)obj->mData)->mTimestamp, right = ((ic_time *)item->mData)->mTimestamp;
  char result = (left == right ? 0 : (left > right ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare time to integer.
 */
void time_op_cmp_int(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  long left = ((ic_time *)obj->mData)->mTimestamp, right = ((ic_int *)item->mData)->mValue;
  char result = (left == right ? 0 : (left > right ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare time to bool.
 */
void time_op_cmp_bool(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  bool left = ((ic_time *)obj->mData)->mTimestamp != 0, right = ((ic_bool *)item->mData)->mValue;
  char result = (left == right ? 0 : (left ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare time to string.
 */
void time_op_cmp_string(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  const char *str  = ((ic_time *)obj->mData)->to_s();
  // note the -1 part to reverse values, as item is at rightside
  char result = ((ic_string *)item->mData)->compare(str) * (-1);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Sets time to current value.
 */
void time_now_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(!obj->mFrozen)
    ((ic_time *)obj->mData)->current();
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Returns year as integer.
 */
void time_year(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_time *time  = (ic_time *)obj->mData;
  head->rSRC.push(head->new_int(time->mTime.tm_year + 1900, obj->mTainted));
}

/**
 * Returns month as integer.
 */
void time_month(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_time *time  = (ic_time *)obj->mData;
  head->rSRC.push(head->new_int(time->mTime.tm_mon, obj->mTainted));
}

/**
 * Returns year day as integer.
 */
void time_day_y(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_time *time  = (ic_time *)obj->mData;
  head->rSRC.push(head->new_int(time->mTime.tm_yday, obj->mTainted));
}

/**
 * Returns month day as integer.
 */
void time_day_m(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_time *time  = (ic_time *)obj->mData;
  head->rSRC.push(head->new_int(time->mTime.tm_mday, obj->mTainted));
}

/**
 * Returns week day as integer.
 */
void time_day_w(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_time *time  = (ic_time *)obj->mData;
  head->rSRC.push(head->new_int(time->mTime.tm_wday, obj->mTainted));
}

/**
 * Returns hour as integer.
 */
void time_hour(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_time *time  = (ic_time *)obj->mData;
  head->rSRC.push(head->new_int(time->mTime.tm_hour, obj->mTainted));
}

/**
 * Returns minute as integer.
 */
void time_min(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_time *time  = (ic_time *)obj->mData;
  head->rSRC.push(head->new_int(time->mTime.tm_min, obj->mTainted));
}

/**
 * Returns second as integer.
 */
void time_sec(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_time *time  = (ic_time *)obj->mData;
  head->rSRC.push(head->new_int(time->mTime.tm_sec, obj->mTainted));
}

/**
 * Returns an array of values.
 */
void time_values(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *flag_var = head->rSRC.pop();
  ic_object *flag  = flag_var->get();
  ic_time *time    = (ic_time *)obj->mData;
  bool value       = false;

  if(flag)
  {
    if(head->pCore->class_type(flag->pClass) == M_CLASS_BOOL)
      value = ((ic_bool *)flag->mData)->mValue;
    else
    {
      head->obj_unlink(flag_var);
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "bool", "values"), M_EXC_ARGS);
      return;
    }
  }

  ic_array *arr = new ic_array();

  if(value)
  {
    arr->set("year", head->new_int(time->mTime.tm_year + 1900, obj->mTainted));
    arr->set("month", head->new_int(time->mTime.tm_mon, obj->mTainted));
    arr->set("day_y", head->new_int(time->mTime.tm_yday, obj->mTainted));
    arr->set("day_m", head->new_int(time->mTime.tm_mday, obj->mTainted));
    arr->set("day_w", head->new_int(time->mTime.tm_wday, obj->mTainted));
    arr->set("hour", head->new_int(time->mTime.tm_hour, obj->mTainted));
    arr->set("min", head->new_int(time->mTime.tm_min, obj->mTainted));
    arr->set("sec", head->new_int(time->mTime.tm_sec, obj->mTainted));
  }
  else
  {
    arr->append(head->new_int(time->mTime.tm_year + 1900, obj->mTainted), false);
    arr->append(head->new_int(time->mTime.tm_mon, obj->mTainted), false);
    arr->append(head->new_int(time->mTime.tm_mday, obj->mTainted), false);
    arr->append(head->new_int(time->mTime.tm_hour, obj->mTainted), false);
    arr->append(head->new_int(time->mTime.tm_min, obj->mTainted), false);
    arr->append(head->new_int(time->mTime.tm_sec, obj->mTainted), false);
  }

  head->rSRC.push(head->new_array(arr, obj->mTainted));
}

/**
 * Outputs formatted time according to format string.
 */
void time_format(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  rc_var *str_var = head->rSRC.pop();
  ic_object *str  = str_var->get();

  if(head->pCore->class_type(str->pClass) == M_CLASS_STRING)
  {
    const char *formatted = ((ic_time *)obj->mData)->format(((ic_string *)str->mData)->get());
    head->rSRC.push(head->new_string(formatted, obj->mTainted));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string", "format"), M_EXC_ARGS);

  head->obj_unlink(str_var);
}

/**
 * Returns a boolean representation of the time.
 */
void time_to_b(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  bool val       = ((ic_time *)obj->mData)->to_b();
  head->rSRC.push(head->new_bool(val, obj->mTainted));
}

/**
 * Returns a float representation of the time.
 */
void time_to_f(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  double val     = ((ic_time *)obj->mData)->to_f();
  head->rSRC.push(head->new_float(val, obj->mTainted));
}

/**
 * Returns the timestamp of the time.
 */
void time_to_i(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  long val       = ((ic_time *)obj->mData)->to_i();
  head->rSRC.push(head->new_int(val, obj->mTainted));
}

/**
 * Returns a string representation of the time.
 */
void time_to_s(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  const char *str = ((ic_time *)obj->mData)->to_s();
  head->rSRC.push(head->new_string(str, obj->mTainted));
}

/**
 * Inspect object
 */
void time_inspect(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_string *str = new ic_string("{time:");
  str->append(((ic_time *)obj->mData)->to_s());
  str->append("}");

  head->rSRC.push(head->new_string(str, obj->mTainted));
}

#endif
