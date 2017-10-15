/**
 * @file m_string.h
 * @author impworks
 * String method header.
 * Defines all methods for string class.
 */

#ifndef M_STRING_H
#define M_STRING_H

/**
 * String constructor.
 */
void string_op_create(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = (item_var ? item_var->get() : NULL);

  if(item)
  {
    char cls = head->pCore->class_type(item->pClass);
    if(cls == M_CLASS_BOOL)
    {
      // true = "true", false = "false"
      ((ic_string *)obj->mData)->set(((ic_bool *)item->mData)->mValue ? "true" : "false");
    }
    else if(cls == M_CLASS_INT)
    {
      ((ic_string *)obj->mData)->set(((ic_int *)item->mData)->to_s());
    }
    else if(cls == M_CLASS_FLOAT)
    {
      ((ic_string *)obj->mData)->set(((ic_float *)item->mData)->to_s());
    }
    else if(cls == M_CLASS_STRING)
    {
      ((ic_string *)obj->mData)->set((ic_string *)item->mData);
    }
    else if(cls != M_CLASS_UNDEF)
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "bool, int, float or string", "new string"),
                      M_EXC_ARGS);
  }

  head->obj_unlink(item_var);
}

/**
 * Works just as concatenation.
 */
void string_op_add_string(rc_head *head)
{
  string_op_shl_string(head);
}


/**
 * Removes a substring.
 */
void string_op_sub_string(rc_head *head)
{
  // works like: str1 - str2 = str1.replace(str2, "")
  head->rSRC.push(head->new_string(""));
  string_replace(head);
}

/**
 * Repeats the string several times.
 */
void string_op_mul_int(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();

  ic_string *newstr = new ic_string(*(ic_string *)obj->mData);
  for(int idx = ((ic_int *)item->mData)->mValue; idx > 1; idx--)
    newstr->append((ic_string *)obj->mData);

  head->rSRC.push(head->new_string(newstr));
  head->obj_unlink(item_var);
}

/**
 * Formats the string with arguments.
 * @todo implement
 */
void string_op_mod_array(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();
  ic_array *arr    = (ic_array *)item->mData;

  // pack array items into voidarray of strings
  sc_voidarray strings;
  sc_voidarray vars;
  sc_voidmapitem *curr;
  arr->iter_rewind();
  while(curr = arr->iter_next())
  {
    rc_var *tmp = head->convert_string((rc_var *)curr->mValue);
    vars.add(tmp);
    strings.add((ic_string *)tmp->get()->mData);
  }

  // format the string
  ic_string *newstr = new ic_string(*(ic_string *)(obj->mData));
  newstr->format(strings);

  // cleaning up the mess
  for(long idx = 0; idx < vars.length(); idx++)
    head->obj_unlink((rc_var *)vars.get(idx));

  head->obj_unlink(item_var);

  head->rSRC.push(head->new_string(newstr));
}

/**
 * Concats a boolean.
 */
void string_op_shl_bool(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();

  ic_string *newstr = new ic_string(*(ic_string *)obj->mData);
  newstr->append(((ic_bool *)item->mData)->mValue ? "true" : "false");
  head->rSRC.push(head->new_string(newstr));

  head->obj_unlink(item_var);
}

/**
 * Concats an integer.
 */
void string_op_shl_int(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();

  ic_string *newstr = new ic_string(*(ic_string *)obj->mData);
  newstr->append(((ic_int *)item->mData)->to_s());
  head->rSRC.push(head->new_string(newstr));

  head->obj_unlink(item_var);
}

/**
 * Concats a float.
 */
void string_op_shl_float(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();

  ic_string *newstr = new ic_string(*(ic_string *)obj->mData);
  newstr->append(((ic_float *)item->mData)->to_s());
  head->rSRC.push(head->new_string(newstr));

  head->obj_unlink(item_var);
}

/**
 * Concats a string.
 */
void string_op_shl_string(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  rc_var *str_var = head->rSRC.pop();
  ic_object *str  = str_var->get();

  ic_string *newstr = new ic_string(*(ic_string *)obj->mData);
  newstr->append((ic_string *)str->mData);
  head->rSRC.push(head->new_string(newstr));

  head->obj_unlink(str_var);
}

/**
 * Concats a range.
 */
void string_op_shl_range(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();

  ic_string *newstr = new ic_string(*(ic_string *)obj->mData);
  newstr->append(((ic_range *)item->mData)->to_s());
  head->rSRC.push(head->new_string(newstr));

  head->obj_unlink(item_var);
}

