/**
 * @file rc_head.h
 * @author impworks.
 * rc_head header.
 * Defines properties and methods of rc_head class.
 */

#ifndef RC_HEAD_H
#define RC_HEAD_H

/**
 * rc_head constructor.
 */
rc_head::rc_head(rc_core *core)
{
  // set misc stuff
  pCore = core;
  mLine = mOffset = 0;
  strcpy(mFile, "<unknown>");
  mStartTime    = clock();
  mStatCommands = mStatFiles = mStatLines = 0;
  mExternalScope                          = false;
  mVars                                   = new sc_voidarray();

  // define registers
  rAX = NULL;
  rCS.resize(16);
}

/**
 * rc_head destructor.
 * @todo remake properly
 */
rc_head::~rc_head()
{
  if(rAX) obj_unlink(rAX);

  cmd_clrsrc();
  cmd_clrdst();

  while(rUS.mLength)
    cmd_popus();

#if MALCO_DEBUG
  clock_t end_time = clock();
  printf("\nExec time: %f s", ((float)end_time - (float)mStartTime) / CLOCKS_PER_SEC);
#endif
}

/**
 * Starts playing the tape.
 * @return The return value of the script.
 */
int rc_head::playback(bool main)
{
  // main execution loop initialization
  if(main)
  {
    pCurrClass = pTmpClass = pCore->mClassCache.pObject;
    pCurrObj               = NULL;
  }

  while((pCmd = pCore->mTape->select(mOffset)) != NULL && pCore->mState != M_STATE_DEAD)
  {
    // exit
    if(pCmd->mCmd == RASM_CMD_EXIT) break;

    execute();

    if(!main && pCmd->mCmd == RASM_CMD_RETURN) break;

    mStatCommands++;

    if(pCmd->mCmd != RASM_CMD_JFALSE && pCmd->mCmd != RASM_CMD_JTRUE && pCmd->mCmd != RASM_CMD_JMP) mOffset++;
  }

  return 0;
}

/**
 * Executes current command.
 */
void rc_head::execute()
{
  switch(pCmd->mCmd)
  {
    case RASM_CMD_LOADAX: cmd_loadax(); break;
    case RASM_CMD_LOADBX: cmd_loadbx(); break;
    case RASM_CMD_SAVEAX: cmd_saveax(); break;
    case RASM_CMD_SAVEBX: cmd_savebx(); break;
    case RASM_CMD_XCHG: cmd_xchg(); break;
    case RASM_CMD_ASSIGN: cmd_assign(); break;
    case RASM_CMD_UNSPLASSIGN: cmd_unsplassign(); break;

    case RASM_CMD_ADD: cmd_add(); break;
    case RASM_CMD_SUB: cmd_sub(); break;
    case RASM_CMD_MUL: cmd_mul(); break;
    case RASM_CMD_DIV: cmd_div(); break;
    case RASM_CMD_MOD: cmd_mod(); break;
    case RASM_CMD_POW: cmd_pow(); break;
    case RASM_CMD_SHL: cmd_shl(); break;
    case RASM_CMD_SHR: cmd_shr(); break;
    case RASM_CMD_BAND: cmd_band(); break;
    case RASM_CMD_BOR: cmd_bor(); break;
    case RASM_CMD_BXOR: cmd_bxor(); break;
    case RASM_CMD_AND: cmd_and(); break;
    case RASM_CMD_OR: cmd_or(); break;
    case RASM_CMD_XOR: cmd_xor(); break;

    case RASM_CMD_INC: cmd_inc(); break;
    case RASM_CMD_DEC: cmd_dec(); break;
    case RASM_CMD_NEG: cmd_neg(); break;

    case RASM_CMD_EQ: cmd_eq(); break;
    case RASM_CMD_EQ_STRICT: cmd_eq_strict(); break;
    case RASM_CMD_REL: cmd_rel(); break;
    case RASM_CMD_LESS: cmd_less(); break;
    case RASM_CMD_LESS_EQ: cmd_less_eq(); break;
    case RASM_CMD_GREATER: cmd_greater(); break;
    case RASM_CMD_GREATER_EQ: cmd_greater_eq(); break;
    case RASM_CMD_CMP: cmd_cmp(); break;
    case RASM_CMD_JTRUE: cmd_jtrue(); break;
    case RASM_CMD_JFALSE: cmd_jfalse(); break;
    case RASM_CMD_JMP: cmd_jmp(); break;

    case RASM_CMD_PUSHUS: cmd_pushus(); break;
    case RASM_CMD_POPUS: cmd_popus(); break;
    case RASM_CMD_PUSHSRC: cmd_pushsrc(); break;
    case RASM_CMD_PUSHDST: cmd_pushdst(); break;
    case RASM_CMD_POPSRC: cmd_popsrc(); break;
    case RASM_CMD_POPDST: cmd_popdst(); break;
    case RASM_CMD_SPLAT: cmd_splat(); break;
    case RASM_CMD_UNSPLAT: cmd_unsplat(); break;
    case RASM_CMD_MOVESRC: cmd_movesrc(); break;
    case RASM_CMD_MOVEDST: cmd_movedst(); break;
    case RASM_CMD_CLRSRC: cmd_clrsrc(); break;
    case RASM_CMD_CLRDST: cmd_clrdst(); break;

    case RASM_CMD_NEW: cmd_new(); break;
    case RASM_CMD_CALL: cmd_call(); break;
    case RASM_CMD_RETURN: cmd_return(); break;
    case RASM_CMD_NSP: cmd_nsp(); break;
    case RASM_CMD_BINDLAMBDA: cmd_bindlambda(); break;

    case RASM_CMD_INDEX: cmd_index(); break;

    case RASM_CMD_INCLUDE: cmd_include(); break;
    case RASM_CMD_REQUIRE: cmd_require(); break;
    case RASM_CMD_GC: cmd_gc(); break;
    case RASM_CMD_SETPTY: cmd_setpty(); break;
    case RASM_CMD_SETLINE: cmd_setline(); break;
    case RASM_CMD_SETFILE: cmd_setfile(); break;
    case RASM_CMD_THROW: cmd_throw(); break;
    case RASM_CMD_TRY: cmd_try(); break;
    case RASM_CMD_TRIED: cmd_tried(); break;

    case RASM_CMD_REGCLASS: cmd_regclass(); break;
    case RASM_CMD_REGPROPERTY: cmd_regproperty(); break;
    case RASM_CMD_REGMETHOD: cmd_regmethod(); break;

    case RASM_CMD_INSPECT: cmd_inspect(); break;
  }
}

/**
 * Saves current state into CS for method invocation.
 */
void rc_head::state_save()
{
  rc_headstate *state   = new rc_headstate();
  state->rAX            = rAX;
  state->mVars          = mVars;
  state->pStateClass    = pCurrClass;
  state->pStateTmpClass = pTmpClass;
  state->pStateObj      = pCurrObj;
  state->mOffset        = mOffset;
  state->mExternalScope = mExternalScope;
  rCS.add((void *)state);

  rAX = NULL;
}

/**
 * Restores current state from CS after method invocation.
 */
void rc_head::state_load()
{
  if(rCS.length())
  {
    rc_headstate *state = (rc_headstate *)rCS.get(rCS.length() - 1);
    rAX                 = state->rAX;
    mVars               = state->mVars;
    pCurrClass          = state->pStateClass;
    pTmpClass           = state->pStateTmpClass;
    pCurrObj            = state->pStateObj;
    mExternalScope      = state->mExternalScope;
    mOffset             = state->mOffset;

    rCS.del(rCS.length() - 1);
  }
  else
    exception(M_ERR_RETURN_1);
}

/**
 * Finds a method by it's name.
 * @param name Method name.
 * @param root Root class to start search from.
 * @return Pointer to resolved method.
 */
