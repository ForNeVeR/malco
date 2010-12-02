/**
 * @file m_array.h
 * @author impworks
 * Array method header.
 * Defines all methods for array class.
 */

#ifndef M_ARRAY_H
#define M_ARRAY_H

/**
 * Array constructor.
 */
void array_op_create(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  while(head->rSRC.mLength)
    arr->append(head->rSRC.pop());
}

/**
 * Index array.
 * @todo multiple indexation
 */
void array_op_idx(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  if(item_var)
  {
    ic_array *arr = (ic_array *)obj->mData;
    ic_object *item = item_var->get();
    char cls = head->pCore->class_type(item->pClass);
    if(cls == M_CLASS_STRING)
    {
      // trivial case: key is string
      rc_var *result = arr->get((ic_string *)item->mData);
      if(!result)
      {
        // register new empty sub-array at location
        result = head->new_array();
        result->mLinks++;
        arr->set((ic_string *)item->mData, result);
      }

      result->mLinks++;
      head->rSRC.push(result);
    }
    else if(cls == M_CLASS_INT)
    {
      // convert int to string and index
      rc_var *result = arr->get(((ic_int *)item->mData)->to_s());
      if(!result)
      {
        // register new empty sub-array at location
        result = head->new_array();
        result->mLinks++;
        arr->set(((ic_int *)item->mData)->to_s(), result);
      }

      result->mLinks++;
      head->rSRC.push(result);
    }
    else if(cls == M_CLASS_RANGE)
    {
      // retrieve many objects, which are available
      ic_int key;
      ic_range *rg = (ic_range *)item->mData;
      rg->iter_rewind();
      while(!rg->mFinished)
      {
        key.mValue = rg->iter_next();
        rc_var *result = arr->get(key.to_s());
        if(result)
        {
          result->mLinks++;
          head->rSRC.push(result);
        }
      }
    }
    else
      head->exception(M_ERR_BAD_INDEX, M_EXC_ARGS);

    head->obj_unlink(item_var);
  }
}

/**
 * Merge two arrays.
 */
void array_op_add_array(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();

  ic_array *left = (ic_array *)obj->mData, *right = (ic_array *)item->mData;
  ic_array *newarr = new ic_array();
  rc_var *tmp;
  sc_voidmapitem *curr;

  // append left
  left->iter_rewind();
  while(curr = left->iter_next())
  {
    tmp = (rc_var *)curr->mValue;
    tmp->mLinks++;
    newarr->set(curr->mKey, tmp);
  }

  // append right
  right->iter_rewind();
  while(curr = right->iter_next())
  {
    tmp = (rc_var *)curr->mValue;
    tmp->mLinks++;
    newarr->set(curr->mKey, tmp);
  }

  head->rSRC.push(head->new_array(newarr));
  head->obj_unlink(item_var);
}

/**
 * Add item to array.
 */
void array_op_shl_object(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(!obj->mFrozen)
  {
    ic_array *arr = (ic_array *)obj->mData;
    arr->append(head->rSRC.pop());
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);

  head->rSRC.push(head->new_undef());
}

/**
 * Compare array to boolean.
 */