/**
 * Concats any other object.
 */
void string_op_shl_object(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  rc_var *str_var = head->rSRC.pop();

  head->method_invoke("to_s", str_var);
  head->obj_unlink(str_var);
  str_var        = head->rSRC.pop();
  ic_object *str = str_var ? str_var->get() : NULL;
  if(!str || head->pCore->class_type(str->pClass) != M_CLASS_STRING)
  {
    head->exception(M_ERR_BAD_TO_S, M_EXC_SCRIPT);
    head->obj_unlink(str_var);
    return;
  }

  ic_string *newstr = new ic_string(*(ic_string *)obj->mData);
  newstr->append((ic_string *)str->mData);
  head->rSRC.push(head->new_string(newstr, obj->mTainted || str->mTainted));

  head->obj_unlink(str_var);
}

/**
 * Returns a substring.
 */
void string_op_idx(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();

  char cls = head->pCore->class_type(item->pClass);
  if(cls == M_CLASS_RANGE)
  {
    // return a substring by range
    ic_range *rg      = (ic_range *)item->mData;
    ic_string *newstr = ((ic_string *)obj->mData)->substr_get(rg->mStart, rg->mEnd - rg->mStart);
    head->rSRC.push(head->new_string(newstr, obj->mTainted));
  }
  else if(cls == M_CLASS_STRING)
  {
    // return the first position where substring is found
    ic_string *str = (ic_string *)item->mData;
    long pos       = ((ic_string *)obj->mData)->substr_first(str);
    head->rSRC.push(head->new_int(pos, obj->mTainted));
  }
  else if(cls == M_CLASS_REGEX)
  {
    // return the position of matching regex
    ic_regex *rx    = (ic_regex *)item->mData;
    ic_match *match = rx->match((ic_string *)obj->mData);
    if(match->count())
    {
      long bounds[2];
      match->bounds(0, bounds);
      head->rSRC.push(head->new_int(bounds[0], obj->mTainted));
    }
    else
      head->rSRC.push(head->new_int(-1, obj->mTainted));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "range, string or regex", "[]"), M_EXC_ARGS);

  head->obj_unlink(item_var);
}

/**
 * Compares with boolean.
 */