rc_method *rc_head::method_resolve(const char *name, rc_class *root)
{
  while(root != NULL)
  {
    rc_method *method = (rc_method *)root->mMethods.get(name);
    if(method) return method;
    root = root->pParent;
  }

  return NULL;
}

/**
 * Invokes a method of a specified object.
 * @param name Method name.
 * @param obj Object to call method on.
 * @param report Flag indicating whether to report method inexistance or not.
 */
inline void rc_head::method_invoke(const char *name, rc_var *object, bool report)
{
  rc_class *cls     = (object ? object->get()->pClass : pTmpClass);
  rc_method *method = method_resolve(name, cls);
  if(method)
    method_invoke(method, object);
  else if(report)
    exception(ic_string::format(M_ERR_NO_FX, name, pTmpClass->mName), M_EXC_NOT_FOUND);
}

/**
 * Invokes a method of a specified object.
 * @param method Method pointer.
 * @param obj Object to call method on.
 * @param report Flag indicating whether to report method inexistance or not.
 */
void rc_head::method_invoke(rc_method *method, rc_var *object)
{
  if(!method)
  {
    exception(ic_string::format(M_ERR_NO_FX, "<unknown>", pTmpClass->mName), M_EXC_NOT_FOUND);
    return;
  }

  // ensure method can be called: if it's dynamical and called statically, throw
  // an error
  if(!(method->mProperties & M_PROP_STATIC) && !object)
    exception(ic_string::format(M_ERR_BAD_STATIC_CALL, method->mName), M_EXC_SCRIPT);

  // validate method's privacy restrictions
  if(((method->mProperties & M_PROP_INTERNAL) && var_get(object)->pClass != pCurrClass) ||
     ((method->mProperties & M_PROP_PRIVATE) && object != pCurrObj))
    exception(ic_string::format(M_ERR_FX_PRIVATE, method->mName), M_EXC_NOT_FOUND);

  // validate method's number of arguments
  if(pCmd->mModifier != RASM_MOD_NAMES && rSRC.mLength < method->mMinArgs)
    exception(ic_string::format(M_ERR_FX_FEW_PARAMS, method->mName), M_EXC_ARGS);

  if(pCmd->mModifier != RASM_MOD_NAMES && rSRC.mLength > method->mMaxArgs && !method->mSplatArgs)
    exception(ic_string::format(M_ERR_FX_MANY_PARAMS, method->mName), M_EXC_ARGS);

  // everything's okay
  state_save();

  if(!method->mNative)
  {
    // prepare scope
    if(method->pExternalScope)
    {
      mExternalScope = true;
      mVars          = method->pExternalScope;
    }
    else
    {
      mExternalScope = false;
      mVars          = new sc_voidarray();
    }
  }


  // calling via argument names?
  if(pCmd->mModifier == RASM_MOD_NAMES)
  {
    rc_var *var   = rSRC.pop();
    ic_array *arr = (ic_array *)var_get(var)->mData;
    for(long idx = 0; idx < method->mParams.length(); idx++)
    {
      ic_string *name = (ic_string *)method->mParams.get(idx);
      rc_var *obj     = arr->get(name);
      arr->unset(name);
      // whole array gets deleted, each object will be unlinked, thus we need to
      // keep the count
      obj->mLinks++;
      if(obj)
        rSRC.push(obj);
      else
        rSRC.push(new_undef());
    }

    // extra parameter warning
    if(arr->length() > 0)
    {
      arr->iter_rewind();
      warning(M_WARN_FX_UNKNOWN_PARAM, method->mName, arr->iter_next()->mKey);
    }

    obj_unlink(var);
  }
  else
  {
    // pad rSRC with undefs until it has proper number of arguments
    while(rSRC.mLength < method->mMaxArgs)
      rSRC.push(new_undef());
  }

  pCurrClass = method->pClass;
  pCurrObj   = (method->mProperties & M_PROP_STATIC) ? NULL : object;

  // execute the method actually
  if(method->mNative)
  {
    (method->pNativeFunc)(this);
    // reset state to previous
    state_load();
  }
  else
  {
    mOffset = method->mExecPoint;
    playback(false);
  }
}

/**
 * Selects a namespace.
 * @param name Namespace name in current visibility level.
 */
inline void rc_head::nsp_select(const char *name)
{
  // fast check: name == ""?
  if(*name == 0)
    pTmpClass = pCore->class_resolve("object");
  else
  {
    // special cases
    if(strcmp(name, "self") == 0)
      pTmpClass = pCurrClass;
    else if(strcmp(name, "parent") == 0)
    {
      if(pTmpClass->pParent)
        pTmpClass = pTmpClass->pParent;
      else
        exception(M_ERR_OBJECT_PARENT, M_EXC_SCRIPT);
    }
    else if(strcmp(name, "root") == 0)
    {
      if(pTmpClass->pRoot)
        pTmpClass = pTmpClass->pRoot;
      else
        exception(M_ERR_CLASS_ROOT, M_EXC_SCRIPT);
    }
    else
      pTmpClass = pCore->class_resolve(name, pTmpClass);
  }
}

/**
 * Unselects any namespace.
 */
inline void rc_head::nsp_flush()
{
  pTmpClass = pCurrClass;
}

/**
 * Returns an ic_object from the rc_var, considering links.
 * @param var Variable to extract object from.
 * @return ic_object
 */
ic_object *rc_head::var_get(rc_var *var)
{
  return var->get();
}

/**
 * Saves an object into a local variable.
 * @param var Variable to accept the object.
 * @param obj Object to be saved.
 * @param cls Class the variable belongs to.
 */
void rc_head::var_save(rc_var *var, ic_object *obj)
{
  // check if variable is a link
  while(var && var->mProperties & M_PROP_LINK)
    var = (rc_var *)var->pObj;

  // check for dumb cases
  if(!var) return;

  obj_unlink((ic_object *)var->pObj);
  var->pObj = obj;
  obj->mLinks++;
}

/**
 * Saves an object into a local variable.
 * @param var Variable to accept the object.
 * @param obj Variable to extract object from (maybe).
 * @param cls Class the variable belongs to.
 */
void rc_head::var_save(rc_var *var, rc_var *obj)
{
  // check for dumb cases
  if(!var || !obj) return;
  if(var->mProperties & M_PROP_CONST)
  {
    exception(M_ERR_ASSIGN_CONST, M_EXC_SCRIPT);
    return;
  }

  if(obj->mProperties & M_PROP_LINK)
  {
    ((rc_var *)obj->pObj)->mLinks++;

    // if assigned variable is a link, the current variable gets relinked
    if(var->mProperties & M_PROP_LINK)
    {
      obj_unlink((rc_var *)var->pObj);
      var->pObj = obj->pObj;
    }
    else
    {
      obj_unlink((ic_object *)var->pObj);
      var->mProperties |= M_PROP_LINK;
      var->pObj = obj->pObj;
    }
  }
  else
  {
    // if the variable is immediate, it gets copied
    var_save(var, (ic_object *)obj->pObj);
  }
}

/**
 * Retrieve a value from the current scope by it's id.
 * @param id Variable id
 * @return Variable
 */
rc_var *rc_head::scope_get(long id)
{
  if(mVars->length() <= id)
  {
    mVars->resize(id + 1);
    rc_var *var      = new_undef();
    var->mProperties = 0;
    mVars->set(id, (void *)var);
    return var;
  }
  else
    return (rc_var *)mVars->get(id);
}

/**
 * Resolve a bare identifier, possibly a class / method / constant.
 * @param name Identifier name
 * @return Related object
 */