void array_op_cmp_bool(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  bool left = ((ic_array *)obj->mData)->length() != 0, right = ((ic_bool *)item->mData)->mValue;
  char result = (left == right ? 0 : (left ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare array to int.
 */
void array_op_cmp_int(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  long left = ((ic_array *)obj->mData)->length(), right = ((ic_int *)item->mData)->mValue;
  char result = (left == right ? 0 : (left > right ? -1 : 1));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare array to string.
 */
void array_op_cmp_string(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  const char *str = "array";
  // note the -1 part to reverse values, as item is at rightside
  char result = ((ic_string *)item->mData)->compare(str) * (-1);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

void array_op_cmp_array(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  ic_array *left = (ic_array *)obj->mData, *right = (ic_array *)item->mData;

  if(left->length() == right->length())
  {
    left->iter_rewind();
    right->iter_rewind();
    while(1)
    {
      sc_voidmapitem *curr1 = left->iter_next(), *curr2 = right->iter_next();
      char res = head->sub_compare((rc_var *)curr1->mValue, (rc_var *)curr2->mValue);
      if(res)
      {
        // items are different!
        head->rSRC.push(head->new_int(res));
        head->obj_unlink(item_var);
        return;
      }
    }

    // no differences found
    head->rSRC.push(head->new_int(0L));
  }
  else
    head->rSRC.push(head->new_int((left->length() > right->length() ? -1 : 1)));

  head->obj_unlink(item_var);
}

/**
 * Splat array into SRC.
 */
void array_op_splat(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  arr->iter_rewind();
  sc_voidmapitem *curr;

  while(curr = arr->iter_next())
  {
    rc_var *value = (rc_var *)curr->mValue;
    value->mLinks++;
    head->rSRC.push(value);
  }
}

/**
 * Mark array recursively.
 */
void array_mark_tainted_r(rc_head *head, ic_array *arr, bool flag, bool *error)
{
  if(error) return;

  // traverse array
  sc_voidmapitem *curr;
  arr->iter_rewind();
  while(curr = arr->iter_next())
  {
    ic_object *obj = ((rc_var *)curr->mValue)->get();
    if(!obj->mFrozen)
    {
      // taint object and objects in sub-arrays
      obj->mTainted = flag;
      if(head->pCore->class_type(obj->pClass) == M_CLASS_ARRAY)
        array_mark_tainted_r(head, (ic_array *)obj->mData, flag, error);
    }
    else
    {
      *error = true;
      head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
    }
  }
}

/**
 * Mark array and all it's sub-objects as tainted.
 */
void array_taint_all_do(rc_head *head)
{
  bool error = false;
  array_mark_tainted_r(head, (ic_array*)head->pCurrObj->get()->mData, true, &error);
  head->pCurrObj->mLinks++;
  head->rSRC.push(head->pCurrObj);
}

/**
 * Mark array and all it's sub-objects as untainted.
 */
void array_untaint_all_do(rc_head *head)
{
  bool error = false;
  array_mark_tainted_r(head, (ic_array*)head->pCurrObj->get()->mData, false, &error);
  head->pCurrObj->mLinks++;
  head->rSRC.push(head->pCurrObj);
}

/**
 * Assert all objects in the array.
 */
void array_assert_all(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  long idx;
  bool found;
  ic_array classes;
  ic_array *arr = (ic_array *)obj->mData;

  // dump classes
  while(head->rSRC.mLength > 0)
  {
    rc_var *curr_var = head->rSRC.pop();
    char cls = head->pCore->class_type(curr_var->get()->pClass);
    if(cls == M_CLASS_CLASS)
      classes.append(curr_var, false);
    else if(cls == M_CLASS_STRING)
    {
      rc_class *curr_class = head->pCore->class_resolve(((ic_string *)curr_var->get()->mData)->get());
      classes.append(head->new_class(curr_class));
      head->obj_unlink(curr_var);
    }
    else
    {
      // zomfg!
      head->cmd_clrsrc();
      head->obj_unlink(curr_var);
      classes.iter_rewind();
      for(idx = 0; idx < classes.length(); idx++)
        head->obj_unlink((rc_var *)classes.iter_next()->mValue);

      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, idx+1, "string or class", "assert_all"), M_EXC_ARGS);
    }
  }

  sc_voidmapitem *curr1; // array item
  sc_voidmapitem *curr2; // class item
  arr->iter_rewind();
  // iterate array objects
  while(curr1 = arr->iter_next())
  {
    classes.iter_rewind();
    found = false;
    idx = 0;
    // iterate possible classes
    while(curr2 = classes.iter_next())
    {
      idx++;
      ic_object *obj = (ic_object *)curr1->mValue;
      rc_class *objclass = ((rc_var *)curr2->mValue)->get()->pClass;

      if(!head->pCore->class_is_heir(obj->pClass, objclass))
      {
        found = true;
        break;
      }
    }

    if(found)
      break;
  }


  head->rSRC.push(head->new_bool(!found));
  classes.iter_rewind();
  for(idx = 0; idx < classes.length(); idx++)
    head->obj_unlink((rc_var *)classes.iter_next()->mValue);
}

/**
 * Assert any objects in the array.
 */
void array_assert_any(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  long idx;
  bool found;
  ic_array classes;
  ic_array *arr = (ic_array *)obj->mData;

  // dump classes
  while(head->rSRC.mLength > 0)
  {
    rc_var *curr_var = head->rSRC.pop();
    char cls = head->pCore->class_type(curr_var->get()->pClass);
    if(cls == M_CLASS_CLASS)
      classes.append(curr_var, false);
    else if(cls == M_CLASS_STRING)
    {
      rc_class *curr_class = head->pCore->class_resolve(((ic_string *)curr_var->get()->mData)->get());
      classes.append(head->new_class(curr_class));
      head->obj_unlink(curr_var);
    }
    else
    {
      // zomfg!
      head->cmd_clrsrc();
      head->obj_unlink(curr_var);
      classes.iter_rewind();
      for(idx = 0; idx < classes.length(); idx++)
        head->obj_unlink((rc_var *)classes.iter_next()->mValue);

      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, idx+1, "string or class", "assert_all"), M_EXC_ARGS);
    }
  }

  sc_voidmapitem *curr1; // array item
  sc_voidmapitem *curr2; // class item
  arr->iter_rewind();
  // iterate array objects
  while(curr1 = arr->iter_next())
  {
    classes.iter_rewind();
    found = false;
    idx = 0;
    // iterate possible classes
    while(curr2 = classes.iter_next())
    {
      idx++;
      ic_object *obj = (ic_object *)curr1->mValue;
      rc_class *objclass = ((rc_var *)curr2->mValue)->get()->pClass;

      if(head->pCore->class_is_heir(obj->pClass, objclass))
      {
        found = true;
        break;
      }
    }

    if(found)
      break;
  }


  head->rSRC.push(head->new_bool(found));
  classes.iter_rewind();
  for(idx = 0; idx < classes.length(); idx++)
    head->obj_unlink((rc_var *)classes.iter_next()->mValue);
}

/**
 * Checks if any of the objects in the array makes the lambda return true.
 */
void array_any(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  rc_var *fx_var = head->rSRC.pop();
  ic_object *fx = fx_var->get();

  if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
  {
    sc_voidmapitem *curr;

    arr->iter_rewind();
    while(curr = arr->iter_next())
    {
      // invoke function
      rc_var *currobj = (rc_var *)curr->mValue;
      currobj->mLinks++;
      head->rSRC.push(currobj);
      head->method_invoke((rc_method *)fx->mData);
      
      // validate results
      rc_var *result = head->rSRC.pop();
      bool is_value = head->sub_value(result);
      head->obj_unlink(result);
      if(is_value)
      {
        head->obj_unlink(fx_var);
        head->rSRC.push(head->new_bool(true));
        return;
      }
    }

    head->rSRC.push(head->new_bool(false));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "method", "any"), M_EXC_ARGS);

  head->obj_unlink(fx_var);
}

/**
 * Checks if all of the objects in the array makes the lambda return true.
 */
void array_all(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  rc_var *fx_var = head->rSRC.pop();
  ic_object *fx = fx_var->get();

  if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
  {
    sc_voidmapitem *curr;

    arr->iter_rewind();
    while(curr = arr->iter_next())
    {
      // invoke function
      rc_var *currobj = (rc_var *)curr->mValue;
      currobj->mLinks++;
      head->rSRC.push(currobj);
      head->method_invoke((rc_method *)fx->mData);

      // validate results
      rc_var *result = head->rSRC.pop();
      bool is_value = head->sub_value(result);
      head->obj_unlink(result);
      if(!is_value)
      {
        head->obj_unlink(fx_var);
        head->rSRC.push(head->new_bool(false));
        return;
      }
    }

    head->rSRC.push(head->new_bool(true));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "method", "all"), M_EXC_ARGS);

  head->obj_unlink(fx_var);
}

/**
 * Checks if the array has the object.
 * @todo implement recursive search
 */
