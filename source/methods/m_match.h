/**
 * @file m_regex.h
 * @author impworks
 * Match method header.
 * Defines all methods for match class, recieved from regex.
 */

#ifndef M_MATCH_H
#define M_MATCH_H

/**
 * Match blocking constructor.
 */
void match_op_create(rc_head *head)
{
  head->exception(M_ERR_MATCH_CONSTRUCTOR, M_EXC_SCRIPT);
}

/**
 * Compare match with match.
 */
void match_op_cmp_match(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  long left = ((ic_match *)obj->mData)->count(), right = ((ic_match *)item->mData)->count();
  char result = (left == right ? 0 : (left > right ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare match with bool.
 */
void match_op_cmp_bool(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  bool left = ((ic_match *)obj->mData)->count() != 0, right = ((ic_bool *)item->mData)->mValue;
  char result = (left == right ? 0 : (left ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare match with string.
 */
void match_op_cmp_string(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  const char *str = "match";
  // note the -1 part to reverse values, as item is at rightside
  char result = ((ic_string *)item->mData)->compare(str) * (-1);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Returns the count of substrings matched.
 */
void match_count(rc_head *head)
{
  ic_match *match = (ic_match *)head->pCurrObj->get()->mData;
  head->rSRC.push(head->new_int(match->count()));
}

/**
 * Returns the specified substring.
 */
void match_get(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *id_var = head->rSRC.pop();
  ic_object *id = id_var->get();
  if(head->pCore->class_type(id->pClass))
  {
    long val = ((ic_int *)id->mData)->mValue;
    if(val >= 0 && val <= ((ic_match *)obj->mData)->count())
    {
      const char *str = ((ic_match *)obj->mData)->get(val);
      head->rSRC.push(head->new_string(str, obj->mTainted));
    }
    else
      head->warning(M_WARN_REGEX_SUBSTR_ID);
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int", "get"), M_EXC_ARGS);

  head->obj_unlink(id_var);
}

/**
 * Returns an array containing found substring's bounds in the original string.
 */
void match_bounds(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *id_var = head->rSRC.pop();
  ic_object *id = id_var->get();
  if(head->pCore->class_type(id->pClass))
  {
    long val = ((ic_int *)id->mData)->mValue;
    if(val >= 0 && val <= 9)
    {
      long bounds[2];
      ((ic_match *)obj->mData)->bounds(val, bounds);

      ic_array *arr = new ic_array();
      arr->set("0", head->new_int(bounds[0], obj->mTainted));
      arr->set("1", head->new_int(bounds[1], obj->mTainted));

      head->rSRC.push(head->new_array(arr));
    }
    else
      head->warning(M_WARN_REGEX_SUBSTR_ID);
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int", "bounds"), M_EXC_ARGS);

  head->obj_unlink(id_var);
}

/**
 * Returns a boolean representation of a match.
 */
void match_to_b(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  bool val = ((ic_match *)obj->mData)->count() > 0;
  head->rSRC.push(head->new_bool(val, obj->mTainted));
}

/**
 * Returns an integer representation of a match.
 */
void match_to_i(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  long count = ((ic_match *)obj->mData)->count();
  head->rSRC.push(head->new_int(count, obj->mTainted));
}

/**
 * Inspect object
 */
void match_inspect(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_int count;
  count.mValue = ((ic_match *)obj->mData)->count();
  ic_string *str = new ic_string("{match:");
  str->append(count.to_s());
  if(count.mValue == 1)
    str->append(" substring}");
  else
    str->append(" substrings}");

  head->rSRC.push(head->new_string(str));
}

#endif