rc_var *rc_head::bare_id_resolve(const char *name)
{
  // first try a class (more common)
  rc_class *cls = (rc_class *)pTmpClass->mClasses.get(name);
  if(cls) return new_class(cls, false);

  // then attempt a method
  rc_method *mth = (rc_method *)pTmpClass->mMethods.get(name);
  if(mth) return new_method(mth, false);

  exception(ic_string::format(M_ERR_BAD_IDENTIFIER, name, pTmpClass->mName), M_EXC_NOT_FOUND);

  return NULL;
}

/**
 * Finds a member by it's name.
 * @param name Member name.
 * @param obj Object to find member in.
 * @param cls Root class to start search from.
 * @return Pointer to resolved member.
 */
inline rc_var *rc_head::member_resolve(const char *name, rc_var *obj, rc_class *cls)
{
  rc_var *var;
  bool dynamic = false; // false = member is static, belongs to class
  bool ok      = true;
  if(obj)
  {
    cls = obj->get()->pClass;
    var = (rc_var *)(obj->get()->mMembers->get(name));
    if(var)
      dynamic = true;
    else
      var = (rc_var *)(cls->mStaticMembers.get(name));
  }
  else
    var = (rc_var *)cls->mStaticMembers.get(name);

  // variable has not been found
  if(!var)
  {
    exception(ic_string::format(M_ERR_NO_PROPERTY, name, cls->mName), M_EXC_SCRIPT);
    return NULL;
  }

  // check permissions
  if(var->mProperties & M_PROP_PRIVATE)
  {
    // private means only the object itself can access it's properties
    if(dynamic)
    {
      // dynamic: must be requested from the object itself
      if(obj != pCurrObj) ok = false;
    }
    else
    {
      // static: only from the class itself
      if(cls != pTmpClass) ok = false;
    }
  }
  else if(var->mProperties & M_PROP_INTERNAL)
  {
    // internal means that objects of the same class may alter each other's
    // properties, and descendant objects can do it
    ok = pCore->class_is_heir(pTmpClass, cls);
  }

  if(!ok)
  {
    exception(ic_string::format(M_ERR_PROPERTY_PRIVATE, name, cls->mName), M_EXC_SCRIPT);
    return NULL;
  }

  if(!var->get()) var_save(var, new_undef());

  return var;
}

/**
 * Reports a warning to core.
 * @param msg Warning message.
 */
void rc_head::warning(const char *msg, ...)
{
  ic_string *str = ic_string::format("%s (file %s, line %i)", msg, mFile, mLine);
  va_list args;
  va_start(args, msg);
  pCore->error_any(ic_string::format_list(str->get(), args), M_EMODE_WARNING);
  va_end(args);
  delete str;
}

/**
 * Reports a notice to core.
 * @param msg Notice message.
 */
void rc_head::notice(const char *msg, ...)
{
  ic_string *str = ic_string::format("%s (file %s, line %i)", msg, mFile, mLine);
  va_list args;
  va_start(args, msg);
  pCore->error_any(ic_string::format_list(str->get(), args), M_EMODE_NOTICE);
  va_end(args);
  delete str;
}

/**
 * Throws an exception.
 * @param msg Exception message
 * @param type Exception type
 */
void rc_head::exception(const char *msg, int type)
{
  if(pCore->mRunMode != M_RUNMODE_SLICK || !(type & M_EXC_SLIGHT))
  {
    obj_unlink(rAX);
    rAX = new_exception(msg, type);
    cmd_throw();
  }
}

/**
 * Throws an exception.
 * @param msg Exception message
 * @param type Exception type
 */
void rc_head::exception(ic_string *msg, int type)
{
  if(pCore->mRunMode != M_RUNMODE_SLICK || !(type & M_EXC_SLIGHT))
  {
    obj_unlink(rAX);
    rAX = new_exception(msg, type);
    cmd_throw();
  }
}

/**
 * Returns object's debug info.
 * @param obj Object to be inspected.
 * @return String representation of the object.
 * @todo implement
 */
ic_string *rc_head::obj_inspect(rc_var *obj)
{
  return NULL;
}

/**
 * Creates a new object of a given class.
 * @param objclass Requested class.
 * @return Pointer to created object.
 */
rc_var *rc_head::obj_create(rc_class *objclass)
{
  int datatype = pCore->class_type(objclass);
  rc_var *obj  = new rc_var(new ic_object(objclass, new_basic(datatype)));

  // call in-class initialization
  // #init may not have parameters, otherwise it will fail the constructor,
  // but since it's only generated by compiler there should be no problems
  method_invoke("#init", obj, false);
  // call constructor
  method_invoke("#create", obj, false);

  return obj;
}

/**
 * Creates a copy of the given object.
 * @param obj Object to be cloned.
 * @return Pointer to created object.
 */
rc_var *rc_head::obj_clone(rc_var *var)
{
  sc_voidmapitem *curr;
  ic_object *obj    = var_get(var);
  rc_var *newvar    = obj_create(obj->pClass);
  ic_object *newobj = var_get(newvar);

  // clone basic object
  switch(pCore->class_type(obj->pClass))
  {
    case M_CLASS_BOOL: ((ic_bool *)newobj->mData)->mValue = ((ic_bool *)obj->mData)->mValue; break;
    case M_CLASS_INT: ((ic_int *)newobj->mData)->mValue = ((ic_int *)obj->mData)->mValue; break;
    case M_CLASS_FLOAT: ((ic_float *)newobj->mData)->mValue = ((ic_float *)obj->mData)->mValue; break;
    case M_CLASS_STRING: ((ic_string *)newobj->mData)->set((ic_string *)obj->mData); break;
    case M_CLASS_TIME: ((ic_time *)newobj->mData)->set(((ic_time *)obj->mData)->get()); break;
    case M_CLASS_ARRAY: ((ic_array *)newobj->mData)->copy((ic_array *)obj->mData); break;
    case M_CLASS_REGEX: *((ic_regex *)newobj->mData) = *((ic_regex *)obj->mData); break;
    case M_CLASS_RANGE: *((ic_range *)newobj->mData) = *((ic_range *)obj->mData); break;
    case M_CLASS_MATCH: *((ic_match *)newobj->mData) = *((ic_match *)obj->mData); break;
    case M_CLASS_DIR: *((ic_dir *)newobj->mData) = *((ic_dir *)obj->mData); break;
    case M_CLASS_SOCKET: *((ic_socket *)newobj->mData) = *((ic_socket *)obj->mData); break;
    case M_CLASS_FILE: *((ic_file *)newobj->mData) = *((ic_file *)obj->mData); break;
    case M_CLASS_CLASS: *((rc_class *)newobj->mData) = *((rc_class *)obj->mData); break;
    case M_CLASS_METHOD: *((rc_method *)newobj->mData) = *((rc_method *)obj->mData); break;
    case M_CLASS_EXCEPTION: *((sc_exception *)newobj->mData) = *((sc_exception *)obj->mData); break;
  }

  // clone members
  while(curr = newobj->mMembers->iter_next())
  {
    rc_var *var = (rc_var *)(newobj->mMembers->get(curr->mKey));
    var->pObj   = (void *)obj_clone((rc_var *)var->pObj);
  }

  return newvar;
}

/**
 * Declares one link being removed from object.
 * @param obj Object of the link.
 */
void rc_head::obj_unlink(rc_var *obj)
{
  if(!obj) return;

  if(obj->mLinks <= 1)
  {
    // detect if the var is a link
    if(obj->mProperties & M_PROP_LINK)
    {
      rc_var *sub = (rc_var *)obj->pObj;
      obj_unlink(sub);
    }
    else
    {
      ic_object *sub = (ic_object *)obj->pObj;

      // detect base classes and invoke destructors
      sc_voidarray *bases = pCore->class_parents(sub->pClass);
      // 'object' does not have a constructor for sure
      for(int idx = 0; idx < bases->length() - 1; idx++)
      {
        rc_class *curr = (rc_class *)bases->get(idx);
        method_invoke("#destroy", obj, false);
        delete curr;
      }
      delete bases;

      obj_unlink(sub);
    }

    // printf("dl %p  ", obj);
    delete obj;
  }
  else
    obj->mLinks--;
}