void array_has(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *search_var = head->rSRC.pop();
  ic_object *search = search_var->get();
  ic_array *arr = (ic_array *)obj->mData;
  sc_voidmapitem *curr;

  // save to-find object in AX,
  // current object in rSRC[0]
  arr->iter_rewind();
  rc_var *tmp = head->rAX;
  head->rAX = search_var;
  while(curr = arr->iter_next())
  {
    rc_var *item = (rc_var *)curr->mValue;
    item->mLinks++;
    head->rSRC.push(item);
    char type = head->sub_compare();

    if(type == M_CLASS_INT)
    {
      if(((ic_int *)head->rSRC.get(0)->get()->mData)->mValue == 0)
      {
        head->obj_unlink(head->rSRC.pop());
        head->rSRC.push(head->new_bool(false, obj->mTainted));
        head->obj_unlink(head->rAX);
        head->rAX = tmp;
        return;
      }
    }

    head->obj_unlink(head->rSRC.pop());
  }

  head->obj_unlink(head->rAX);
  head->rAX = tmp;
  head->rSRC.push(head->new_bool(false, obj->mTainted));
}

/**
 * Calls a function on each item from the array.
 */
void array_each(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  rc_var *fx_var = head->rSRC.pop();
  ic_object *fx = fx_var->get();

  if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
  {
    sc_voidmapitem *curr;

    arr->iter_rewind();
    while(curr = arr->iter_next())
    {
      rc_var *currobj = (rc_var *)curr->mValue;
      currobj->mLinks++;
      head->rSRC.push(currobj);
      head->method_invoke((rc_method *)fx->mData);
      head->cmd_clrsrc();
    }
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "method", "each"), M_EXC_ARGS);

  head->obj_unlink(fx_var);
}

/**
 * Calls a function on each key from the array.
 * @todo implement links!
 */
void array_each_key(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  rc_var *fx_var = head->rSRC.pop();
  ic_object *fx = fx_var->get();

  if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
  {
    sc_voidmapitem *curr;

    arr->iter_rewind();
    while(curr = arr->iter_next())
    {
      head->rSRC.push(head->new_string(curr->mKey));
      head->method_invoke((rc_method *)fx->mData);
      head->cmd_clrsrc();
    }
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "method", "each_key"), M_EXC_ARGS);

  head->obj_unlink(fx_var);
}

/**
 * Calls a function on each pair of item and key from the array.
 * @todo implement links!
 */
void array_each_pair(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  rc_var *fx_var = head->rSRC.pop();
  ic_object *fx = fx_var->get();

  if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
  {
    sc_voidmapitem *curr;

    arr->iter_rewind();
    while(curr = arr->iter_next())
    {
      head->rSRC.push(head->new_string(curr->mKey));
      rc_var *currobj = (rc_var *)curr->mValue;
      currobj->mLinks++;
      head->rSRC.push(currobj);
      head->method_invoke((rc_method *)fx->mData);
      head->cmd_clrsrc();
    }
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "method", "each_pair"), M_EXC_ARGS);

  head->obj_unlink(fx_var);
}

/**
 * Returns the next item from the array.
 */
void array_next(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  sc_voidmapitem *pair = ((ic_array *)obj->mData)->iter_next();
  rc_var *curr = (rc_var *)pair->mValue;
  curr->mLinks++;
  head->rSRC.push(curr);
}

/**
 * Returns the next key from the array.
 */
void array_next_key(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  sc_voidmapitem *pair = ((ic_array *)obj->mData)->iter_next();
  head->rSRC.push(head->new_string(pair->mKey, obj->mTainted));
}

/**
 * Returns the next pair of item and key from the array.
 */
void array_next_pair(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  sc_voidmapitem *pair = ((ic_array *)obj->mData)->iter_next();
  ((rc_var *)pair->mValue)->mLinks++;
  ic_array *arr = new ic_array();
  arr->append(head->new_string(pair->mKey, obj->mTainted), false);
  arr->append((rc_var *)pair->mValue, false);

  head->rSRC.push(head->new_array(arr, obj->mTainted));
}

/**
 * Rewinds array's internal iterator.
 */
void array_rewind(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ((ic_array *)obj->mData)->iter_rewind();
}

/**
 * Finds a specific item in the array.
 */
void array_find(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *needle_var = head->rSRC.pop();
  ic_array *arr = (ic_array *)obj->mData;
  sc_voidmapitem *curr;

  rc_var *tmp = head->rAX;
  head->rAX = needle_var;
  arr->iter_rewind();
  while(curr = arr->iter_next())
  {
    rc_var *item = (rc_var *)curr->mValue;
    item->mLinks++;
    head->rSRC.push(item);
    char test = head->sub_compare();
    if(test == M_CLASS_INT)
    {
      long value = ((ic_int *)head->rSRC.get(0)->get()->mData)->mValue;
      if(!value)
      {
        // object found
        head->obj_unlink(head->rSRC.pop());
        head->obj_unlink(needle_var);
        head->rAX = tmp;
        head->rSRC.push(head->new_string(curr->mKey, obj->mTainted));
        return;
      }

      head->obj_unlink(head->rSRC.pop());
    }
  }

  head->rAX = tmp;
  head->obj_unlink(needle_var);
}

/**
 * Returns an array with keys and values switched.
 */
void array_flip(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  ic_array *new_arr = new ic_array();
  sc_voidmapitem *curr;

  arr->iter_rewind();
  while(curr = arr->iter_next())
  {
    rc_var *tmp = head->convert_string((rc_var *)curr->mValue);
    ic_string *key = (ic_string *)tmp->get()->mData;

    // issue a warning if key already exists
    if(new_arr->get(key))
      head->warning(M_WARN_DUPLICATE_KEY, key->get());

    new_arr->set(key, head->new_string(curr->mKey));

    head->obj_unlink(tmp);
  }

  head->rSRC.push(head->new_array(new_arr, obj->mTainted));
}

/**
 * Switches keys and values in the array.
 */
void array_flip_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  ic_array *new_arr = new ic_array();
  sc_voidmapitem *curr;

  arr->iter_rewind();
  while(curr = arr->iter_next())
  {
    rc_var *tmp = head->convert_string((rc_var *)curr->mValue);
    ic_string *key = (ic_string *)tmp->get()->mData;

    // issue a warning if key already exists
    if(new_arr->get(key))
      head->warning(M_WARN_DUPLICATE_KEY, key->get());

    new_arr->set(key, head->new_string(curr->mKey));

    head->obj_unlink(tmp);
  }

  head->var_save(head->pCurrObj, head->new_array(new_arr, obj->mTainted));
  head->pCurrObj->mLinks++;
  head->rSRC.push(head->pCurrObj);
}

/**
 * Returns the array sorted with a best-fitting algorithm.
 * @todo direction
 */