void string_op_cmp_bool(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();

  char result = ((ic_string *)obj->mData)->compare(((ic_bool *)item->mData)->mValue ? "true" : "false");
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compares with integer.
 */
void string_op_cmp_int(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();

  char result = ((ic_string *)obj->mData)->compare(((ic_int *)item->mData)->to_s());
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compares with float.
 */
void string_op_cmp_float(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();

  char result = ((ic_string *)obj->mData)->compare(((ic_float *)item->mData)->to_s());
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compares with string.
 */
void string_op_cmp_string(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item  = item_var->get();

  char result = ((ic_string *)obj->mData)->compare((ic_string *)item->mData);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Matches the string against regexp.
 */
void string_op_rel_regex(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *rx_var = head->rSRC.pop();
  ic_object *rx  = rx_var->get();

  ic_string *str  = (ic_string *)obj->mData;
  ic_match *match = ((ic_regex *)rx->mData)->match(str);
  head->rSRC.push(head->new_match(match, obj->mTainted));

  head->obj_unlink(rx_var);
}


/**
 * Returns the length of the string.
 */
void string_length(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  long length    = ((ic_string *)obj->mData)->length();
  head->rSRC.push(head->new_int(length, obj->mTainted));
}

/**
 * Returns the trimmed string.
 */
void string_trim(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *newobj = head->obj_clone(head->pCurrObj);
  ((ic_string *)newobj->get()->mData)->trim();
  newobj->get()->taint(obj->mTainted);
  head->rSRC.push(newobj);
}

/**
 * Returns the string trimmed from left.
 */
void string_trim_left(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *newobj = head->obj_clone(head->pCurrObj);
  ((ic_string *)newobj->get()->mData)->ltrim();
  newobj->get()->taint(obj->mTainted);
  head->rSRC.push(newobj);
}

/**
 * Returns the string trimmed from right.
 */
void string_trim_right(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *newobj = head->obj_clone(head->pCurrObj);
  ((ic_string *)newobj->get()->mData)->rtrim();
  newobj->get()->taint(obj->mTainted);
  head->rSRC.push(newobj);
}

/**
 * Trims the string.
 */
void string_trim_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(!obj->mFrozen)
  {
    ((ic_string *)obj->mData)->trim();
    head->pCurrObj->mLinks++;
    head->rSRC.push(head->pCurrObj);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Returns the string trimmed from left.
 */
void string_trim_left_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(!obj->mFrozen)
  {
    ((ic_string *)obj->mData)->ltrim();
    head->pCurrObj->mLinks++;
    head->rSRC.push(head->pCurrObj);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Returns the string trimmed from right.
 */
void string_trim_right_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(!obj->mFrozen)
  {
    ((ic_string *)obj->mData)->rtrim();
    head->pCurrObj->mLinks++;
    head->rSRC.push(head->pCurrObj);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}


/**
 * Returns the integer value of string's first char.
 */
void string_ord(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  char chr       = ((ic_string *)obj->mData)->char_at(0);
  head->rSRC.push(head->new_int((long)chr, obj->mTainted));
}

/**
 * Returns a reversed string.
 */
void string_reverse(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *newobj = head->new_string((ic_string *)obj->mData, obj->mTainted);
  ((ic_string *)newobj->get()->mData)->reverse();
  newobj->get()->taint(obj->mTainted);
  head->rSRC.push(newobj);
}

/**
 * Reverses the string.
 */
void string_reverse_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(!obj->mFrozen)
  {
    ((ic_string *)obj->mData)->reverse();
    head->pCurrObj->mLinks++;
    head->rSRC.push(head->pCurrObj);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Returns a substring.
 */
void string_sub(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *start_var = head->rSRC.pop(), *len_var = head->rSRC.pop();
  ic_object *start = start_var->get(), *len = (len_var ? len_var->get() : NULL);

  if(head->pCore->class_type(start->pClass) == M_CLASS_RANGE && !len)
  {
    ic_range *iv   = (ic_range *)start->mData;
    ic_string *str = ((ic_string *)obj->mData)->substr_get(iv->mStart, iv->mEnd - iv->mStart);
    if(str)
      head->rSRC.push(head->new_string(str, obj->mTainted));
    else
    {
      head->rSRC.push(head->new_string("", obj->mTainted));
      head->notice(M_WARN_EMPTY_SUBSTR);
    }
  }
  else if(head->pCore->class_type(start->pClass) == M_CLASS_INT)
  {
    if(head->pCore->class_type(len->pClass) == M_CLASS_INT)
    {
      long s = ((ic_int *)start->mData)->mValue, l = ((ic_int *)len->mData)->mValue;
      ic_string *str = ((ic_string *)obj->mData)->substr_get(s, l);
      if(str)
        head->rSRC.push(head->new_string(str, obj->mTainted));
      else
      {
        head->rSRC.push(head->new_string("", obj->mTainted));
        head->notice(M_WARN_EMPTY_SUBSTR);
      }
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "int", "sub"), M_EXC_ARGS);
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "range or int", "sub"), M_EXC_ARGS);

  head->obj_unlink(start_var);
  if(len_var) head->obj_unlink(len_var);
}

/**
 * Finds leftmost substring offset.
 */
void string_sub_first(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  rc_var *str_var = head->rSRC.pop(), *offset_var = head->rSRC.pop();
  ic_object *str = str_var->get(), *offset = (offset_var ? offset_var->get() : NULL);
  if(str->class_id() == M_CLASS_STRING)
  {
    long pad = 0;
    if(offset)
    {
      if(offset->class_id() == M_CLASS_INT)
        pad = ((ic_int *)offset->mData)->mValue;
      else
      {
        head->obj_unlink(str_var);
        head->obj_unlink(offset_var);
        head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "int", "sub_first"), M_EXC_ARGS);
        return;
      }
    }

    long result = ((ic_string *)obj->mData)->substr_first((ic_string *)str->mData, pad);
    if(result != -1) head->rSRC.push(head->new_int(result, obj->mTainted));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string", "sub_first"), M_EXC_ARGS);

  head->obj_unlink(str_var);
  head->obj_unlink(offset_var);
}

/**
 * Finds rightmost substring offset.
 */
void string_sub_last(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  rc_var *str_var = head->rSRC.pop(), *offset_var = head->rSRC.pop();
  ic_object *str = str_var->get(), *offset = (offset_var ? offset_var->get() : NULL);
  if(str->class_id() == M_CLASS_STRING)
  {
    long pad = 0;
    if(offset)
    {
      if(offset->class_id() == M_CLASS_INT)
        pad = ((ic_int *)offset->mData)->mValue;
      else
      {
        head->obj_unlink(str_var);
        head->obj_unlink(offset_var);
        head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "int", "sub_last"), M_EXC_ARGS);
        return;
      }
    }

    long result = ((ic_string *)obj->mData)->substr_last((ic_string *)str->mData, pad);
    if(result != -1) head->rSRC.push(head->new_int(result, obj->mTainted));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string", "sub_last"), M_EXC_ARGS);

  head->obj_unlink(str_var);
  head->obj_unlink(offset_var);
}

/**
 * Counts substrings.
 */
void string_sub_count(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  rc_var *str_var = head->rSRC.pop();
  ic_object *str  = str_var->get();
  if(str->class_id() == M_CLASS_STRING)
  {
    long result = ((ic_string *)obj->mData)->substr_count((ic_string *)str->mData);
    head->rSRC.push(head->new_int(result, obj->mTainted));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string", "sub_count"), M_EXC_ARGS);

  head->obj_unlink(str_var);
}

/**
 * Returns a string with given object's string representation appended.
 */
void string_append(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  rc_var *str_var = head->rSRC.pop();
  ic_object *str  = str_var->get();

  if(head->pCore->class_type(str->pClass) != M_CLASS_STRING)
  {
    head->method_invoke("to_s", str_var);
    head->obj_unlink(str_var);
    str_var = head->rSRC.pop();
    str     = str_var ? str_var->get() : NULL;
    if(!str || head->pCore->class_type(str->pClass) != M_CLASS_STRING)
    {
      head->exception(M_ERR_BAD_TO_S, M_EXC_SCRIPT);
      if(str_var) head->obj_unlink(str_var);
      return;
    }
  }

  ic_string *newstr = new ic_string(*(ic_string *)obj->mData);
  newstr->append((ic_string *)str->mData);
  head->rSRC.push(head->new_string(newstr, obj->mTainted || str->mTainted));

  head->obj_unlink(str_var);
}

/**
 * Returns a string with given object's string representation prepended.
 */
void string_prepend(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  rc_var *str_var = head->rSRC.pop();
  ic_object *str  = str_var->get();

  if(head->pCore->class_type(str->pClass) != M_CLASS_STRING)
  {
    head->method_invoke("to_s", str_var);
    head->obj_unlink(str_var);
    str_var = head->rSRC.pop();
    str     = str_var ? str_var->get() : NULL;
    if(!str || head->pCore->class_type(str->pClass) != M_CLASS_STRING)
    {
      head->exception(M_ERR_BAD_TO_S, M_EXC_SCRIPT);
      if(str_var) head->obj_unlink(str_var);
      return;
    }
  }

  ic_string *newstr = new ic_string(*(ic_string *)obj->mData);
  newstr->prepend((ic_string *)str->mData);
  head->rSRC.push(head->new_string(newstr, obj->mTainted || str->mTainted));

  head->obj_unlink(str_var);
}

/**
 * Returns a string with substring matched by string / regex replaced.
 */
void string_replace(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *from_var = head->rSRC.pop(), *to_var = head->rSRC.pop(), *max_var = head->rSRC.pop();
  ic_object *from = from_var->get(), *to = to_var->get(), *max = (max_var ? max_var->get() : NULL);
  ic_string *newstr = new ic_string(*(ic_string *)obj->mData);
  long count        = 0;

  // validate 'max'
  if(max && head->pCore->class_type(max->pClass) != M_CLASS_UNDEF)
  {
    if(head->pCore->class_type(max->pClass) == M_CLASS_INT)
      count = ((ic_int *)max->mData)->mValue;
    else
    {
      head->obj_unlink(from_var);
      head->obj_unlink(to_var);
      head->obj_unlink(max_var);
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 3, "int", "replace"), M_EXC_ARGS);
      return;
    }
  }

  // validate 'to'
  if(head->pCore->class_type(to->pClass) != M_CLASS_STRING)
  {
    head->obj_unlink(from_var);
    head->obj_unlink(to_var);
    head->obj_unlink(max_var);
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "string", "replace"), M_EXC_ARGS);
    return;
  }

  // validate 'from'
  char cls = head->pCore->class_type(from->pClass);
  if(cls == M_CLASS_STRING)
  {
    newstr->replace((ic_string *)from->mData, (ic_string *)to->mData, count);
    head->rSRC.push(head->new_string(newstr, obj->mTainted || to->mTainted));
  }
  else if(cls == M_CLASS_REGEX)
  {
    newstr->replace((ic_regex *)from->mData, (ic_string *)to->mData, count);
    head->rSRC.push(head->new_string(newstr, obj->mTainted || to->mTainted));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string or regex", "replace"), M_EXC_ARGS);

  head->obj_unlink(from_var);
  head->obj_unlink(to_var);
  head->obj_unlink(max_var);
}

/**
 * Appends an object's string representation.
 */
void string_append_do(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  rc_var *str_var = head->rSRC.pop();
  ic_object *str  = str_var->get();

  if(!obj->mFrozen)
  {
    if(head->pCore->class_type(str->pClass) != M_CLASS_STRING)
    {
      head->method_invoke("to_s", str_var);
      head->obj_unlink(str_var);
      str_var = head->rSRC.pop();
      str     = str_var ? str_var->get() : NULL;
      if(!str || head->pCore->class_type(str->pClass) != M_CLASS_STRING)
      {
        head->exception(M_ERR_BAD_TO_S, M_EXC_SCRIPT);
        if(str_var) head->obj_unlink(str_var);
        return;
      }
    }

    ((ic_string *)obj->mData)->append((ic_string *)str->mData);
    if(str->mTainted) obj->mTainted = true;
    head->pCurrObj->mLinks++;
    head->rSRC.push(head->pCurrObj);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);

  head->obj_unlink(str_var);
}

/**
 * Prepends a string.
 */
void string_prepend_do(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  rc_var *str_var = head->rSRC.pop();
  ic_object *str  = str_var->get();

  if(!obj->mFrozen)
  {
    if(head->pCore->class_type(str->pClass) != M_CLASS_STRING)
    {
      head->method_invoke("to_s", str_var);
      head->obj_unlink(str_var);
      str_var = head->rSRC.pop();
      str     = str_var ? str_var->get() : NULL;
      if(!str || head->pCore->class_type(str->pClass) != M_CLASS_STRING)
      {
        head->exception(M_ERR_BAD_TO_S, M_EXC_SCRIPT);
        if(str_var) head->obj_unlink(str_var);
        return;
      }
    }

    ((ic_string *)obj->mData)->prepend((ic_string *)str->mData);
    if(str->mTainted) obj->mTainted = true;
    head->pCurrObj->mLinks++;
    head->rSRC.push(head->pCurrObj);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);

  head->obj_unlink(str_var);
}

/**
 * Replaces a substring matched by string / regex.
 */
void string_replace_do(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *from_var = head->rSRC.pop(), *to_var = head->rSRC.pop(), *max_var = head->rSRC.pop();
  ic_object *from = from_var->get(), *to = to_var->get(), *max = (max_var ? max_var->get() : NULL);
  long count = 0;

  if(!obj->mFrozen)
  {
    // validate 'max'
    if(max && head->pCore->class_type(max->pClass) != M_CLASS_UNDEF)
    {
      if(head->pCore->class_type(max->pClass) == M_CLASS_INT)
        count = ((ic_int *)max->mData)->mValue;
      else
      {
        head->obj_unlink(from_var);
        head->obj_unlink(to_var);
        head->obj_unlink(max_var);
        head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 3, "int", "replace!"), M_EXC_ARGS);
        return;
      }
    }

    // validate 'to'
    if(head->pCore->class_type(to->pClass) != M_CLASS_STRING)
    {
      head->obj_unlink(from_var);
      head->obj_unlink(to_var);
      head->obj_unlink(max_var);
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "string", "replace!"), M_EXC_ARGS);
      return;
    }

    // validate 'from'
    char cls = head->pCore->class_type(from->pClass);
    if(cls == M_CLASS_STRING)
    {
      ((ic_string *)obj->mData)->replace((ic_string *)from->mData, (ic_string *)to->mData, count);
      head->pCurrObj->mLinks++;
      head->rSRC.push(head->pCurrObj);
    }
    else if(cls == M_CLASS_REGEX)
    {
      ((ic_string *)obj->mData)->replace((ic_regex *)from->mData, (ic_string *)to->mData, count);
      head->pCurrObj->mLinks++;
      head->rSRC.push(head->pCurrObj);
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string or regex", "replace!"), M_EXC_ARGS);

    if(to->mTainted) obj->mTainted = true;
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);

  head->obj_unlink(from_var);
  head->obj_unlink(to_var);
  head->obj_unlink(max_var);
}