/**
 * Declares one link being removed from object.
 * @param obj Object of the link.
 */
void rc_head::obj_unlink(ic_object *obj)
{
  if(!obj) return;

  if(obj->mLinks <= 1)
  {
    // unlink object properties if there are any
    sc_voidmap *props = obj->mMembers;
    if(props && props->length())
    {
      props->iter_rewind();
      sc_voidmapitem *curr = NULL;
      while(curr = props->iter_next())
      {
        rc_var *pty = (rc_var *)curr->mValue;
        obj_unlink(pty);
      }
    }

    // unlink values if the object is an array
    if(pCore->class_type(obj->pClass) == M_CLASS_ARRAY)
    {
      ic_array *arr = (ic_array *)obj->mData;
      arr->iter_rewind();
      while(sc_voidmapitem *curr = arr->iter_next())
        obj_unlink((rc_var *)curr->mValue);
    }

    // printf("do %p  ", obj);
    delete obj;
  }
  else
  {
    // printf("undo %i  ", obj->mLinks);
    obj->mLinks--;
  }
}

/**
 * Convert an object to boolean representation.
 * Return value should be manually unlinked.
 * @param obj Object to be converted.
 * @return Variable holding an ic_bool object.
 */
rc_var *rc_head::convert_bool(rc_var *var)
{
  var->mLinks++;
  method_invoke("to_b", var, false);
  if(rSRC.mLength)
  {
    rc_var *curr   = rSRC.pop();
    ic_object *obj = var_get(curr);
    if(pCore->class_type(obj->pClass) == M_CLASS_BOOL) return curr;
  }

  exception(M_ERR_BAD_TO_B, M_EXC_ARGS);
  return NULL;
}

/**
 * Convert an object to string representation.
 * Return value should be manually unlinked.
 * @param obj Object to be converted.
 * @return Variable holding an ic_string object.
 */
rc_var *rc_head::convert_string(rc_var *var)
{
  var->mLinks++;
  method_invoke("to_s", var, false);
  if(rSRC.mLength)
  {
    rc_var *curr   = rSRC.pop();
    ic_object *obj = var_get(curr);
    char ct        = pCore->class_type(obj->pClass);
    if(ct == M_CLASS_STRING) return curr;
  }

  exception(M_ERR_BAD_TO_S, M_EXC_ARGS);
  return NULL;
}

/**
 * Convert an object to integer representation.
 * Return value should be manually unlinked.
 * @param obj Object to be converted.
 * @return Variable holding an ic_int object.
 */
rc_var *rc_head::convert_int(rc_var *var)
{
  var->mLinks++;
  method_invoke("to_i", var, false);
  if(rSRC.mLength)
  {
    rc_var *curr   = rSRC.pop();
    ic_object *obj = var_get(curr);
    if(pCore->class_type(obj->pClass) == M_CLASS_INT) return curr;
  }

  exception(M_ERR_BAD_TO_I, M_EXC_ARGS);
  return NULL;
}

/**
 * Creates an object of an ic_basic class.
 * @param type Object type.
 * @return Object.
 */
inline void *rc_head::new_basic(char type)
{
  switch(type)
  {
    case M_CLASS_BOOL: return new ic_bool();
    case M_CLASS_INT: return new ic_int();
    case M_CLASS_FLOAT: return new ic_float();
    case M_CLASS_STRING: return new ic_string();
    case M_CLASS_RANGE: return new ic_range();
    case M_CLASS_REGEX: return new ic_regex();
    case M_CLASS_TIME: return new ic_time();
    case M_CLASS_FILE: return new ic_file();
    case M_CLASS_DIR: return new ic_dir();
    case M_CLASS_SOCKET: return new ic_socket();
    case M_CLASS_METHOD: return new rc_method();
    case M_CLASS_CLASS: return new rc_class();
    case M_CLASS_ARRAY: return new ic_array();

    default: return NULL;
  }
}

/**
 * Creates a new undef object.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_undef(bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pUndef, NULL))->taint(tainted));
}

/**
 * Creates a new bool object.
 * @param value Boolean value.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_bool(bool value, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pBool, new ic_bool(value)))->taint(tainted));
}

/**
 * Creates a new bool object.
 * @param value Pointer to ic_bool.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_bool(ic_bool *value, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pBool, value))->taint(tainted));
}

/**
 * Creates a new int object.
 * @param value Integer value.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_int(long value, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pInt, new ic_int(value)))->taint(tainted));
}

/**
 * Creates a new int object.
 * @param value Pointer to ic_int.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_int(ic_int *value, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pInt, value))->taint(tainted));
}

/**
 * Creates a new float object.
 * @param value Floating point value.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_float(double value, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pFloat, new ic_float(value)))->taint(tainted));
}

/**
 * Creates a new float object.
 * @param value Pointer to ic_float.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_float(ic_float *value, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pFloat, value))->taint(tainted));
}

/**
 * Creates a new string object.
 * @param string Char string.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_string(const char *string, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pString, new ic_string(string)))->taint(tainted));
}

/**
 * Creates a new string object.
 * @param string Pointer to ic_string.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_string(ic_string *string, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pString, string))->taint(tainted));
}

/**
 * Creates a new regex object.
 * @param regex Pointer to ic_regex.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_regex(ic_regex *regex, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pRegex, regex))->taint(tainted));
}

/**
 * Creates a new match object.
 * @param regex Pointer to ic_match.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_match(ic_match *match, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pMatch, match))->taint(tainted));
}

/**
 * Creates a new range object.
 * @param start Interval start.
 * @param end Interval end.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_range(long start, long end, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pRange, new ic_range(start, end)))->taint(tainted));
}

/**
 * Creates a new range object.
 * @param range Pointer to ic_range.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_range(ic_range *range, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pRange, range))->taint(tainted));
}

/**
 * Creates a new date object.
 * @param stamp Timestamp.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_time(long stamp, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pTime, new ic_time(stamp)))->taint(tainted));
}

/**
 * Creates a new date object.
 * @param time Pointer to ic_time.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_time(ic_time *time, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pTime, time))->taint(tainted));
}

/**
 * Creates a new method object.
 * @param method Pointer to rc_method.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_method(rc_method *method, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pMethod, method))->taint(tainted));
}

/**
 * Creates a new class object.
 * @param cls Pointer to rc_class.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_class(rc_class *cls, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pClass, cls))->taint(tainted));
}

/**
 * Creates a new empty array.
 * @param tainted Tainted flag.
 * @return Pointer to created object
 */
inline rc_var *rc_head::new_array(bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pArray, new ic_array()))->taint(tainted));
}

/**
 * Creates an array object.
 * @param arr Array.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_array(ic_array *arr, bool tainted)
{
  return new rc_var((new ic_object(pCore->mClassCache.pArray, arr))->taint(tainted));
}

/**
 * Creates an exception object.
 * @param msg Exception message.
 * @param type Exception type.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_exception(const char *msg, int type, bool tainted)
{
  sc_exception *exc = new sc_exception(msg, type, mFile, mLine);
  return new rc_var((new ic_object(pCore->mClassCache.pException, exc))->taint(tainted));
}

/**
 * Creates an exception object.
 * @param msg Exception message.
 * @param type Exception type.
 * @param tainted Tainted flag.
 * @return Pointer to created object.
 */
