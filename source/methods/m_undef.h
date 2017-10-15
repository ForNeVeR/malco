/**
 * @file m_undef.h
 * @author impworks
 * Array method header.
 * Defines all methods for array class.
 */

#ifndef M_UNDEF_H
#define M_UNDEF_H

/**
 * Perform any operation on an undef.
 */
void undef_op_any(rc_head *head)
{
  head->obj_unlink(head->rSRC.pop());
  head->rSRC.push(head->new_undef());
}

/**
 * Compare to undef.
 */
void undef_op_cmp_any(rc_head *head)
{
  head->obj_unlink(head->rSRC.pop());
  head->rSRC.push(head->new_bool(false));
}

/**
 * Relate to undef.
 */
void undef_op_rel_any(rc_head *head)
{
  ic_object *item = head->rSRC.get(0)->get();
  bool res        = false;

  if(item->pClass == head->pCore->mClassCache.pUndef) res = true;

  head->obj_unlink(head->rSRC.pop());
  head->rSRC.push(head->new_bool(res));
}

/**
 * Index an undef.
 */
void undef_op_index(rc_head *head)
{
  head->cmd_clrsrc();
  head->rSRC.push(head->new_undef());
}

/**
 * Inspect object.
 */
void undef_inspect(rc_head *head)
{
  head->rSRC.push(head->new_string("{undef}"));
}

#endif