/**
 * Splits a string into an array by string / regex.
 */
void string_split(rc_head *head)
{
  ic_object *obj    = head->pCurrObj->get();
  rc_var *split_var = head->rSRC.pop(), *max_var = head->rSRC.pop();
  ic_object *split = split_var->get(), *max = (max_var ? max_var->get() : NULL);
  ic_string *str = (ic_string *)obj->mData;
  sc_voidarray *tokens;
  long count = 0;

  // convert maximum number of splits to int
  if(max && head->pCore->class_type(max->pClass) != M_CLASS_UNDEF)
  {
    if(head->pCore->class_type(max->pClass) == M_CLASS_INT)
      count = ((ic_int *)max->mData)->mValue;
    else
    {
      head->obj_unlink(split_var);
      head->obj_unlink(max_var);
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "int", "split"), M_EXC_ARGS);
      return;
    }
  }

  // split
  char cls = head->pCore->class_type(split->pClass);
  if(cls == M_CLASS_STRING)
  {
    ic_string *sp = (ic_string *)split->mData;
    tokens        = str->split(sp, count);
  }
  else if(cls == M_CLASS_REGEX)
  {
    ic_regex *sp = (ic_regex *)split->mData;
    tokens       = str->split(sp, count);
  }
  else
  {
    head->obj_unlink(split_var);
    head->obj_unlink(max_var);
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string or regex", "split"), M_EXC_ARGS);
    return;
  }

  ic_array *arr = new ic_array();
  for(long idx = 0; idx < tokens->length(); idx++)
  {
    ic_string *curr = (ic_string *)(tokens->mPtr[idx]);
    arr->append(head->new_string(curr), obj->mTainted);
  }

  delete tokens;

  head->rSRC.push(head->new_array(arr, obj->mTainted));

  head->obj_unlink(split_var);
  head->obj_unlink(max_var);
}