inline rc_var *rc_head::new_exception(ic_string *msg, int type, bool tainted)
{
  sc_exception *exc = new sc_exception(msg, type, mFile, mLine);
  return new rc_var((new ic_object(pCore->mClassCache.pException, exc))->taint(tainted));
}

/**
 * Loads current variable / constant into AX.
 */
void rc_head::cmd_loadax()
{
  if(rAX) obj_unlink(rAX);

  const char *name = NULL;
  switch(pCmd->mModifier)
  {
    case RASM_MOD_NULL: rAX = NULL; break;
    case RASM_MOD_UNDEF: rAX = new_undef(); break;
    case RASM_MOD_FALSE: rAX = new_bool(false); break;
    case RASM_MOD_TRUE: rAX = new_bool(true); break;
    case RASM_MOD_INT: rAX = new_int(pCmd->mParam.addr); break;
    case RASM_MOD_FLOAT: rAX = new_float(pCmd->mParam.val); break;
    case RASM_MOD_STRING: rAX = new_string(pCore->mStrTable->get(pCmd->mParam.addr)); break;

    case RASM_MOD_VAR:
      rAX = scope_get(pCmd->mParam.addr);
      if(rAX) rAX->mLinks++;
      break;

    case RASM_MOD_PROPERTY:
      rAX = member_resolve(pCore->mStrTable->get(pCmd->mParam.addr), rAX, pTmpClass);
      if(rAX) rAX->mLinks++;
      nsp_flush();
      break;

    case RASM_MOD_CONST:
      rAX = bare_id_resolve(pCore->mStrTable->get(pCmd->mParam.addr));
      if(rAX) rAX->mLinks++;
      nsp_flush();
      break;

    case RASM_MOD_SELF:
      if(pCurrObj)
      {
        rAX = pCurrObj;
        rAX->mLinks++;
      }
      else
        exception(M_ERR_THIS_DISABLED, M_EXC_SCRIPT);
      break;

    default: exception(ic_string::format(M_ERR_UNKNOWN_MOD, pCmd->mModifier, mOffset), M_EXC_INTERNAL);
  }
}

/**
 * Loads current variable / constant into BX.
 */
void rc_head::cmd_loadbx()
{
  cmd_clrsrc();
  rc_var *bx;

  switch(pCmd->mModifier)
  {
    case RASM_MOD_NULL: bx = NULL; break;
    case RASM_MOD_UNDEF: bx = new_undef(); break;
    case RASM_MOD_FALSE: bx = new_bool(false); break;
    case RASM_MOD_TRUE: bx = new_bool(true); break;
    case RASM_MOD_INT: bx = new_int(pCmd->mParam.addr); break;
    case RASM_MOD_FLOAT: bx = new_float(pCmd->mParam.val); break;
    case RASM_MOD_STRING: bx = new_string(pCore->mStrTable->get(pCmd->mParam.addr)); break;

    case RASM_MOD_VAR:
      bx = scope_get(pCmd->mParam.addr);
      if(bx) bx->mLinks++;
      break;

    case RASM_MOD_PROPERTY:
      bx = member_resolve(pCore->mStrTable->get(pCmd->mParam.addr), rAX, pTmpClass);
      if(bx) bx->mLinks++;
      nsp_flush();
      break;

    case RASM_MOD_CONST:
      bx = bare_id_resolve(pCore->mStrTable->get(pCmd->mParam.addr));
      if(bx) bx->mLinks++;
      nsp_flush();
      break;

    case RASM_MOD_SELF:
      if(pCurrObj)
      {
        bx = pCurrObj;
        bx->mLinks++;
      }
      else
        exception(M_ERR_THIS_DISABLED, M_EXC_SCRIPT);
      break;

    default: exception(ic_string::format(M_ERR_UNKNOWN_MOD, pCmd->mModifier, mOffset), M_EXC_INTERNAL);
  }

  rSRC.push(bx);
}

/**
 * Saves the contents of AX into current variable.
 */
inline void rc_head::cmd_saveax()
{
  if(!rAX) exception(ic_string::format(M_ERR_INTERNAL, "SAVEAX with AX=0"), M_EXC_INTERNAL);

  rAX->mLinks++;
  var_save(scope_get(pCmd->mParam.addr), rAX);
}

/**
 * Saves the contents of BX into current variable.
 */
inline void rc_head::cmd_savebx()
{
  if(!rSRC.mLength) exception(ic_string::format(M_ERR_INTERNAL, "SAVEBX with BX=0"), M_EXC_INTERNAL);

  rc_var *res = rSRC.pop();
  res->mLinks++;
  var_save(scope_get(pCmd->mParam.addr), res);
}

/**
 * Process assignment.
 */
void rc_head::cmd_assign()
{
  // detect number of assignments either from a parameter
  // or from the actual length of queues
  int count = MIN(rSRC.mLength, rDST.mLength);
  int max   = pCmd->mParam.addr;
  if(max) count = MIN(count, max);

  // report an imbalanced assignment notice
  if(max == 0 && rSRC.mLength != rDST.mLength) notice(M_WARN_IMBALANCED_PASG);

  while(count-- > 0)
    var_save(rDST.pop(), rSRC.pop());
}

/**
 * Process unsplatted assignment.
 */
void rc_head::cmd_unsplassign()
{
  // detect number of items either from a parameter
  // or from the actual length of queues
  int count = MIN(rSRC.mLength, rDST.mLength);
  int max   = pCmd->mParam.addr;
  if(max) count = MIN(count, max);

  // pack a needed number of objects into an array
  rc_var *var   = new_array();
  ic_array *arr = (ic_array *)(var_get(var)->mData);
  while(count-- > 0)
    arr->append(rSRC.pop(), false);

  var_save(rDST.pop(), var);
}

/**
 * Exchanges AX and BX.
 */
inline void rc_head::cmd_xchg()
{
  rc_var *tmp       = rAX;
  rAX               = rSRC.mFirst->pObj;
  rSRC.mFirst->pObj = tmp;
}

/**
 * Adds BX to AX, saves result in RES.
 */
inline void rc_head::cmd_add()
{
  sub_math_op("#add");
}

/**
 * Subtracts BX from AX, saves result in RES.
 */
inline void rc_head::cmd_sub()
{
  sub_math_op("#sub");
}

/**
 * Multiplies AX by BX, saves result in RES.
 */
inline void rc_head::cmd_mul()
{
  sub_math_op("#mul");
}

/**
 * Divides AX by BX, saves result in RES.
 */
inline void rc_head::cmd_div()
{
  sub_math_op("#div");
}

/**
 * Gets division remainder from AX / BX, saves result in RES.
 */
inline void rc_head::cmd_mod()
{
  sub_math_op("#mod");
}

/**
 * Brings AX to power BX, saves result in RES.
 */
inline void rc_head::cmd_pow()
{
  sub_math_op("#pow");
}

/**
 * Shifts AX BX times left, saves result in RES.
 */
inline void rc_head::cmd_shl()
{
  sub_math_op("#shl");
}

/**
 * Shifts AX BX times right, saves result in RES.
 */
inline void rc_head::cmd_shr()
{
  sub_math_op("#shr");
}

/**
 * Does binary AND on AX and BX, saves result in RES.
 */
inline void rc_head::cmd_band()
{
  sub_math_op("#and");
}

/**
 * Does binary OR on AX and BX, saves result in RES.
 */
inline void rc_head::cmd_bor()
{
  sub_math_op("#or");
}

/**
 * Does binary XOR on AX and BX, saves result in RES.
 */
inline void rc_head::cmd_bxor()
{
  sub_math_op("#xor");
}

/**
 * Does logical AND on AX and BX, saves result in RES.
 */
inline void rc_head::cmd_and()
{
  sub_logical_op('a');
}

/**
 * Does logical OR on AX and BX, saves result in BX.
 */
