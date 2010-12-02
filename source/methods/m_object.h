/**
 * @file m_object.h
 * @author impworks.
 * Object method header.
 * Defines all methods for object class.
 */

#ifndef M_OBJECT_H
#define M_OBJECT_H

/**
 * Creates an object of a specified class.
 */
void object_op_create(rc_head *head)
{
  ic_object *name = head->var_get(head->rSRC.get(0));
  char namecls = head->pCore->class_type(name->pClass);
  if(namecls == M_CLASS_STRING)
  {
    rc_class *cls = head->pCore->class_resolve(((ic_string *)name->mData)->get(), head->pCore->pClassRoot);
    if(cls)
      head->var_save(head->pCurrObj, head->obj_create(cls));
    else
      head->exception(ic_string::format(M_ERR_NO_CLASS, ((ic_string *)name->mData)->get()), M_EXC_SCRIPT);
  }
  else if(namecls == M_CLASS_CLASS)
  {
    rc_class *cls = (rc_class *)name->mData;
    head->var_save(head->pCurrObj, head->obj_create(cls));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string or class", "#create"), M_EXC_ARGS);

  head->cmd_clrsrc();
}

/**
 * Returns boolean, whether object is tainted or not.
 */
void object_tainted(rc_head *head)
{
  head->rSRC.push(head->new_bool(head->pCurrObj->get()->mTainted));
}

/**
 * Sets object as tainted.
 */
void object_taint_do(rc_head *head)
{
  ic_object *curr = head->pCurrObj->get();
  if(!curr->mFrozen)
    curr->mTainted = true;
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Sets object as untainted.
 */
void object_untaint_do(rc_head *head)
{
  ic_object *curr = head->pCurrObj->get();
  if(!curr->mFrozen)
    curr->mTainted = false;
  else
    head->exception(M_ERR_FROZEN, M_EXC_SCRIPT);
}

/**
 * Returns boolean, whether object is frozen or not.
 */
void object_frozen(rc_head *head)
{
  head->rSRC.push(head->new_bool(head->pCurrObj->get()->mFrozen));
}

/**
 * Freezes an object.
 */
void object_freeze_do(rc_head *head)
{
  head->pCurrObj->get()->mFrozen = true;
  head->pCurrObj->mLinks++;
  head->rSRC.push(head->pCurrObj);
}

/**
 * Calls a specific method on an object.
 */
void object_exec(rc_head *head)
{
  rc_var *name_var = head->rSRC.pop();
  ic_object *name = name_var->get();
  if(head->pCore->class_type(name->pClass) == M_CLASS_STRING)
  {
    ic_string *name_str = (ic_string *)name->mData;

    // make sure it's really a method, not an operator
    if(name_str->char_at(0) != '#')
      head->method_invoke(name_str->get(), head->pCurrObj);
    else
      head->exception(M_ERR_DIRECT_OP_INVOKE, M_EXC_SCRIPT);
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "method", "exec"), M_EXC_SCRIPT);

  head->obj_unlink(name_var);
}

/**
 * Asserts that the object is any of the given classes.
 */
void object_assert(rc_head *head)
{
  char *name = head->pCurrObj->get()->pClass->mName;
  rc_var *curr;
  bool asserted = false;
  int count = 1;
  while(curr = head->rSRC.pop())
  {
    if(head->pCore->class_type(curr->get()->pClass) == M_CLASS_STRING)
    {
      if(((ic_string *)curr->get()->mData)->compare(name) == 0)
      {
        asserted = true;
        break;
      }
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, count, "string", "assert"), M_EXC_ARGS);

    count++;
  }

  head->rSRC.push(head->new_bool(asserted));
}

/**
 * Returns an object id (actually it's pointer).
 */
void object_id(rc_head *head)
{
  head->rSRC.push(head->new_int((long)head->pCurrObj->get()));
}

/**
 * Returns object's class.
 */
void object_class(rc_head *head)
{
  head->rSRC.push(head->new_class(head->pCurrObj->get()->pClass));
}

/**
* Returns object's class name.
*/
void object_class_name(rc_head *head)
{
  head->rSRC.push(head->new_string(head->pCurrObj->get()->pClass->mName));
}

/**
 * Checks if the object has specified member.
 */
void object_has_member(rc_head *head)
{
  rc_var *name_var = head->rSRC.pop();
  ic_object *name = name_var->get();
  if(head->pCore->class_type(name->pClass) == M_CLASS_STRING)
  {
    bool exists = head->pCurrObj->get()->mMembers->get((ic_string *)name->mData) != NULL;
    head->cmd_clrsrc();
    head->rSRC.push(head->new_bool(exists));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string", "has_member"), M_EXC_ARGS);
}

/**
 * Checks if the object has specified method.
 */
void object_has_method(rc_head *head)
{
  rc_var *name_var = head->rSRC.pop();
  ic_object *name = name_var->get();
  if(head->pCore->class_type(name->pClass) == M_CLASS_STRING)
  {
    bool exists = head->method_resolve(((ic_string *)name->mData)->get(), head->pCurrObj->get()->pClass) != NULL;
    head->rSRC.push(head->new_bool(exists));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string", "has_method"), M_EXC_ARGS);

  head->obj_unlink(name_var);
}

/**
 * Returns object's debug representation.
 */
void object_inspect(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_string *str = new ic_string("{");
  str->append(head->pCurrObj->get()->pClass->mName);
  str->append("}");

  head->rSRC.push(head->new_string(str, obj->mTainted));
}

/**
 * Returns an array representation of an object.
 */
void object_to_a(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  head->rSRC.push(head->new_array(obj->mTainted));
}

/**
 * Returns a boolean representation of an object.
 */
void object_to_b(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  head->rSRC.push(head->new_bool(false, obj->mTainted));
}

/**
 * Returns a float representation of an object.
 */
void object_to_f(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  head->rSRC.push(head->new_float(0.0, obj->mTainted));
}

/**
 * Returns an integer representation of an object.
 */
void object_to_i(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  head->rSRC.push(head->new_int(0L, obj->mTainted));
}

/**
 * Returns a string representation of an object.
 */
void object_to_s(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  head->rSRC.push(head->new_string(obj->pClass->mName, obj->mTainted));
}

/**
 * Prints out arguments.
 */
void object_print(rc_head *head)
{
  rc_var *var;
  sc_voidarray items;
  while(var = head->rSRC.pop())
    items.add((void *)var);

  for(int idx = 0; idx < items.length(); idx++)
  {
    var = (rc_var *)items.get(idx);
    rc_var *curr = head->convert_string(var);
    ic_string *str = (ic_string *)curr->get()->mData;
    printf("%s", str->get());

    head->obj_unlink(var);
    if(var != curr)
      head->obj_unlink(curr);
  }
}

#endif