/**
 * Returns a string with a lambda applied to the string's part.
 */
void string_apply(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  rc_var *idx_var = head->rSRC.pop(), *fx_var = head->rSRC.pop();
  ic_object *idx = idx_var->get(), *fx = fx_var->get();

  if(head->pCore->class_type(idx->pClass) == M_CLASS_RANGE)
  {
    if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
    {
      ic_range *iv   = (ic_range *)idx->mData;
      ic_string *str = ((ic_string *)obj->mData)->substr_get(iv->mStart, iv->mEnd - iv->mStart);
      if(str)
      {
        head->rSRC.push(head->new_string(str, obj->mTainted));
        head->method_invoke((rc_method *)fx->mData);
        rc_var *applied_var = head->rSRC.pop();
        ic_object *applied  = applied_var->get();
        if(head->pCore->class_type(applied->pClass) == M_CLASS_STRING)
        {
          ic_string *newstr = new ic_string(*(ic_string *)obj->mData);
          newstr->substr_set(iv->mStart, iv->mEnd - iv->mStart, (ic_string *)applied->mData);
          head->rSRC.push(head->new_string(newstr, obj->mTainted || applied->mTainted));
        }
        else
          head->exception(ic_string::format(M_ERR_TYPE_EXPECTED, "string"), M_EXC_SCRIPT);

        head->obj_unlink(applied_var);
      }
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "function", "apply"), M_EXC_ARGS);
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "range", "apply"), M_EXC_ARGS);

  head->obj_unlink(idx_var);
  head->obj_unlink(fx_var);
}