inline void rc_head::cmd_or()
{
  sub_logical_op('o');
}

/**
 * Does logical XOR on AX and BX, saves result in BX.
 */
inline void rc_head::cmd_xor()
{
  sub_logical_op('x');
}

/**
 * Increments AX by 1.
 */
inline void rc_head::cmd_inc()
{
  if(!rAX) exception(ic_string::format(M_ERR_OP_EMPTY_REGISTER, "inc"), M_EXC_ARGS);

  // object gets modified, but stays in rAX
  method_invoke("#inc", rAX);
}

/**
 * Decrements AX by 1.
 */
inline void rc_head::cmd_dec()
{
  if(!rAX) exception(ic_string::format(M_ERR_OP_EMPTY_REGISTER, "dec"), M_EXC_ARGS);

  // object gets modified, but stays in rAX
  method_invoke("#dec", rAX);
}

/**
 * Inverts AX, saves result in RES.
 */
inline void rc_head::cmd_neg()
{
  method_invoke("to_b", rAX, false);
  if(rSRC.mLength)
  {
    ic_object *obj = var_get(rSRC.get(0));
    if(pCore->class_type(obj->pClass) == M_CLASS_BOOL)
    {
      bool value                      = !((ic_bool *)obj->mData)->mValue;
      ((ic_bool *)obj->mData)->mValue = value;
    }
    else
      exception(M_ERR_BAD_TO_B, M_EXC_ARGS);
  }
}

/**
 * Compare two objects using == operator.
 */
void rc_head::cmd_eq()
{
  // compare
  char type   = sub_compare();
  rc_var *var = rSRC.pop();
  if(type == M_CLASS_UNDEF)
    rSRC.push(new_bool(false, var_get(var)->mTainted));
  else
  {
    // "cmp" returned a 0
    bool res = ((ic_int *)var_get(var)->mData)->mValue == 0;
    rSRC.push(new_bool(res, var_get(var)->mTainted));
  }
  obj_unlink(var);
}

/**
 * Strictly compare two objects using === operator.
 */
void rc_head::cmd_eq_strict()
{
  // compare
  char type   = sub_compare(true);
  rc_var *var = rSRC.pop();
  if(type == M_CLASS_UNDEF)
    rSRC.push(new_bool(false, var_get(var)->mTainted));
  else
  {
    // "cmp" returned a 0
    bool res = ((ic_int *)var_get(var)->mData)->mValue == 0;
    rSRC.push(new_bool(res, var_get(var)->mTainted));
  }
  obj_unlink(var);
}

/**
 * Compare two objects by relation using ~ operator.
 */
void rc_head::cmd_rel()
{
  if(!rAX || !rSRC.mLength) exception(ic_string::format(M_ERR_OP_EMPTY_REGISTER, "#rel"), M_EXC_ARGS);

  ic_object *left = var_get(rAX), *right = var_get(rSRC.get(0));
  bool taint = left->mTainted || right->mTainted;

  rc_method *op = pCore->op_resolve("#rel", left->pClass, right->pClass);
  if(op)
  {
    // relativity method does exist
    method_invoke(op, rAX);

    // no return value means an undef
    if(!rSRC.mLength)
      rSRC.push(new_undef(taint));
    else
    {
      ic_object *obj = var_get(rSRC.get(0));
      obj->mTainted  = taint;
    }
  }
  else if(pCore->class_is_heir(left->pClass, right->pClass) || pCore->class_is_heir(right->pClass, left->pClass))
  {
    // fall back to primitive comparison, if the classes are identical
    op = pCore->op_resolve("#cmp", left->pClass, right->pClass);
    if(op)
    {
      method_invoke(op, rAX);

      // convert -1..0..1 to true / false
      if(rSRC.mLength)
      {
        ic_object *tmp = rSRC.get(0)->get();
        if(pCore->class_type(tmp->pClass) == M_CLASS_INT)
          ((ic_int *)tmp->mData)->mValue = (int)(((ic_int *)tmp->mData)->mValue == 0);

        tmp->taint(taint);
      }
      else
        rSRC.push(new_undef(taint));
    }
  }
}

/**
 * Compare two objects using < operator.
 */
void rc_head::cmd_less()
{
  // compare
  char type   = sub_compare(true);
  rc_var *var = rSRC.pop();
  if(type == M_CLASS_UNDEF)
    rSRC.push(new_bool(false, var_get(var)->mTainted));
  else
  {
    // "cmp" returned a 0
    bool res = ((ic_int *)var_get(var)->mData)->mValue < 0;
    rSRC.push(new_bool(res, var_get(var)->mTainted));
  }
  obj_unlink(var);
}

/**
 * Compare two objects using <= operator.
 */
void rc_head::cmd_less_eq()
{
  // compare
  char type   = sub_compare(true);
  rc_var *var = rSRC.pop();
  if(type == M_CLASS_UNDEF)
    rSRC.push(new_bool(false, var_get(var)->mTainted));
  else
  {
    // "cmp" returned a 0
    bool res = ((ic_int *)var_get(var)->mData)->mValue <= 0;
    rSRC.push(new_bool(res, var_get(var)->mTainted));
  }
  obj_unlink(var);
}

/**
 * Compare two objects using > operator.
 */
void rc_head::cmd_greater()
{
  // compare
  char type   = sub_compare(true);
  rc_var *var = rSRC.pop();
  if(type == M_CLASS_UNDEF)
    rSRC.push(new_bool(false, var_get(var)->mTainted));
  else
  {
    // "cmp" returned a 0
    bool res = ((ic_int *)var_get(var)->mData)->mValue > 0;
    rSRC.push(new_bool(res, var_get(var)->mTainted));
  }
  obj_unlink(var);
}

/**
 * Compare two objects using >= operator.
 */
void rc_head::cmd_greater_eq()
{
  // compare
  char type   = sub_compare(true);
  rc_var *var = rSRC.pop();
  if(type == M_CLASS_UNDEF)
    rSRC.push(new_bool(false, var_get(var)->mTainted));
  else
  {
    // "cmp" returned a 0
    bool res = ((ic_int *)var_get(var)->mData)->mValue >= 0;
    rSRC.push(new_bool(res, var_get(var)->mTainted));
  }
  obj_unlink(var);
}

/**
 * Compare two objects using <=> operator.
 */
inline void rc_head::cmd_cmp()
{
  sub_compare();
}

/**
 * Jump to address if AX is non-empty.
 */
inline void rc_head::cmd_jtrue()
{
  if(!rSRC.mLength) exception(ic_string::format(M_ERR_INTERNAL, "JTRUE with BX=0"), M_EXC_INTERNAL);

  rc_var *var = rSRC.pop();
  if(sub_value(var))
    cmd_jmp();
  else
    mOffset++;
  obj_unlink(var);
}

/**
 * Jump to address if AX is empty.
 */
inline void rc_head::cmd_jfalse()
{
  if(!rSRC.mLength) exception(ic_string::format(M_ERR_INTERNAL, "JFALSE with BX=0"), M_EXC_INTERNAL);

  rc_var *var = rSRC.pop();
  if(!sub_value(var))
    cmd_jmp();
  else
    mOffset++;
  obj_unlink(var);
}

/**
 * Jumps to address.
 */
inline void rc_head::cmd_jmp()
{
  mOffset = pCmd->mParam.addr;
}

/**
 * Pushes AX into US.
 */
inline void rc_head::cmd_pushus()
{
  rAX->mLinks++;
  rUS.push(rAX);
}

/**
 * Pops object(s) from US.
 */
inline void rc_head::cmd_popus()
{
  if(pCmd->mParam.addr == 0)
  {
    // single object
    obj_unlink(rAX);
    rAX = rUS.pop();
  }
  else
  {
    // a list of objects
    for(int idx = MIN(pCmd->mParam.addr, rUS.mLength); idx > 0; idx--)
      obj_unlink(rUS.pop());
  }
}

