/**
 * @file m_math.h
 * @author impworks, ForNeVeR.
 * Math method header.
 * Defines all methods for math static class.
 */

#ifndef M_MATH_H
#define M_MATH_H

/**
 * Returns an absolute value of the argument.
 */
void math_abs(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT)
  {
    long val = abs(((ic_int *)value->mData)->mValue);
    head->rSRC.push(head->new_int(val));
  }
  else if(cls == M_CLASS_FLOAT)
  {
    double val = abs(((ic_float *)value->mData)->mValue);
    head->rSRC.push(head->new_float(val));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "abs"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the sine value of the argument.
 */
void math_sin(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT)
  {
    val = sin((float)((ic_int *)value->mData)->mValue);
    head->rSRC.push(head->new_float(val));
  }
  else if(cls == M_CLASS_FLOAT)
  {
    val = sin(((ic_float *)value->mData)->mValue);
    head->rSRC.push(head->new_float(val));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "sin"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the hyperbolic sine value of the argument.
 */
void math_sinh(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT)
  {
    val = sinh((float)((ic_int *)value->mData)->mValue);
    head->rSRC.push(head->new_float(val));
  }
  else if(cls == M_CLASS_FLOAT)
  {
    val = sinh(((ic_float *)value->mData)->mValue);
    head->rSRC.push(head->new_float(val));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "sinh"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the cosine value of the argument.
 */
void math_cos(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT)
  {
    val = cos((float)((ic_int *)value->mData)->mValue);
    head->rSRC.push(head->new_float(val));
  }
  else if(cls == M_CLASS_FLOAT)
  {
    val = cos(((ic_float *)value->mData)->mValue);
    head->rSRC.push(head->new_float(val));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "cos"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the hyperbolic cosine value of the argument.
 */
void math_cosh(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT)
  {
    val = cosh((float)((ic_int *)value->mData)->mValue);
    head->rSRC.push(head->new_float(val));
  }
  else if(cls == M_CLASS_FLOAT)
  {
    val = cosh(((ic_float *)value->mData)->mValue);
    head->rSRC.push(head->new_float(val));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "cosh"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the tangent value of the argument.
 */
void math_tan(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT)
  {
    val = tan((float)((ic_int *)value->mData)->mValue);
    head->rSRC.push(head->new_float(val));
  }
  else if(cls == M_CLASS_FLOAT)
  {
    val = tan(((ic_float *)value->mData)->mValue);
    head->rSRC.push(head->new_float(val));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "tan"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the hyperbolic tangent value of the argument.
 */
void math_tanh(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT)
  {
    val = tanh((float)((ic_int *)value->mData)->mValue);
    head->rSRC.push(head->new_float(val));
  }
  else if(cls == M_CLASS_FLOAT)
  {
    val = tanh(((ic_float *)value->mData)->mValue);
    head->rSRC.push(head->new_float(val));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "tanh"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the arc sine value of the argument.
 */
void math_arcsin(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT || cls == M_CLASS_FLOAT)
  {
    if(cls == M_CLASS_INT)
      val = ((ic_int *)value->mData)->mValue;
    else
      val = ((ic_float *)value->mData)->mValue;

    if(abs(val) <= 1)
      head->rSRC.push(head->new_float(asin(val)));
    else
      head->warning(M_ERR_BAD_MATH, "arcsin(x > 1)");
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "arcsin"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the hyperbolic arc sine value of the argument.
 */
void math_arcsinh(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT || cls == M_CLASS_FLOAT)
  {
    if(cls == M_CLASS_INT)
      val = ((ic_int *)value->mData)->mValue;
    else
      val = ((ic_float *)value->mData)->mValue;

    if(abs(val) <= 1)
      head->rSRC.push(head->new_float(asinh(val)));
    else
      head->warning(M_ERR_BAD_MATH, "arcsinh(x > 1)");
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "arcsinh"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the arc cosine value of the argument.
 */
void math_arccos(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT || cls == M_CLASS_FLOAT)
  {
    if(cls == M_CLASS_INT)
      val = ((ic_int *)value->mData)->mValue;
    else
      val = ((ic_float *)value->mData)->mValue;

    if(abs(val) <= 1)
      head->rSRC.push(head->new_float(acos(val)));
    else
      head->warning(M_ERR_BAD_MATH, "arccos(x > 1)");
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "arccos"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the hyperbolic arc cosine value of the argument.
 */
void math_arccosh(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT || cls == M_CLASS_FLOAT)
  {
    if(cls == M_CLASS_INT)
      val = ((ic_int *)value->mData)->mValue;
    else
      val = ((ic_float *)value->mData)->mValue;

    if(abs(val) <= 1)
      head->rSRC.push(head->new_float(acosh(val)));
    else
      head->warning(M_ERR_BAD_MATH, "arccosh(x > 1)");
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "arccosh"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the arc tangent value of the argument.
 */
void math_arctan(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT || cls == M_CLASS_FLOAT)
  {
    if(cls == M_CLASS_INT)
      val = ((ic_int *)value->mData)->mValue;
    else
      val = ((ic_float *)value->mData)->mValue;

    head->rSRC.push(head->new_float(atan(val)));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "arctan"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the hyperbolic arc tangent value of the argument.
 */
void math_arctanh(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT || cls == M_CLASS_FLOAT)
  {
    if(cls == M_CLASS_INT)
      val = ((ic_int *)value->mData)->mValue;
    else
      val = ((ic_float *)value->mData)->mValue;

    head->rSRC.push(head->new_float(atanh(val)));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "arctanh"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the square root value of the argument.
 */
void math_sqrt(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT || cls == M_CLASS_FLOAT)
  {
    if(cls == M_CLASS_INT)
      val = ((ic_int *)value->mData)->mValue;
    else
      val = ((ic_float *)value->mData)->mValue;

    if(val > 0)
      head->rSRC.push(head->new_float(acosh(val)));
    else
      head->warning(M_ERR_BAD_MATH, "sqrt(x < 0)");
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "sqrt"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the cubic root value of the argument.
 */
void math_cbrt(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT || cls == M_CLASS_FLOAT)
  {
    if(cls == M_CLASS_INT)
      val = ((ic_int *)value->mData)->mValue;
    else
      val = ((ic_float *)value->mData)->mValue;

    head->rSRC.push(head->new_float(cbrt(val)));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "cbrt"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the exponent value of the argument.
 */
void math_exp(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT || cls == M_CLASS_FLOAT)
  {
    if(cls == M_CLASS_INT)
      val = ((ic_int *)value->mData)->mValue;
    else
      val = ((ic_float *)value->mData)->mValue;

    head->rSRC.push(head->new_float(exp(val)));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "exp"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the natural logarithm value of the argument.
 */
void math_log(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT || cls == M_CLASS_FLOAT)
  {
    if(cls == M_CLASS_INT)
      val = ((ic_int *)value->mData)->mValue;
    else
      val = ((ic_float *)value->mData)->mValue;

    if(val > 0)
      head->rSRC.push(head->new_float(log(val)));
    else
      head->warning(M_ERR_BAD_MATH, "log(x = 0)");
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "log"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the binary logarithm value of the argument.
 */
void math_log2(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT || cls == M_CLASS_FLOAT)
  {
    if(cls == M_CLASS_INT)
      val = ((ic_int *)value->mData)->mValue;
    else
      val = ((ic_float *)value->mData)->mValue;

    if(val > 0)
      head->rSRC.push(head->new_float(log2(val)));
    else
      head->warning(M_ERR_BAD_MATH, "log2(x = 0)");
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "log2"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the decimal value of the argument.
 */
void math_log10(rc_head *head)
{
  rc_var *value_var = head->rSRC.pop();
  ic_object *value = value_var->get();
  double val;

  char cls = head->pCore->class_type(value->pClass);

  if(cls == M_CLASS_INT || cls == M_CLASS_FLOAT)
  {
    if(cls == M_CLASS_INT)
      val = ((ic_int *)value->mData)->mValue;
    else
      val = ((ic_float *)value->mData)->mValue;

    if(val > 0)
      head->rSRC.push(head->new_float(log10(val)));
    else
      head->warning(M_ERR_BAD_MATH, "log10(x = 0)");
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or float", "log10"), M_EXC_ARGS);

  head->obj_unlink(value_var);
}

/**
 * Returns the PI constant.
 */
void math_pi(rc_head *head)
{
  head->rSRC.push(head->new_float(3.1415926535));
}

/**
 * Returns the exponent constant.
 */
void math_e(rc_head *head)
{
  head->rSRC.push(head->new_float(2.7182818284));
}

/**
 * Returns a random value.
 */
void math_random(rc_head *head)
{
  rc_var *first_var = head->rSRC.pop(), *second_var = head->rSRC.pop();
  ic_object *first = first_var->get(), *second = (second_var ? second_var->get() : NULL);

  if(second)
  {
    // parameters are int and int
    if(head->pCore->class_type(first->pClass) == M_CLASS_INT)
    {
      if(head->pCore->class_type(second->pClass) == M_CLASS_INT)
      {
        long min = ((ic_int *)first->mData)->mValue, max = ((ic_int *)second->mData)->mValue;
        head->rSRC.push(head->new_int(sc_random::get_next(min, max)));
      }
      else
        head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 2, "int", "random"), M_EXC_ARGS);
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int", "random"), M_EXC_ARGS);
  }
  else
  {
    char cls = head->pCore->class_type(first->pClass);
    if(cls == M_CLASS_INT)
    {
      // 0..max
      long max = ((ic_int *)first->mData)->mValue;
      head->rSRC.push(head->new_int(sc_random::get_next(max)));
    }
    else if(cls == M_CLASS_RANGE)
    {
      long min = ((ic_range *)first->mData)->mStart, max = ((ic_range *)first->mData)->mEnd;
      head->rSRC.push(head->new_int(sc_random::get_next(min, max)));
    }
    else
      head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int or range", "random"), M_EXC_ARGS);
  }
}

/**
 * Returns the minimal of the objects.
 */
void math_min(rc_head *head)
{
  rc_var *curr_var;
  ic_object *curr;
  double min, val;
  bool any = false, hasfloat = false, valid;
  while(curr_var = head->rSRC.pop())
  {
    curr = curr_var->get();
    char cls = head->pCore->class_type(curr->pClass);
    // valid tells the object is a proper number, int or float
    valid = true;

    if(cls == M_CLASS_INT)
      val = ((ic_int *)curr->mData)->mValue;
    else if(cls == M_CLASS_FLOAT)
    {
      val = ((ic_float *)curr->mData)->mValue;
      hasfloat = true;
    }
    else
      valid = false;

    if(valid && (!any || val < min))
    {
      // the object is a number and it's either currently smallest or first
      min = val;
      any = true;
    }

    // remove variable
    head->obj_unlink(curr_var);
  }

  if(any)
  {
    if(!hasfloat)
      head->rSRC.push(head->new_int((long)min));
    else
      head->rSRC.push(head->new_float(min));
  }
}

/**
 * Returns the maximal of the objects.
 */
void math_max(rc_head *head)
{
  rc_var *curr_var;
  ic_object *curr;
  double max, val;
  bool any = false, hasfloat = false, valid;
  while(curr_var = head->rSRC.pop())
  {
    curr = curr_var->get();
    char cls = head->pCore->class_type(curr->pClass);
    // valid tells the object is a proper number, int or float
    valid = true;

    if(cls == M_CLASS_INT)
      val = ((ic_int *)curr->mData)->mValue;
    else if(cls == M_CLASS_FLOAT)
    {
      val = ((ic_float *)curr->mData)->mValue;
      hasfloat = true;
    }
    else
      valid = false;

    if(valid && (!any || val > max))
    {
      // the object is a number and it's either currently smallest or first
      max = val;
      any = true;
    }

    // remove variable
    head->obj_unlink(curr_var);
  }

  if(any)
  {
    if(!hasfloat)
      head->rSRC.push(head->new_int((long)max));
    else
      head->rSRC.push(head->new_float(max));
  }
}

void math_even(rc_head *head)
{
  rc_var *var = head->rSRC.pop();
  ic_object *obj = var->get();
  if(head->pCore->class_type(obj->pClass) == M_CLASS_INT)
  {
    bool result = ((ic_int *)obj->mData)->mValue % 2 == 0;
    head->rSRC.push(head->new_bool(result));
  }
  else
    head->exception(ic_string::format(M_ERR_FX_WRONG_TYPE, 1, "int", "even"), M_EXC_ARGS);
}

/**
 * Converts argument from one base to another.
 * @todo implement.
 */
void math_base_cvt(rc_head *head)
{

}

#endif