/**
 * Applies a lambda to the string's part.
 */
void string_apply_do(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  rc_var *idx_var = head->rSRC.pop(), *fx_var = head->rSRC.pop();
  ic_object *idx = idx_var->get(), *fx = fx_var->get();

  if(!obj->mFrozen)
  {
    if(head->pCore->class_type(idx->pClass) == M_CLASS_RANGE)
    {
      if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
      {
        ic_range *iv   = (ic_range *)idx->mData;
        ic_string *str = ((ic_string *)obj->mData)->substr_get(iv->mStart, iv->mEnd - iv->mStart);
        if(str)
        {
          head->rSRC.push(head->new_string(str, obj->mTainted));
          head->method_invoke((rc_method *)fx->mData);
          rc_var *applied_var = head->rSRC.pop();
          ic_object *applied  = applied_var->get();
          if(head->pCore->class_type(applied->pClass) == M_CLASS_STRING)
          {
            ((ic_string *)obj->mData)->substr_set(iv->mStart, iv->mEnd - iv->mStart, (ic_string *)applied->mData);
            if(applied->mTainted) obj->mTainted = true;
            head->pCurrObj->mLinks++;
            head->rSRC.push(head->pCurrObj);
          }
          else
            head->exception(ic_string::format(M_ERR_TYPE_EXPECTED, "string"), M_EXC_SCRIPT);

          head->obj_unlink(applied_var);
        }
      }
      else
        head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "function", "apply"), M_EXC_ARGS);
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "range", "apply"), M_EXC_ARGS);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);

  head->obj_unlink(idx_var);
  head->obj_unlink(fx_var);
}