void array_sort(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(((ic_array *)obj->mData)->length() < 1000)
    array_sort_shell(head);
  else
    array_sort_quick(head);
}

/**
 * Actual recursive quicksort function.
 * @todo direction
 */
void array_sort_quick_r(rc_head *head, sc_voidmapitem **arr, long start, long end)
{
  long idx = start, idx2 = end;
  rc_var *curr = (rc_var *)arr[(start + end) / 2]->mValue;

  do
  {
    while (head->sub_compare((rc_var *)arr[idx]->mValue, curr) == -1) idx++;
    while (head->sub_compare((rc_var *)arr[idx2]->mValue, curr) == 1) idx2--;

    if(idx <= idx2)
    {
      if (idx < idx2)
      {
        sc_voidmapitem *tmp = arr[idx];
        arr[idx] = arr[idx2];
        arr[idx2] = tmp;
      }
      idx++;
      idx2--;
    }
  } while (idx <= idx2);

  if (idx < end)
    array_sort_quick_r(head, arr, idx, end);

  if (start < idx2)
    array_sort_quick_r(head, arr, start, idx2);
}

/**
 * Returns the array sorted using quick sort algorithm.
 * @todo direction
 */
void array_sort_quick(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  long idx = 0;
  sc_voidmapitem *curr;

  // create temp array of pointers
  arr->iter_rewind();
  sc_voidmapitem **tmparr = new sc_voidmapitem*[arr->length()];
  while(curr = arr->iter_next())
  {
    tmparr[idx] = curr;
    idx++;
  }

  // sort array
  array_sort_quick_r(head, tmparr, 0, idx-1);

  // create new ic_array from temp array
  ic_array *newarr = new ic_array();
  for(idx = 0; idx < arr->length(); idx++)
    newarr->set(tmparr[idx]->mKey, (rc_var *)tmparr[idx]->mValue);

  delete [] tmparr;

  head->rSRC.push(head->new_array(newarr, obj->mTainted));
}

/**
 * Returns the array sorted using shell sort algorithm.
 * @todo direction
 */
void array_sort_shell(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  long idx = 0, idx2;
  sc_voidmapitem *curr;

  // create temp array of pointers
  arr->iter_rewind();
  sc_voidmapitem **tmparr = new sc_voidmapitem*[arr->length()];
  while(curr = arr->iter_next())
  {
    tmparr[idx] = curr;
    idx++;
  }

  // sort array
  long step = arr->length() / 2;
  while (step > 0)
  {
    for (idx = step; idx < arr->length(); idx2++)
    {
      sc_voidmapitem *value = tmparr[idx];

      for (idx2 = idx - step; (idx2 >= 0) && (head->sub_compare((rc_var *)tmparr[idx2]->mValue, (rc_var *)value->mValue) > 0); idx2 -= step)
        tmparr[idx2 + step] = tmparr[idx2];

      tmparr[idx2 + step] = value;
    }
    step /= 2;
  }

  // create new ic_array from temp array
  ic_array *newarr = new ic_array();
  for(idx = 0; idx < arr->length(); idx++)
    newarr->set(tmparr[idx]->mKey, (rc_var *)tmparr[idx]->mValue);

  delete [] tmparr;

  head->rSRC.push(head->new_array(newarr, obj->mTainted));
}

/**
 * Sorts the array with a best-fitting algorithm.
 * @todo direction
 */
void array_sort_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(((ic_array *)obj->mData)->length() < 1000)
    array_sort_shell_do(head);
  else
    array_sort_quick_do(head);
}

/**
 * Sorts the array using quick sort algorithm.
 * @todo Rewrite the assign part to restrain it from cloning objects!
 * @todo direction
 */
void array_sort_quick_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  long idx = 0;
  sc_voidmapitem *curr;

  // create temp array of pointers
  arr->iter_rewind();
  sc_voidmapitem **tmparr = new sc_voidmapitem*[arr->length()];
  while(curr = arr->iter_next())
  {
    tmparr[idx] = curr;
    idx++;
  }

  // sort array
  array_sort_quick_r(head, tmparr, 0, idx-1);

  // create new ic_array from temp array
  ic_array *newarr = new ic_array();
  for(idx = 0; idx < arr->length(); idx++)
    newarr->set(tmparr[idx]->mKey, (rc_var *)tmparr[idx]->mValue);

  head->var_save(head->pCurrObj, head->new_array(newarr));

  delete [] tmparr;

  head->rSRC.push(head->pCurrObj);
}

/**
 * Sorts the array using shell sort algorithm.
 * @todo direction
 */
void array_sort_shell_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  long idx = 0, idx2;
  sc_voidmapitem *curr;

  if(!obj->mFrozen)
  {
    // create temp array of pointers
    arr->iter_rewind();
    sc_voidmapitem **tmparr = new sc_voidmapitem*[arr->length()];
    while(curr = arr->iter_next())
    {
      tmparr[idx] = curr;
      idx++;
    }

    // sort array
    long step = arr->length() / 2;
    while (step > 0)
    {
      for (idx = step; idx < arr->length(); idx2++)
      {
        sc_voidmapitem *value = tmparr[idx];

        for (idx2 = idx - step; (idx2 >= 0) && (head->sub_compare((rc_var *)tmparr[idx2]->mValue, (rc_var *)value->mValue) > 0); idx2 -= step)
          tmparr[idx2 + step] = tmparr[idx2];

        tmparr[idx2 + step] = value;
      }
      step /= 2;
    }

    // create new ic_array from temp array
    ic_array *newarr = new ic_array();
    for(idx = 0; idx < arr->length(); idx++)
      newarr->set(tmparr[idx]->mKey, (rc_var *)tmparr[idx]->mValue);

    head->var_save(head->pCurrObj, head->new_array(newarr, obj->mTainted));

    delete [] tmparr;

    head->pCurrObj->mLinks++;
    head->rSRC.push(head->pCurrObj);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Returns the array sorted according to a specific function.
 * @todo implement
 */
void array_sort_by(rc_head *head)
{

}

/**
 * Sorts the array according to a specific function.
 * @todo implement
 */
void array_sort_by_do(rc_head *head)
{

}

/**
 * The random-value function for shuffling array.
 */
void array_shuffle_rand(rc_head *head)
{
  // discard input
  head->obj_unlink(head->rSRC.pop());
  head->rSRC.push(head->new_int(sc_random::get_next()));
}

/**
 * Returns array randomly shuffled.
 * @todo rewrite using more efficient technique (maybe)
 */
void array_shuffle(rc_head *head)
{
  rc_method *shuf = new rc_method();
  shuf->setup(1, 1, false, "item");
  shuf->mProperties = M_PROP_STATIC | M_PROP_PUBLIC;
  shuf->pClass = head->pCore->mClassCache.pObject;

  head->rSRC.push(head->new_method(shuf));
  array_sort_by(head);
}

/**
 * Randomly shuffles array.
 * @todo rewrite using more efficient technique (maybe)
 */
void array_shuffle_do(rc_head *head)
{
  rc_method *shuf = new rc_method();
  shuf->setup(1, 1, false, "item");
  shuf->mProperties = M_PROP_STATIC | M_PROP_PUBLIC;
  shuf->pClass = head->pCore->mClassCache.pObject;

  head->rSRC.push(head->new_method(shuf));
  array_sort_by_do(head);
}

/**
 * Maps each array's item to the corresponding item from the other array.
 */
void array_map_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  sc_voidmapitem *curr, *curr2;
  ic_array *arr = (ic_array *)obj->mData;
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();

  if(head->pCore->class_type(item->pClass) == M_CLASS_ARRAY)
  {
    ic_array *arr2 = (ic_array *)item->mData;

    arr->iter_rewind();
    arr2->iter_rewind();
    while(curr = arr->iter_next())
    {
      curr2 = arr2->iter_next();
      if(curr2)
        head->var_save((rc_var *)curr->mValue, (rc_var *)curr2->mValue);
      else
        break;
    }
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "array", "map!"), M_EXC_ARGS);

  head->obj_unlink(item_var);
}