/**
 * Pushes AX into SRC.
 */
inline void rc_head::cmd_pushsrc()
{
  if(!rAX)
    exception(ic_string::format(M_ERR_INTERNAL, "PUSHSRC with AX=0"), M_EXC_ARGS);
  else
    rSRC.push(new rc_var(rAX));
}

/**
 * Pops AX from SRC.
 */
inline void rc_head::cmd_popsrc()
{
  obj_unlink(rAX);
  rAX = rSRC.pop();
}

/**
 * Pushes variable into DST.
 */
inline void rc_head::cmd_pushdst()
{
  if(!rAX)
    exception(ic_string::format(M_ERR_INTERNAL, "PUSHDST with AX=0"), M_EXC_ARGS);
  else
  {
    rAX->mLinks++;
    rDST.push(rAX);
  }
}

/**
 * Pushes variable into DST.
 */
inline void rc_head::cmd_popdst()
{
  if(rDST.mLength) obj_unlink(rDST.pop());
}

/**
 * Splats AX into SRC.
 */
inline void rc_head::cmd_splat()
{
  if(!rAX)
    exception(ic_string::format(M_ERR_OP_EMPTY_REGISTER, "#dec"), M_EXC_ARGS);
  else
    method_invoke("#splat", rAX);
}

/**
 * Unsplats SRC into AX.
 */
inline void rc_head::cmd_unsplat()
{
  rc_var *var   = new_array();
  ic_array *arr = (ic_array *)(var_get(var)->mData);
  while(rSRC.mLength > 0)
  {
    rc_var *curr = rSRC.pop();
    curr->mLinks++;
    arr->append(curr, false);
  }

  obj_unlink(rAX);
  rAX = var;
}

/**
 * Moves N elements from SRC to DST.
 * @todo implement
 */
void rc_head::cmd_movesrc()
{
  long count = MIN(rSRC.mLength, pCmd->mParam.addr);
  while(count-- > 0)
    rDST.push(rSRC.pop());
}

/**
 * Moves N elements from DST to SRC.
 * @todo implement
 */
void rc_head::cmd_movedst()
{
  long count = MIN(rDST.mLength, pCmd->mParam.addr);
  while(count-- > 0)
    rSRC.push(rDST.pop());
}

/**
 * Clears SRC.
 */
inline void rc_head::cmd_clrsrc()
{
  rc_var *curr;
  while(curr = rSRC.pop())
    obj_unlink(curr);
}

/**
 * Clears DST.
 */
inline void rc_head::cmd_clrdst()
{
  rc_var *curr;
  while(curr = rDST.pop())
    obj_unlink(curr);
}

/**
 * Creates new object of class BX into AX.
 * @todo error checking & reporting
 */
inline void rc_head::cmd_new()
{
  if(rAX) obj_unlink(rAX);

  char *classname = pCore->mStrTable->get(pCmd->mParam.addr);
  rAX             = obj_create(pCore->class_resolve(classname, pTmpClass));
  nsp_flush();
}

/**
 * Calls a method on AX.
 */
inline void rc_head::cmd_call()
{
  const char *name = pCore->mStrTable->get(pCmd->mParam.addr);

  rc_method *method;

  rc_var *curr = rAX ? rAX : pCurrObj;
  method_invoke(name, curr);

  // remove pTmpClass if any
  nsp_flush();
}

/**
 * Returns from a method.
 */
inline void rc_head::cmd_return()
{
  if(!mExternalScope)
  {
    // collect garbage
    for(long idx = 0; idx < mVars->length(); idx++)
      obj_unlink((rc_var *)mVars->get(idx));

    delete mVars;
  }

  state_load();
}

/**
 * Selects a namespace for current operation.
 */
inline void rc_head::cmd_nsp()
{
  nsp_select(pCore->mStrTable->get(pCmd->mParam.addr));
}

/**
 * Binds the lambda to current scope.
 */
inline void rc_head::cmd_bindlambda()
{
  ic_object *lambda = var_get(rAX);
  if(pCore->class_type(lambda->pClass) == M_CLASS_METHOD)
  {
    rc_method *method      = (rc_method *)lambda->mData;
    method->pExternalScope = mVars;
  }
  else
    exception(M_ERR_BAD_BIND, M_EXC_SCRIPT);
}

/**
 * Indexes array in AX with SRC.
 * Puts resulting array of objects back into SRC.
 */
inline void rc_head::cmd_index()
{
  if(!rAX) exception(ic_string::format(M_ERR_OP_EMPTY_REGISTER, "idx"), M_EXC_ARGS);

  method_invoke("#idx", rAX);

  if(!rSRC.mLength) rSRC.push(new_undef());
}

/**
 * Includes file named by AX.
 * @todo Implement when the parser is ready.
 */
void rc_head::cmd_include()
{
}

/**
 * Includes file named by AX or dies.
 * @todo Implement when the parser is ready.
 */
void rc_head::cmd_require()
{
}

/**
 * Verbosely collects garbage at address.
 * @todo Implement one day.
 */
void rc_head::cmd_gc()
{
  // not needed currently
}

/**
 * Sets RVM property (for later usage).
 */
void rc_head::cmd_setpty()
{
  // not needed currently
}

/**
 * Sets current file (for debug output).
 */
inline void rc_head::cmd_setfile()
{
  strncpy(mFile, pCore->mStrTable->get(pCmd->mParam.addr), 254);
  mStatFiles++;
}

/**
 * Sets current line (for debug output).
 */
inline void rc_head::cmd_setline()
{
  mLine = pCmd->mParam.addr;
  mStatLines++;
}

/**
 * Throws AX as an exception object.
 * AX gets transfered to IN. It needs to be popped
 * at the entrance of the safe zone to an appropriate variable.
 */
void rc_head::cmd_throw()
{
  // convert to sc_exception if string found
  char type = var_get(rAX)->class_id();
  if(type != M_CLASS_EXCEPTION)
  {
    // create a textual representation
    rc_var *tmp;
    cmd_clrsrc();
    method_invoke("to_s", rAX, false);
    if(rSRC.mLength)
    {
      rc_var *str    = rSRC.pop();
      ic_string *msg = (ic_string *)var_get(str)->mData;
      var_get(str)->mLinks++;
      obj_unlink(str);
      tmp = new_exception(msg, M_EXC_USER, var_get(rAX)->mTainted);
    }
    else
      tmp = new_exception("", M_EXC_USER, var_get(rAX)->mTainted);

    obj_unlink(rAX);
    rAX = tmp;
  }

  if(rSS.mLength)
  {
    cmd_clrsrc();
    cmd_pushsrc();

    rCS.add(rSS.pop());
    state_load();
  }
  else
  {
    sc_exception *exc = (sc_exception *)var_get(rAX)->mData;
    pCore->error_any(ic_string::format(M_ERR_EXCEPTION, exc->mErrorMsg->get(), exc->mFile, exc->mLine), M_EMODE_EXCEPT);
  }
}

/**
 * Declares entering a safe block, jumping to address in case of exception.
 */
inline void rc_head::cmd_try()
{
  state_save();
  rc_headstate *state = (rc_headstate *)rCS.pop();
  state->mOffset      = pCmd->mParam.addr;
  rSS.add((void *)state);
}

/**
 * Declares exit of safe block.
 */
inline void rc_head::cmd_tried()
{
  rSS.pop();
}

/**
 * Registers a class.
 * @todo implement
 */
void rc_head::cmd_regclass()
{
}

/**
 * Registers a property.
 * @todo implement
 */
void rc_head::cmd_regproperty()
{
}

