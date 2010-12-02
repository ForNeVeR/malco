/**
 * @file m_exception.h
 * @author impworks
 * Exception method header.
 * Defines all methods for exception class.
 */

#ifndef M_EXCEPTION_H
#define M_EXCEPTION_H

/**
 * Exception constructor.
 */
void exception_op_create(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = (item_var ? item_var->get() : NULL);

  if(item)
  {
    char cls = head->pCore->class_type(item->pClass);
    if(cls == M_CLASS_STRING)
    {
      ((sc_exception *)obj->mData)->mErrorMsg->set((ic_string *)item->mData);
    }
    else if(cls != M_CLASS_UNDEF)
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "string", "new exception"), M_EXC_ARGS);
  }

  head->obj_unlink(item_var);
}

/**
 * Compare exception to exception.
 */
void exception_op_cmp_exception(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  ic_string *left = ((sc_exception *)obj->mData)->mErrorMsg, *right = ((sc_exception *)item->mData)->mErrorMsg;
  // note the -1 part to reverse values, as item is at rightside
  int result = left->compare(right);
  result = (result < 0 ? -1 : (result > 0 ? 1 : 0));
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare exception to bool.
 */
void exception_op_cmp_bool(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  // an exception cannot be 'false'
  bool left = true, right = ((ic_bool *)item->mData)->mValue;
  char result = (left == right ? 0 : 1);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Compare exception to string.
 */
void exception_op_cmp_string(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  rc_var *item_var = head->rSRC.pop();
  ic_object *item = item_var->get();
  ic_string *str = ((sc_exception *)obj->mData)->mErrorMsg;
  // note the -1 part to reverse values, as item is at rightside
  char result = ((ic_string *)item->mData)->compare(str) * (-1);
  head->rSRC.push(head->new_int(result));
  head->obj_unlink(item_var);
}

/**
 * Return the exception's message.
 */
void exception_msg(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_string *msg = new ic_string(*((sc_exception *)obj->mData)->mErrorMsg);
  head->rSRC.push(head->new_string(msg, obj->mTainted));
}

/**
 * Return the exception's type as string.
 */
void exception_type(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  char type = ((sc_exception *)obj->mData)->mErrorType;
  head->rSRC.push(head->new_int(type, obj->mTainted));
}

/**
 * Return the exception's file.
 */
void exception_file(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  const char *file = ((sc_exception *)obj->mData)->mFile;
  head->rSRC.push(head->new_string(file, obj->mTainted));
}

/**
 * Return the exception's line.
 */
void exception_line(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  long line = ((sc_exception *)obj->mData)->mLine;
  head->rSRC.push(head->new_int(line, obj->mTainted));
}

/**
 * Inspect exception.
 */
void exception_inspect(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  sc_exception *ex = (sc_exception *)obj->mData;
  ic_int line;
  line.mValue = ex->mLine;
  ic_string *str = new ic_string("{exception:");
  str->append(ex->mErrorMsg);
  str->append(" @ ");
  str->append(ex->mFile);
  str->append(":");
  str->append(line.to_s());
  str->append("}");
  head->rSRC.push(head->new_string(str, obj->mTainted));
}

/**
 * Convert to boolean.
 */
void exception_to_b(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  head->rSRC.push(head->new_bool(true, obj->mTainted));
}

/**
 * Convert to string.
 */
void exception_to_s(rc_head *head)
{
  ic_object *obj = head->pCurrObj->get();
  ic_string *msg = new ic_string(*((sc_exception *)obj->mData)->mErrorMsg);
  head->rSRC.push(head->new_string(msg, obj->mTainted));
}

#endif