/**
 * Returns sum of all array values.
 */
void array_sum(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  sc_voidmapitem *curr;
  ic_array *arr = (ic_array *)obj->mData;

  rc_var *tmp = head->rAX;
  head->rSRC.push(head->new_float(0.0, false));
  arr->iter_rewind();
  while(curr = arr->iter_next())
  {
    head->rAX = (rc_var *)curr->mValue;
    head->rAX->mLinks++;
    head->cmd_add();
    head->obj_unlink(head->rAX);
  }
}

/**
 * Returns multiplication of all array values.
 */
void array_mul(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  sc_voidmapitem *curr;
  ic_array *arr = (ic_array *)obj->mData;

  rc_var *tmp = head->rAX;
  head->rSRC.push(head->new_float(0.0, false));
  arr->iter_rewind();
  while(curr = arr->iter_next())
  {
    head->rAX = (rc_var *)curr->mValue;
    head->rAX->mLinks++;
    head->cmd_mul();
    head->obj_unlink(head->rAX);
  }
}

/**
 * Joins array items as strings using a delimiter.
 */
void array_join(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  rc_var *delim_var = head->rSRC.pop();
  ic_object *delim = (delim_var ? delim_var->get() : NULL);

  // check delimiter for being a string
  if(delim)
  {
    if(head->pCore->class_type(delim->pClass) != M_CLASS_STRING)
    {
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string", "join"), M_EXC_ARGS);
      head->obj_unlink(delim_var);
      return;
    }
  }

  ic_string *str = new ic_string();
  arr->iter_rewind();
  if(arr->length())
  {
    sc_voidmapitem *curr;

    // append first element
    curr = arr->iter_next();
    rc_var *item = head->convert_string((rc_var *)curr->mValue);
    str->append((ic_string *)item->get()->mData);
    head->obj_unlink(item);

    while(curr = arr->iter_next())
    {
      // append delimiter
      if(delim)
        str->append((ic_string *)delim->mData);

      // append next string
      item = head->convert_string((rc_var *)curr->mValue);
      str->append((ic_string *)item->get()->mData);
      head->obj_unlink(item);
    }
  }

  head->rSRC.push(head->new_string(str, obj->mTainted));
}

/**
 * Pops one or more item from the end of the array.
 */
void array_pop_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *count_var = head->rSRC.pop();
  ic_object *count = (count_var ? count_var->get() : NULL);
  long idx = 1;

  if(count)
  {
    if(head->pCore->class_type(count->pClass) == M_CLASS_INT)
    {
      idx = ((ic_int *)count->mData)->mValue;
      // limit idx to 1 .. array->length
      idx = MIN(((ic_array *)obj->mData)->length(), MAX(idx, 1));
    }
    else
    {
      head->obj_unlink(count_var);
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int", "pop!"), M_EXC_ARGS);
      return;
    }
  }

  sc_voidmapitem *item;
  ic_array *arr = (ic_array *)obj->mData;
  while(idx --> 0)
  {
    item = arr->iter_last();
    head->rSRC.push((rc_var *)item->mValue);
    arr->unset(item->mKey);
  }
}

/**
 * Pushes item(s) to the end of the array.
 */
void array_push_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *curr;

  if(!obj->mFrozen)
  {
    while(curr = head->rSRC.pop())
    {
      curr->get()->mTainted = obj->mTainted;
      ((ic_array *)obj->mData)->append(curr);
    }
  }
  else
  {
    head->cmd_clrsrc();
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
  }
}

/**
 * Returns the same array with indexes changed to start from 0 and continue in numerical fashion.
 */
void array_reindex(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData, *newarr = new ic_array();
  sc_voidmapitem *curr;

  arr->iter_rewind();
  while(curr = arr->iter_next())
  {
    ((rc_var *)curr->mValue)->mLinks++;
    newarr->append((rc_var *)curr->mValue, false);
  }

  head->rSRC.push(head->new_array(newarr, obj->mTainted));
}

/**
 * Changes indexes to start from 0 and continue in numerical fashion.
 */