/**
 * Registers a method.
 * @todo implement
 */
void rc_head::cmd_regmethod()
{
}

/**
 * Show debug info about RVM state.
 */
#define INSPECT_SRC 1
#define INSPECT_DST 2
#define INSPECT_AX 3
#define INSPECT_BX 4
#define INSPECT_US 5
#define INSPECT_CS 6
#define INSPECT_SS 7

void rc_head::cmd_inspect()
{
  int count   = 0;
  rc_var *obj = NULL;
  switch(pCmd->mParam.addr)
  {
    case INSPECT_AX: obj_inspect(rAX); break;
    case INSPECT_BX: obj_inspect(rSRC.get(0)); break;

    case INSPECT_SRC:
      printf("{src:\n");
      rSRC.iter_rewind();
      while(obj = rSRC.iter_next())
        obj_inspect(obj);
      printf("}\n");
      break;

    case INSPECT_US:
      printf("{us(10):\n");
      rUS.iter_rewind();
      while((obj = rUS.iter_next()) != NULL && count < 10)
      {
        obj_inspect(obj);
        count++;
      }
      printf("}\n");
      break;

    case INSPECT_CS:
      printf("{cs(10):\n");
      // todo!
      printf("}\n");
      break;
  }
}

#undef INSPECT_SRC
#undef INSPECT_AX
#undef INSPECT_BX
#undef INSPECT_US
#undef INSPECT_CS
#undef INSPECT_SS

#endif

/**
 * Invoke a two-arguments casual operator by name.
 * @param name Operator name.
 */
inline void rc_head::sub_math_op(const char *name)
{
  if(!rAX || !rSRC.mLength) exception(ic_string::format(M_ERR_OP_EMPTY_REGISTER, name), M_EXC_ARGS);

  ic_object *left = var_get(rAX), *right = var_get(rSRC.get(0));
  bool taint = left->mTainted || right->mTainted;

  rc_method *op = pCore->op_resolve(name, left->pClass, right->pClass);
  if(op) method_invoke(op, rAX);

  if(!rSRC.mLength) rSRC.push(new_undef());

  if(taint) var_get(rSRC.get(0))->mTainted = true;
}

/**
 * Invoke a two-arguments logical short-circuit operator by name.
 * @param name Operator name as a character: 'a' = and, 'o' = 'or', 'x' = 'xor'
 */
inline void rc_head::sub_logical_op(char name)
{
  if(!rAX || !rSRC.mLength) exception(ic_string::format(M_ERR_OP_EMPTY_REGISTER, "to_b"), M_EXC_ARGS);

  bool taint = var_get(rAX)->mTainted || var_get(rSRC.get(0))->mTainted;
  bool left = false, right = false, result = false;

  rc_var *tmp = rSRC.pop();

  // convert left operand to boolean
  rc_var *leftvar = convert_bool(rAX);
  if(leftvar) left = ((ic_bool *)var_get(leftvar)->mData)->mValue;

  // convert left operand to boolean
  rc_var *rightvar = convert_bool(tmp);
  if(rightvar) right = ((ic_bool *)var_get(rightvar)->mData)->mValue;

  switch(name)
  {
    case 'a': result = left && right; break;
    case 'o': result = left || right; break;
    case 'x': result = left != right; break;
  }

  rSRC.push(new_bool(result, taint));
}

/**
 * Invokes the 'cmp' operator and ensures it has returned proper values.
 * @param strict Strictly compare objects (they should be identical, not just
 * equal)
 * @return object type - M_CLASS_INT or M_CLASS_UNDEF
 */
char rc_head::sub_compare(bool strict)
{
  if(!rAX || !rSRC.mLength) exception(ic_string::format(M_ERR_OP_EMPTY_REGISTER, "#cmp"), M_EXC_ARGS);

  ic_object *left = var_get(rAX), *right = var_get(rSRC.get(0));
  bool taint = left->mTainted || right->mTainted;

  if(strict && left->pClass != right->pClass)
  {
    obj_unlink(rSRC.pop());
    rSRC.push(new_int(1, taint));
  }
  else
  {
    if(left == right)
    {
      obj_unlink(rSRC.pop());
      rSRC.push(new_int(0L, taint));
    }
    else
    {
      rc_method *op = pCore->op_resolve("#cmp", left->pClass, right->pClass);
      if(op) method_invoke(op, rAX);

      // no return value means an undef
      if(!rSRC.mLength) rSRC.push(new_undef());
    }
  }

  ic_object *res = var_get(rSRC.get(0));
  res->mTainted  = taint;

  // return class type or WTF
  char cls = pCore->class_type(res->pClass);
  if(rSRC.mLength == 1 && (cls == M_CLASS_INT || cls == M_CLASS_UNDEF))
    return cls;
  else
  {
    exception(M_ERR_BAD_CMP_RETURN, M_EXC_ARGS);
    return 0;
  }
}

/**
 * Compare two objects for internal purpose.
 * @param left Left object
 * @param right Right object
 * @param strict Flag indicating the comparison is strict
 * @return -2 = unknown, -1 = less, 0 = equal, 1 = greater
 */
inline char rc_head::sub_compare(rc_var *left, rc_var *right, bool strict)
{
  rc_var *tmp = rAX;
  left->mLinks++;
  right->mLinks++;

  rAX = left;
  cmd_clrsrc();
  rSRC.push(right);
  char res = sub_compare(strict);
  if(res == M_CLASS_UNDEF)
    res = -2;
  else
    res = (char)((ic_int *)rSRC.get(0)->get()->mData)->mValue;

  obj_unlink(rSRC.pop());
  obj_unlink(rAX);
  rAX = tmp;

  return res;
}

/**
 * Checks if the variable is an empty value.
 * All built-in and built-in-inherited classes have a default check routine.
 * User classes need to define a 'to_b' method.
 * @param var Variable to be inspected.
 * @return Flag indicating the value is non-empty.
 * @todo implement M_CLASS_DIR, M_CLASS_FILE
 */
bool rc_head::sub_value(rc_var *var)
{
  if(!var) return false;

  rc_var *tmp = NULL;

  ic_object *obj = var_get(var);
  char cls       = pCore->class_type(obj->pClass);
  switch(cls)
  {
    // always false
    case M_CLASS_UNDEF:
      return false;

    // always true
    case M_CLASS_CLASS:
    case M_CLASS_METHOD:
    case M_CLASS_EXCEPTION:
      return true;

    // depends
    case M_CLASS_BOOL: return (((ic_bool *)obj->mData)->mValue);
    case M_CLASS_INT: return (((ic_int *)obj->mData)->mValue != 0);
    case M_CLASS_FLOAT: return (((ic_float *)obj->mData)->mValue != 0);
    case M_CLASS_STRING: return (((ic_string *)obj->mData)->mLength != 0);
    case M_CLASS_RANGE: return (((ic_range *)obj->mData)->length() != 0);
    case M_CLASS_REGEX: return (((ic_regex *)obj->mData)->to_b());
    case M_CLASS_ARRAY: return (((ic_array *)obj->mData)->length() != 0);
    case M_CLASS_FILE: return (((ic_file *)obj->mData)->exists());
    case M_CLASS_TIME: return (((ic_time *)obj->mData)->to_b());
    case M_CLASS_MATCH:
      return (((ic_match *)obj->mData)->count() > 0);

    // todo
    case M_CLASS_DIR: return true;
    case M_CLASS_SOCKET:
      return true;

    // user classes
    case M_CLASS_OTHER:
      tmp = convert_bool(var);
      obj = var_get(tmp);
      if(obj)
      {
        obj_unlink(tmp);
        return (((ic_bool *)obj->mData)->mValue);
      }
      else
      {
        obj_unlink(tmp);
        return false;
      }

    default: return false;
  }
}