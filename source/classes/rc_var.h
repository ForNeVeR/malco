/**
 * @file rc_var.h
 * @author impworks.
 * rc_var header.
 * Defines properties and methods of rc_tape class.
 */

#ifndef RC_VAR_H
#define RC_VAR_H

/**
 * rc_var default constructor.
 */
rc_var::rc_var()
{
//  mName = NULL;
  mProperties = 0;
  mLinks = 1;
  //pBaseVar = NULL;
  //pBaseClass = NULL;
  pObj = NULL;
}

/**
 * rc_var object-based constructor.
 */
rc_var::rc_var(ic_object *obj)
{
  //mName = NULL;
  mProperties = M_PROP_CONST;
  mLinks = 1;
  //pBaseVar = NULL;
  //pBaseClass = NULL;
  pObj = (void *)obj;
  obj->mLinks++;
}

/**
 * rc_var variable-based constructor.
 */
rc_var::rc_var(rc_var *obj)
{
  //mName = NULL;
  mProperties = M_PROP_LINK;
  mLinks = 1;
  //pBaseVar = NULL;
  //pBaseClass = NULL;
  pObj = (void *)obj;
  obj->mLinks++;
}

/**
 * Returns the object, dereferencing links.
 * Required for places outside rc_head where variable is to be dereferenced.
 * @return ic_object.
 */
inline ic_object *rc_var::get()
{
  rc_var *var = this;
  // process links
  while(var && var->mProperties & M_PROP_LINK)
    var = (rc_var *)var->pObj;

  // return an object if one exists
  if(var)
    return (ic_object *)var->pObj;
  else
    return NULL;
}

#endif