/**
 * Returns the result of insertion of given string at given position.
 */
void string_insert(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  rc_var *str_var = head->rSRC.pop(), *idx_var = head->rSRC.pop();
  ic_object *str = str_var->get(), *idx = idx_var->get();

  if(head->pCore->class_type(str->pClass) == M_CLASS_STRING)
  {
    if(head->pCore->class_type(idx->pClass) == M_CLASS_INT)
    {
      ic_string *newstr = new ic_string(*((ic_string *)obj->mData));
      newstr->substr_set(((ic_int *)idx->mData)->mValue, 0, (ic_string *)str->mData);
      head->rSRC.push(head->new_string(newstr, obj->mTainted || str->mTainted));
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "int", "insert"), M_EXC_ARGS);
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string", "insert"), M_EXC_ARGS);

  head->obj_unlink(str_var);
  head->obj_unlink(idx_var);
}

/**
 * Inserts the given string at given position.
 */
void string_insert_do(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  rc_var *str_var = head->rSRC.pop(), *idx_var = head->rSRC.pop();
  ic_object *str = str_var->get(), *idx = idx_var->get();

  if(obj->mFrozen)
  {
    if(head->pCore->class_type(str->pClass) == M_CLASS_STRING)
    {
      if(head->pCore->class_type(idx->pClass) == M_CLASS_INT)
      {
        ((ic_string *)obj->mData)->substr_set(((ic_int *)idx->mData)->mValue, 0, (ic_string *)str->mData);
        if(str->mTainted) obj->mTainted = true;
        head->pCurrObj->mLinks++;
        head->rSRC.push(head->pCurrObj);
      }
      else
        head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "int", "insert"), M_EXC_ARGS);
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string", "insert"), M_EXC_ARGS);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);

  head->obj_unlink(str_var);
  head->obj_unlink(idx_var);
}

/**
 * Checks if string contains a substring.
 */