void array_reindex_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  if(!obj->mFrozen)
  {
    ic_int *idx = new ic_int();
    sc_voidmapitem *curr;

    arr->iter_rewind();
    while(curr = arr->iter_next())
    {
      curr->mKey->set(idx->to_s());
      idx->mValue++;
    }

    head->pCurrObj->mLinks++;
    head->rSRC.push(head->pCurrObj);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Adds items from one array to another.
 */
void array_flatten_r(rc_head *head, ic_array *from, ic_array *to)
{
  sc_voidmapitem *curr;

  from->iter_rewind();
  while(curr = from->iter_next())
  {
    if(curr->mValue)
    {
      ic_object *obj = ((rc_var *)curr->mValue)->get();
      if(head->pCore->class_type(obj->pClass) == M_CLASS_ARRAY)
      {
        ic_array *tmp = (ic_array *)obj->mData;
        array_flatten_r(head, tmp, to);
      }
      else
        to->append((rc_var *)curr->mValue, false);
    }
  }
}

/**
 * Returns flattened array if it has sub-arrays.
 */
void array_flatten(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *to = new ic_array();
  array_flatten_r(head, (ic_array *)obj->mData, to);
  head->rSRC.push(head->new_array(to, obj->mTainted));
}

/**
 * Returns array inflated depending on given size.
 */
void array_inflate(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *size_var = head->rSRC.pop();
  ic_object *size = size_var->get();
  if(head->pCore->class_type(size->pClass) == M_CLASS_INT)
  {
    // size of each piece at max
    long count = ((ic_int *)size->mData)->mValue;
    // size of current piece
    long tmpsize = 0;
    ic_array *arr = (ic_array *)obj->mData;
    // newarr = [tmparr = [item, ...], tmparr = [item, ...], ...]
    ic_array *newarr = new ic_array(), *tmparr = new ic_array();
    sc_voidmapitem *curr;

    arr->iter_rewind();
    while(curr = arr->iter_next())
    {
      rc_var *item = (rc_var *)curr->mValue;
      item->mLinks++;
      tmparr->append(item, false);
      tmpsize++;

      // tmparr is full -> drop it into newarr
      if(tmpsize == count)
      {
        newarr->append(head->new_array(tmparr, obj->mTainted), false);
        tmpsize = 0;
        tmparr = new ic_array();
      }
    }

    // last box may not be full, but anyway
    if(tmpsize > 0)
      newarr->append(head->new_array(tmparr, obj->mTainted), false);

    head->rSRC.push(head->new_array(newarr, obj->mTainted));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int", "inflate"), M_EXC_ARGS);

  head->obj_unlink(size_var);
}

/**
 * Returns unique values from the array.
 */
void array_unique(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  ic_array *new_arr = new ic_array();
  sc_voidmapitem *curr, *curr2;

  // append items
  arr->iter_rewind();
  while(curr = arr->iter_next())
  {
    new_arr->iter_rewind();
    bool found = false;
    while(curr2 = new_arr->iter_next())
    {
      // sub_compare in strict mode: -1 & 1 or 1 & 1 give 1, 0 or 2 give 0
      if(head->sub_compare((rc_var *)curr->mValue, (rc_var *)curr->mValue, true) & 1)
      {
        found = true;
        break;
      }
    }

    // append the item if it has not been found in the array
    if(!found)
      new_arr->append((rc_var *)curr->mValue);
  }

  head->rSRC.push(head->new_array(new_arr, obj->mTainted));
}

/**
 * Returns items that match the condition in the lambda.
 */
void array_select(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *fx_var = head->rSRC.pop();
  ic_object *fx = fx_var->get();

  if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
  {
    ic_array *arr = (ic_array *)obj->mData, *newarr = new ic_array();
    arr->iter_rewind();
    sc_voidmapitem *curr;

    while(curr = arr->iter_next())
    {
      head->rSRC.push((rc_var *)curr->mValue);
      head->method_invoke((rc_method *)fx->mData);
      if(head->sub_value(head->rSRC.get(0)))
        newarr->set(curr->mKey, (rc_var *)curr->mValue);
      head->cmd_clrsrc();
    }

    head->rSRC.push(head->new_array(newarr, obj->mTainted));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "method", "select"), M_EXC_ARGS);

  head->obj_unlink(fx_var);
}

/**
 * Returns items that don't match the condition in the lambda.
 */
void array_reject(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *fx_var = head->rSRC.pop();
  ic_object *fx = fx_var->get();

  if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
  {
    ic_array *arr = (ic_array *)obj->mData, *newarr = new ic_array();
    arr->iter_rewind();
    sc_voidmapitem *curr;

    while(curr = arr->iter_next())
    {
      head->rSRC.push((rc_var *)curr->mValue);
      head->method_invoke((rc_method *)fx->mData);
      if(!head->sub_value(head->rSRC.get(0)))
        newarr->set(curr->mKey, (rc_var *)curr->mValue);
      head->cmd_clrsrc();
    }

    head->rSRC.push(head->new_array(newarr, obj->mTainted));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "method", "select"), M_EXC_ARGS);

  head->obj_unlink(fx_var);
}

/**
 * Collects data from traversing the whole array.
 */
void array_collect(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *def_var = head->rSRC.pop(), *fx_var = head->rSRC.pop();
  ic_object *fx = fx_var->get();
  ic_array *arr = (ic_array *)obj->mData;
  sc_voidmapitem *curr;

  if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
  {
    arr->iter_rewind();
    // default var at first
    head->rSRC.push(def_var);
    while(curr = arr->iter_next())
    {
      head->rSRC.push((rc_var *)curr->mValue);
      head->method_invoke((rc_method *)fx->mData);

      // check function for returning a proper value
      if(head->rSRC.mLength == 0)
      {
        head->warning(M_WARN_FX_NO_RETURN);
        head->rSRC.push(head->new_undef());
      }

      // check function for returning too many values
      if(head->rSRC.mLength > 1)
      {
        head->warning(M_WARN_FX_BAD_RETURN_COUNT, head->rSRC.mLength, 1);
        rc_var *tmp = head->rSRC.pop();
        head->cmd_clrsrc();
        head->rSRC.push(tmp);
      }
    }
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "method", "collect"), M_EXC_ARGS);

  // def_var gets unlinked at first iteration.
  // the resulting return value from method rests in rSRC.
  // if no iterations have been made, the def_var is still there.
  head->obj_unlink(fx_var);
}

/**
 * Flattens array if it has sub-arrays.
 */
void array_flatten_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *to = new ic_array();
  array_flatten_r(head, (ic_array *)obj->mData, to);
  head->var_save(head->pCurrObj, head->new_array(to, obj->mTainted));
  head->pCurrObj->mLinks++;
  head->rSRC.push(head->pCurrObj);
}

/**
 * Inflates array depending on given size.
 */
