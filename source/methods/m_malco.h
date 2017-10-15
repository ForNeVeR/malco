/**
 * @file m_malco.h
 * @author impworks
 * Malco method header.
 * Defines all methods for Malco namespace.
 */

#ifndef M_MALCO_H
#define M_MALCO_H

/**
 * Returns Malco's build.
 */
void malco_build(rc_head *head)
{
  head->rSRC.push(head->new_int(MALCO_BUILD));
}

/**
 * Returns Malco's release.
 */
void malco_release(rc_head *head)
{
  head->rSRC.push(head->new_string(MALCO_RELEASE));
}

/**
 * Returns Malco's version
 */
void malco_version(rc_head *head)
{
  head->rSRC.push(head->new_string(MALCO_VERSION));
}

/**
 * Returns Malco's unicode flag.
 */
void malco_unicode(rc_head *head)
{
  head->rSRC.push(head->new_bool(MALCO_UNICODE ? true : false));
}

/**
 * Returns Malco's copyright statement.
 */
void malco_copyright(rc_head *head)
{
  head->rSRC.push(head->new_string(MALCO_COPYRIGHT));
}

/**
 * Sets / gets Malco's error mode.
 */
void malco_error_mode(rc_head *head)
{
  int mode = head->pCore->mErrorMode;

  if(head->rSRC.mLength == 1)
  {
    rc_var *modevar = head->rSRC.pop();
    ic_object *mode = modevar->get();
    if(head->pCore->class_type(mode->pClass) == M_CLASS_INT)
      head->pCore->mErrorMode = (int)((ic_int *)mode->mData)->mValue;
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int", "error_mode"), M_EXC_ARGS);
  }

  head->rSRC.push(head->new_int(mode));
}

/**
 * Tells how to make explosives.
 */
void malco_how_to_create_explosives(rc_head *head)
{
  rc_var *arrvar = head->new_array();
  ic_object *obj = arrvar->get();
  ((ic_array *)obj->mData)->append(head->new_string("Charcoal"));
  ((ic_array *)obj->mData)->append(head->new_string("Jellyfish juice"));
  ((ic_array *)obj->mData)->append(head->new_string("Gum base"));

  head->rSRC.push(arrvar);
}

#endif