void string_has(rc_head *head)
{
  ic_object *obj  = head->pCurrObj->get();
  rc_var *str_var = head->rSRC.pop();
  ic_object *str  = str_var->get();
  if(head->pCore->class_type(str->pClass) == M_CLASS_STRING)
  {
    bool has = ((ic_string *)obj->mData)->substr_first((ic_string *)str->mData) > -1;
    head->rSRC.push(head->new_bool(has, obj->mTainted));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string", "has"), M_EXC_ARGS);

  head->obj_unlink(str_var);
}

/**
 * Returns uppercase version of the string.
 */
void string_case_up(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *newobj = head->obj_clone(head->pCurrObj);
  ((ic_string *)newobj->get()->mData)->case_up();
  newobj->get()->taint(obj->mTainted);
  head->rSRC.push(newobj);
}

/**
 * Returns lowercase version of the string.
 */
void string_case_down(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *newobj = head->obj_clone(head->pCurrObj);
  ((ic_string *)newobj->get()->mData)->case_down();
  newobj->get()->taint(obj->mTainted);
  head->rSRC.push(newobj);
}

/**
 * Returns case-swapped version of the string.
 */
void string_case_swap(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *newobj = head->obj_clone(head->pCurrObj);
  ((ic_string *)newobj->get()->mData)->case_swap();
  newobj->get()->taint(obj->mTainted);
  head->rSRC.push(newobj);
}

/**
 * Converts string to uppercase.
 */
void string_case_up_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(!obj->mFrozen)
  {
    ((ic_string *)obj->mData)->case_up();
    head->pCurrObj->mLinks++;
    head->rSRC.push(head->pCurrObj);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Converts string to lowercase.
 */
void string_case_down_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(!obj->mFrozen)
  {
    ((ic_string *)obj->mData)->case_down();
    head->pCurrObj->mLinks++;
    head->rSRC.push(head->pCurrObj);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Swaps string's case.
 */
void string_case_swap_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(!obj->mFrozen)
  {
    ((ic_string *)obj->mData)->case_swap();
    head->pCurrObj->mLinks++;
    head->rSRC.push(head->pCurrObj);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Returns an array of lines-character strings.
 */
void string_chars(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  ic_string *str   = (ic_string *)obj->mData;
  ic_array *newarr = new ic_array();

  char *buf = str->get();
  char newstr[2];
  newstr[1] = '\0';
  for(long idx = 0; idx < str->length(); idx++)
  {
    newstr[0] = buf[idx];
    newarr->append(head->new_string(newstr), obj->mTainted);
  }

  head->rSRC.push(head->new_array(newarr, obj->mTainted));
}

/**
 * Returns an array of lines-character strings.
 */
void string_lines(rc_head *head)
{
  ic_string delim  = "\n";
  ic_object *obj   = head->pCurrObj->get();
  ic_string *str   = (ic_string *)obj->mData;
  ic_array *newarr = new ic_array();

  sc_voidarray *arr = str->split(delim);
  for(long idx = 0; idx < arr->length(); idx++)
  {
    ic_string *line = (ic_string *)arr->mPtr[idx];
    newarr->append(head->new_string(line), obj->mTainted);
    delete line;
  }

  delete arr;
  head->rSRC.push(head->new_array(newarr, obj->mTainted));
}

/**
 * Translates characters in the string.
 */
void string_translate(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *from_var = head->rSRC.pop(), *to_var = head->rSRC.pop();
  ic_object *from = from_var->get(), *to = to_var->get();

  if(head->pCore->class_type(from->pClass) == M_CLASS_STRING)
  {
    if(head->pCore->class_type(to->pClass) == M_CLASS_STRING)
    {
      ic_string *str1 = (ic_string *)from->mData, *str2 = (ic_string *)to->mData;
      ic_string *newstr = new ic_string(*((ic_string *)obj->mData));
      newstr->translate(str1, str2);
      head->rSRC.push(head->new_string(newstr, obj->mTainted || to->mTainted));
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "string", "translate"), M_EXC_ARGS);
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string", "translate"), M_EXC_ARGS);

  head->obj_unlink(from_var);
  head->obj_unlink(to_var);
}

/**
 * Translates characters in the string.
 */
void string_translate_do(rc_head *head)
{
  ic_object *obj   = head->pCurrObj->get();
  rc_var *from_var = head->rSRC.pop(), *to_var = head->rSRC.pop();
  ic_object *from = from_var->get(), *to = to_var->get();

  if(!obj->mFrozen)
  {
    if(head->pCore->class_type(from->pClass) == M_CLASS_STRING)
    {
      if(head->pCore->class_type(to->pClass) == M_CLASS_STRING)
      {
        ic_string *str1 = (ic_string *)from->mData, *str2 = (ic_string *)to->mData;
        ((ic_string *)obj->mData)->translate(str1, str2);
        if(to->mTainted) obj->mTainted = true;
        head->pCurrObj->mLinks++;
        head->rSRC.push(head->pCurrObj);
      }
      else
        head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "string", "translate!"), M_EXC_ARGS);
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string", "translate!"), M_EXC_ARGS);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);

  head->obj_unlink(from_var);
  head->obj_unlink(to_var);
}

/**
 * Returns a boolean representation of a string.
 */
void string_to_b(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  bool val       = ((ic_string *)obj->mData)->to_b();
  head->rSRC.push(head->new_bool(val, obj->mTainted));
}

/**
 * Returns a float representation of a string.
 */
void string_to_f(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  double val     = ((ic_string *)obj->mData)->to_f();
  head->rSRC.push(head->new_float(val, obj->mTainted));
}

/**
 * Returns an integer representation of a string.
 */
void string_to_i(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  long val       = ((ic_string *)obj->mData)->to_i();
  head->rSRC.push(head->new_int(val, obj->mTainted));
}

/**
 * Returns the string object itself.
 */
void string_to_s(rc_head *head)
{
  head->pCurrObj->mLinks++;
  head->rSRC.push(head->pCurrObj);
}

/**
 * Inspect object
 */
void string_inspect(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_string *str = new ic_string("{string:\"");
  str->append((ic_string *)obj->mData);
  str->append("\"}");

  head->rSRC.push(head->new_string(str));
}

#endif