void array_inflate_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *size_var = head->rSRC.pop();
  ic_object *size = size_var->get();
  if(!obj->mFrozen)
  {
    if(head->pCore->class_type(size->pClass) == M_CLASS_INT)
    {
      // size of each piece at max
      long count = ((ic_int *)size->mData)->mValue;
      // size of current piece
      long tmpsize = 0;
      ic_array *arr = (ic_array *)obj->mData;
      // newarr = [tmparr = [item, ...], tmparr = [item, ...], ...]
      ic_array *newarr = new ic_array(), *tmparr = new ic_array();
      sc_voidmapitem *curr;

      arr->iter_rewind();
      while(curr = arr->iter_next())
      {
        rc_var *item = (rc_var *)curr->mValue;
        item->mLinks++;
        tmparr->append(item, false);
        tmpsize++;

        // tmparr is full -> drop it into newarr
        if(tmpsize == count)
        {
          newarr->append(head->new_array(tmparr, obj->mTainted), false);
          tmpsize = 0;
          tmparr = new ic_array();
        }
      }

      // last box may not be full, but anyway
      if(tmpsize > 0)
        newarr->append(head->new_array(tmparr, obj->mTainted), false);

      head->var_save(head->pCurrObj, head->new_array(newarr, obj->mTainted));
      head->pCurrObj->mLinks++;
      head->rSRC.push(head->pCurrObj);
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int", "inflate!"), M_EXC_ARGS);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);

  head->obj_unlink(size_var);
}

/**
 * Leaves only unique values from the array.
 */
void array_unique_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  ic_array *new_arr = new ic_array();
  sc_voidmapitem *curr, *curr2;

  // append items
  arr->iter_rewind();
  while(curr = arr->iter_next())
  {
    new_arr->iter_rewind();
    bool found = false;
    while(curr2 = new_arr->iter_next())
    {
      // sub_compare in strict mode: -1 & 1 or 1 & 1 give 1, 0 or 2 give 0
      if(head->sub_compare((rc_var *)curr->mValue, (rc_var *)curr->mValue, true) & 1)
      {
        found = true;
        break;
      }
    }

    // append the item if it has not been found in the array
    if(!found)
      new_arr->append((rc_var *)curr->mValue);
  }

  head->var_save(head->pCurrObj, head->new_array(new_arr, obj->mTainted));
  head->pCurrObj->mLinks++;
  head->rSRC.push(head->pCurrObj);
}

/**
 * Leaves only items that match the condition in the lambda.
 */
void array_select_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *fx_var = head->rSRC.pop();
  ic_object *fx = fx_var->get();

  if(!obj->mFrozen)
  {
    if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
    {
      ic_array *arr = (ic_array *)obj->mData;
      arr->iter_rewind();
      sc_voidmapitem *curr;
      sc_voidarray data;
      data.resize(arr->length());
      while(curr = arr->iter_next())
        data.add((void *)curr);

      for(long idx = 0; idx < data.length(); idx ++)
      {
        curr = (sc_voidmapitem *)data.get(idx);
        head->rSRC.push((rc_var *)curr->mValue);
        head->method_invoke((rc_method *)fx->mData);
        if(!head->sub_value(head->rSRC.get(0)))
          arr->unset(curr->mKey);
        head->cmd_clrsrc();
      }

      head->pCurrObj->mLinks++;
      head->rSRC.push(head->pCurrObj);
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "method", "select!"), M_EXC_ARGS);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);

  head->obj_unlink(fx_var);
}

/**
 * Removes items that match the condition in the lambda.
 */
void array_reject_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *fx_var = head->rSRC.pop();
  ic_object *fx = fx_var->get();

  if(!obj->mFrozen)
  {
    if(head->pCore->class_type(fx->pClass) == M_CLASS_METHOD)
    {
      ic_array *arr = (ic_array *)obj->mData;
      arr->iter_rewind();
      sc_voidmapitem *curr;

      while(curr = arr->iter_next())
      {
        head->rSRC.push((rc_var *)curr->mValue);
        head->method_invoke((rc_method *)fx->mData);
        if(head->sub_value(head->rSRC.get(0)))
          arr->unset(curr->mKey);
        head->cmd_clrsrc();
      }

      head->pCurrObj->mLinks++;
      head->rSRC.push(head->pCurrObj);
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "method", "reject!"), M_EXC_ARGS);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);

  head->obj_unlink(fx_var);
}

/**
 * Clears the array.
 */
void array_clear_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  if(!obj->mFrozen)
  {
    sc_voidmapitem *curr;
    ic_array *arr = (ic_array *)obj->mData;
    arr->iter_rewind();
    while(curr = arr->iter_next())
      head->obj_unlink((rc_var *)curr->mValue);

    arr->clear();
    head->pCurrObj->mLinks++;
    head->rSRC.push(head->pCurrObj);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Returns two arrays zipped.
 */
void array_zip(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();

  if(head->pCore->class_type(item->pClass) == M_CLASS_ARRAY)
  {
    ic_array *arr1 = (ic_array *)obj->mData, *arr2 = (ic_array *)item->mData;
    ic_array *res = new ic_array();
    bool first = true, second = true;
    sc_voidmapitem *curr;

    arr1->iter_rewind();
    arr2->iter_rewind();
    while(first && second)
    {
      if(first)
      {
        curr = arr1->iter_next();
        if(curr)
        {
          ((rc_var *)curr->mValue)->mLinks++;
          res->append((rc_var *)curr->mValue, false);
        }
        else
          first = false;
      }

      if(second)
      {
        curr = arr2->iter_next();
        if(curr)
        {
          ((rc_var *)curr->mValue)->mLinks++;
          res->append((rc_var *)curr->mValue, false);
        }
        else
          first = false;
      }
    }

    head->rSRC.push(head->new_array(res, obj->mTainted));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "array", "zip"), M_EXC_ARGS);

  head->obj_unlink(item_var);
}

/**
 * Zips two arrays into one.
 */
