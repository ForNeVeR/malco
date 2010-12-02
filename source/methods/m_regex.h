/**
 * @file m_regex.h
 * @author impworks
 * Regex method header.
 * Defines all methods for regex class.
 */

#ifndef M_REGEX_H
#define M_REGEX_H

/**
 * Regex constructor.
 */
void regex_op_create(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = (item_var ? item_var->get() : NULL);

  if(item)
  {
    char cls = head->pCore->class_type(item->pClass);
    if(cls == M_CLASS_REGEX)
    {
      ((ic_regex *)obj->mData)->set(((ic_regex *)item->mData)->to_s());
    }
    else if(cls == M_CLASS_STRING)
    {
      ((ic_regex *)obj->mData)->set(((ic_string *)item->mData)->get());
    }
    else if(cls != M_CLASS_UNDEF)
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string or regex", "new regex"), M_EXC_ARGS);
  }

  head->obj_unlink(item_var);
}

/**
 * Compare regex with bool.
 */
void regex_op_cmp_bool(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  bool left = ((ic_regex *)obj->mData)->to_b(), right = ((ic_bool *)item->mData)->mValue;
  char result = (left == right ? 0 : (left ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare regex with regex.
 */
void regex_op_cmp_regex(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  const char *left = ((ic_regex *)obj->mData)->to_s(), *right = ((ic_regex *)item->mData)->to_s();
  char result = strcmp(left, right);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare regex with string.
 */
void regex_op_cmp_string(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  const char *str = ((ic_regex *)obj->mData)->to_s();
  // note the -1 part to reverse values, as item is at rightside
  char result = ((ic_string *)item->mData)->compare(str) * (-1);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Matches the regular expression against a string.
 */
void regex_match(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *str_var = head->rSRC.pop();
  ic_object *str = str_var->get();

  if(head->pCore->class_type(str->pClass) == M_CLASS_STRING)
  {
    ic_match *match = ((ic_regex *)obj->mData)->match((ic_string *)str->mData);
    head->rSRC.push(head->new_match(match, obj->mTainted));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string", "match"), M_EXC_ARGS);

  head->obj_unlink(str_var);
}

/**
 * Studies the regex for faster execution.
 */
void regex_study(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ((ic_regex *)obj->mData)->study();
}

/**
 * Returns a bool representation of the regex.
 */
void regex_to_b(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  bool val = ((ic_regex *)obj->mData)->to_b();
  head->rSRC.push(head->new_bool(val, obj->mTainted));
}

/**
 * Returns a string representation of the regex.
 */
void regex_to_s(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  const char *str = ((ic_regex *)obj->mData)->to_s();
  head->rSRC.push(head->new_string(str, obj->mTainted));
}

/**
 * Inspect object
 */
void regex_inspect(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_string *str = new ic_string("{regex:");
  str->append(((ic_regex *)obj->mData)->to_s());
  str->append("}");

  head->rSRC.push(head->new_string(str));
}

#endif