void array_zip_do(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();

  if(!obj->mFrozen)
  {
    if(head->pCore->class_type(item->pClass) == M_CLASS_ARRAY)
    {
      ic_array *arr1 = (ic_array *)obj->mData, *arr2 = (ic_array *)item->mData;
      ic_array *res = new ic_array();
      bool first = true, second = true;
      sc_voidmapitem *curr;

      arr1->iter_rewind();
      arr2->iter_rewind();
      while(first && second)
      {
        if(first)
        {
          curr = arr1->iter_next();
          if(curr)
          {
            ((rc_var *)curr->mValue)->mLinks++;
            res->append((rc_var *)curr->mValue, false);
          }
          else
            first = false;
        }

        if(second)
        {
          curr = arr2->iter_next();
          if(curr)
          {
            ((rc_var *)curr->mValue)->mLinks++;
            res->append((rc_var *)curr->mValue, false);
          }
          else
            first = false;
        }
      }

      head->var_save(head->pCurrObj, head->new_array(res, obj->mTainted));
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "array", "zip!"), M_EXC_ARGS);
  }
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);

  head->obj_unlink(item_var);
}

/**
 * Returns the minimal value from the array.
 */
void array_min(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  sc_voidmapitem *curr;

  if(arr->length() == 0)
    return;

  arr->iter_rewind();

  // minimal value is stored in AX register
  rc_var *ax = head->rAX;
  head->rAX = (rc_var *)arr->iter_next()->mValue;
  head->rAX->mLinks++;
  while(curr = arr->iter_next())
  {
    rc_var *tmp = (rc_var *)curr->mValue;
    tmp->mLinks++;
    head->rSRC.push(tmp);
    char res = head->sub_compare();

    // incomparable types
    if(res == M_CLASS_UNDEF)
    {
      head->warning(M_WARN_ARRAY_INCOMPARABLE, "min");
      head->obj_unlink(head->rSRC.pop());
      head->obj_unlink(head->rAX);
      head->rAX = ax;
      return;
    }
    else
    {
      // check if current is smaller
      int value = ((ic_int *)head->rSRC.get(0)->get()->mData)->mValue;
      if(value == -1)
      {
        head->obj_unlink(head->rAX);
        head->rAX = tmp;
        tmp->mLinks++;
      }
      // discard result
      head->obj_unlink(head->rSRC.pop());
    }
  }

  // mLinks ++ and -- omitted
  head->rSRC.push(head->rAX);
  head->rAX = ax;
}

/**
 * Returns the maximal value from the array.
 */
void array_max(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  sc_voidmapitem *curr;

  if(arr->length() == 0)
    return;

  arr->iter_rewind();

  // maximal value is stored in AX register
  rc_var *ax = head->rAX;
  head->rAX = (rc_var *)arr->iter_next()->mValue;
  head->rAX->mLinks++;
  while(curr = arr->iter_next())
  {
    rc_var *tmp = (rc_var *)curr->mValue;
    tmp->mLinks++;
    head->rSRC.push(tmp);
    char res = head->sub_compare();

    // incomparable types
    if(res == M_CLASS_UNDEF)
    {
      head->warning(M_WARN_ARRAY_INCOMPARABLE, "max");
      head->obj_unlink(head->rSRC.pop());
      head->obj_unlink(head->rAX);
      head->rAX = ax;
      return;
    }
    else
    {
      // check if current is larger
      int value = ((ic_int *)head->rSRC.get(0)->get()->mData)->mValue;
      if(value == 1)
      {
        head->obj_unlink(head->rAX);
        head->rAX = tmp;
        tmp->mLinks++;
      }
      // discard result
      head->obj_unlink(head->rSRC.pop());
    }
  }

  // mLinks ++ and -- omitted
  head->rSRC.push(head->rAX);
  head->rAX = ax;
}

/**
 * Returns both the minimal and the maximal items from the array.
 */
void array_min_max(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  sc_voidmapitem *curr;

  if(arr->length() == 0)
    return;

  arr->iter_rewind();

  // register first value as both minimal and maximal
  rc_var *min, *max;
  min = max = (rc_var *)((ic_array *)obj->mData)->iter_next()->mValue;

  while(curr = arr->iter_next())
  {
    rc_var *item = (rc_var *)curr->mValue;
    char minres = head->sub_compare(item, min);
    char maxres = head->sub_compare(item, max);

    // incomparable types
    if(minres == -2 || maxres == -2)
    {
      head->warning(M_WARN_ARRAY_INCOMPARABLE, "min_max");
      head->rSRC.push(head->new_undef());
      return;
    }

    // replace new items if needed
    if(minres == 1) min = item;
    if(maxres == -1) max = item;
  }

  ic_array *newarr = new ic_array();
  min->mLinks++;
  newarr->set("min", min);
  max->mLinks++;
  newarr->set("max", max);
  head->rSRC.push(head->new_array(newarr, obj->mTainted));
}

/**
 * Returns array's length.
 */
void array_length(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  long length = ((ic_array *)obj->mData)->length();
  head->rSRC.push(head->new_int(length, obj->mTainted));
}

/**
 * Returns the boolean representation of the array.
 */
void array_to_b(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  bool val = ((ic_array *)obj->mData)->to_b();
  head->rSRC.push(head->new_bool(val, obj->mTainted));
}

/**
 * Returns the integer representation of the array.
 */
void array_to_i(rc_head *head)
{
  array_length(head);
}

/**
 * Returns the string representation of the array.
 */
void array_to_s(rc_head *head)
{
  head->rSRC.push(head->new_string("array"));
}

/**
 * Inspect object.
 */
void array_inspect(rc_head *head)
{
  ic_string *str = new ic_string("{array: ");

  sc_voidmapitem *curr;
  ic_object *obj = head->pCurrObj->get();
  ic_array *arr = (ic_array *)obj->mData;
  long count = arr->length(), idx = 0;

  arr->iter_rewind();
  while(curr = arr->iter_next())
  {
    head->method_invoke("inspect", (rc_var *)curr->mValue);
    rc_var *item_var = head->rSRC.pop();
    ic_object *item = item_var->get();

    if(head->pCore->class_type(item->pClass) == M_CLASS_STRING)
    {
      str->append(curr->mKey);
      str->append(":");
      str->append((ic_string*)item->mData);

      // not the last item yet
      if(idx < count-1)
        str->append(", ");
    }
    else
    {
      head->obj_unlink(item_var);
      delete str;
      head->exception(ic_string::format(M_ERR_TYPE_EXPECTED, "string"), M_EXC_SCRIPT);
      return;
    }

    idx++;
    head->obj_unlink(item_var);
  }

  str->append("}");
  head->rSRC.push(head->new_string(str, obj->mTainted));
}

#endif