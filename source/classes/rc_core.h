/**
 * @file rc_core.h
 * @author impworks.
 * rc_core header.
 * Defines properties and methods of rc_core class.
 */

#ifndef RC_CORE_H
#define RC_CORE_H

#include <memory>

/**
 * rc_core constructor.
 */
rc_core::rc_core()
{
  mSetup    = NULL;
  mParser   = NULL;
  mCompiler = NULL;

  mFile = NULL;

  mSource = NULL;
  mHead   = NULL;
  mTape   = NULL;

  mPlugins = NULL;

  mStrTable = NULL;

  pClassRoot = NULL;
}

/**
 * rc_core constructor.
 * @todo fix bug with deleting mHead crashing the whole app
 */
rc_core::~rc_core()
{
  delete mSetup;
  delete mParser;
  delete mCompiler;

  delete mSource;
  delete mHead;
  delete mTape;

  delete mStrTable;

  delete mPlugins;
}

/**
 * Initializes rc_core class.
 */
void rc_core::init()
{
  setlocale(LC_ALL, "C");

  mState     = M_STATE_INIT;
  mErrorMode = M_EMODE_DEFAULT;

  try
  {
    mSetup = new sc_ini("malco.ini");
  }
  catch(const sc_exception &ex)
  {
    if(!ex.mErrorMsg->compare(M_ERR_IO_NO_FILE))
    {
      ERROR(M_ERR_NO_SETUP, M_EMODE_INIT);
    }
    else
      throw;
  }

  mFile   = new ic_string();
  mSource = new ic_string();
  mTape   = new rc_tape();
  mHead   = new rc_head(this);

  mStrTable = new rc_strtable();

  mPlugins = new sc_voidmap();

  sc_random::seed(time(NULL));
}

/**
 * Processes specified Malco task.
 * @param argc Count of arguments.
 * @param argv Array of arguments.
 * @return Script return value.
 */
int rc_core::process(int argc, char *argv[])
{

  mState = M_STATE_BOOT;
  if(argc > 1)
  {
    mTask = cmdline_task(argv[1]);
    if(argc == 2)
    {
      switch(mTask)
      {
        case M_TASK_VERSION: return task_version();
        case M_TASK_CREDITS: return task_credits();
      }
    }
    else if(argc == 3)
    {
      switch(mTask)
      {
        case M_TASK_COMPILE: return task_compile(argv[2]);
        case M_TASK_RUN: return task_run(argv[2]);
        case M_TASK_RUN_BC: return task_run_bc(argv[2]);
        case M_TASK_EVAL: return task_eval(argv[2]);
      }
    }
  }

  error(M_ERR_BAD_COMMANDLINE);
  return 0;
}

/**
 * Builds class hierarchy and registers all environment values.
 */
void rc_core::equip()
{
  // object
  mClassCache.pObject = class_create("object", NULL, M_PROP_STUB);
  method_add("taint!", mClassCache.pObject, object_taint_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("untaint!", mClassCache.pObject, object_untaint_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("tainted", mClassCache.pObject, object_tainted, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("freeze!", mClassCache.pObject, object_freeze_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("frozen", mClassCache.pObject, object_frozen, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("id", mClassCache.pObject, object_id, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("exec", mClassCache.pObject, object_exec, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "name");
  method_add("assert", mClassCache.pObject, object_assert, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, true, "names");
  method_add("class", mClassCache.pObject, object_class, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("class_name", mClassCache.pObject, object_class_name, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("has_method", mClassCache.pObject, object_has_method, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 1, false, "name");
  method_add("has_member", mClassCache.pObject, object_has_member, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 1, false, "name");
  method_add("inspect", mClassCache.pObject, object_inspect, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_a", mClassCache.pObject, object_to_a, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_b", mClassCache.pObject, object_to_b, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_f", mClassCache.pObject, object_to_s, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_i", mClassCache.pObject, object_to_i, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_s", mClassCache.pObject, object_to_s, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);

  method_add("print", mClassCache.pObject, object_print, M_PROP_PUBLIC | M_PROP_FINAL | M_PROP_STATIC)
      ->setup(1, 1, true, "values");

  // undef
  mClassCache.pUndef = class_create("undef", NULL, M_PROP_STUB);
  method_add("#add_object", mClassCache.pUndef, undef_op_any, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#sub_object", mClassCache.pUndef, undef_op_any, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#mul_object", mClassCache.pUndef, undef_op_any, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#div_object", mClassCache.pUndef, undef_op_any, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#mod_object", mClassCache.pUndef, undef_op_any, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#pow_object", mClassCache.pUndef, undef_op_any, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#shl_object", mClassCache.pUndef, undef_op_any, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#shr_object", mClassCache.pUndef, undef_op_any, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#band_object", mClassCache.pUndef, undef_op_any, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#bor_object", mClassCache.pUndef, undef_op_any, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#bxor_object", mClassCache.pUndef, undef_op_any, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_object", mClassCache.pUndef, undef_op_cmp_any, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#rel_object", mClassCache.pUndef, undef_op_rel_any, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#inc", mClassCache.pUndef, undef_op_any, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("#dec", mClassCache.pUndef, undef_op_any, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("#index", mClassCache.pUndef, undef_op_index, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, true, "values");
  method_add("inspect", mClassCache.pUndef, undef_inspect, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);

  // bool
  mClassCache.pBool = class_create("bool");
  method_add("#create", mClassCache.pBool, bool_op_create, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#band_bool", mClassCache.pBool, bool_op_band_bool, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#bor_bool", mClassCache.pBool, bool_op_bor_bool, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#bxor_bool", mClassCache.pBool, bool_op_bxor_bool, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_bool", mClassCache.pBool, bool_op_cmp_bool, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_object", mClassCache.pBool, bool_op_cmp_object, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#rel_object", mClassCache.pBool, bool_op_cmp_object, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("toggle!", mClassCache.pBool, bool_toggle_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("inspect", mClassCache.pBool, bool_inspect, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_b", mClassCache.pBool, bool_to_b, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_f", mClassCache.pBool, bool_to_f, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_i", mClassCache.pBool, bool_to_i, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_s", mClassCache.pBool, bool_to_s, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);

  // int
  mClassCache.pInt = class_create("int");
  method_add("#create", mClassCache.pInt, int_op_create, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#add_int", mClassCache.pInt, int_op_add_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#add_float", mClassCache.pInt, int_op_add_float, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#sub_int", mClassCache.pInt, int_op_sub_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#sub_float", mClassCache.pInt, int_op_sub_float, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#mul_int", mClassCache.pInt, int_op_mul_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#mul_float", mClassCache.pInt, int_op_mul_float, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#div_int", mClassCache.pInt, int_op_div_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#div_float", mClassCache.pInt, int_op_div_float, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#pow_int", mClassCache.pInt, int_op_pow_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#pow_float", mClassCache.pInt, int_op_pow_float, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#mod_int", mClassCache.pInt, int_op_mod_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#shl_int", mClassCache.pInt, int_op_shl_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#shr_int", mClassCache.pInt, int_op_shr_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#band_int", mClassCache.pInt, int_op_band_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#bor_int", mClassCache.pInt, int_op_bor_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#bxor_int", mClassCache.pInt, int_op_bxor_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#inc", mClassCache.pInt, int_op_inc, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#dec", mClassCache.pInt, int_op_dec, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_int", mClassCache.pInt, int_op_cmp_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_bool", mClassCache.pInt, int_op_cmp_bool, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_float", mClassCache.pInt, int_op_cmp_float, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_string", mClassCache.pInt, int_op_cmp_string, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#rel_range", mClassCache.pInt, int_op_rel_range, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("inspect", mClassCache.pInt, int_inspect, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("times", mClassCache.pInt, int_times, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "fx");
  method_add("to", mClassCache.pInt, int_to, M_PROP_PUBLIC | M_PROP_FINAL)->setup(2, 2, false, "num", "fx");
  method_add("upto", mClassCache.pInt, int_upto, M_PROP_PUBLIC | M_PROP_FINAL)->setup(2, 2, false, "num", "fx");
  method_add("downto", mClassCache.pInt, int_downto, M_PROP_PUBLIC | M_PROP_FINAL)->setup(2, 2, false, "num", "fx");
  method_add("char", mClassCache.pInt, int_char, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_b", mClassCache.pInt, int_to_b, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_i", mClassCache.pInt, int_to_i, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_f", mClassCache.pInt, int_to_f, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_s", mClassCache.pInt, int_to_s, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);

  // float
  mClassCache.pFloat = class_create("float");
  method_add("#create", mClassCache.pFloat, float_op_create, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#add_int", mClassCache.pFloat, float_op_add_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#add_float", mClassCache.pFloat, float_op_add_float, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#sub_int", mClassCache.pFloat, float_op_sub_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#sub_float", mClassCache.pFloat, float_op_sub_float, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#mul_int", mClassCache.pFloat, float_op_mul_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#mul_float", mClassCache.pFloat, float_op_mul_float, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#div_int", mClassCache.pFloat, float_op_div_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#div_float", mClassCache.pFloat, float_op_div_float, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#pow_int", mClassCache.pFloat, float_op_pow_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#pow_float", mClassCache.pFloat, float_op_pow_float, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#inc", mClassCache.pFloat, float_op_inc, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#dec", mClassCache.pFloat, float_op_dec, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_int", mClassCache.pFloat, float_op_cmp_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_float", mClassCache.pFloat, float_op_cmp_float, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_string", mClassCache.pFloat, float_op_cmp_string, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_bool", mClassCache.pFloat, float_op_cmp_bool, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#rel_range", mClassCache.pFloat, float_op_rel_range, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("inspect", mClassCache.pFloat, float_inspect, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("ceil", mClassCache.pFloat, float_ceil, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("floor", mClassCache.pFloat, float_floor, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("decimal", mClassCache.pFloat, float_decimal, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_b", mClassCache.pFloat, float_to_b, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_f", mClassCache.pFloat, float_to_f, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_i", mClassCache.pFloat, float_to_i, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_s", mClassCache.pFloat, float_to_s, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);

  // string
  mClassCache.pString = class_create("string");
  method_add("#create", mClassCache.pString, string_op_create, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#add_string", mClassCache.pString, string_op_add_string, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#sub_string", mClassCache.pString, string_op_sub_string, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#mul_int", mClassCache.pString, string_op_mul_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#mod_array", mClassCache.pString, string_op_mod_array, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#shl_bool", mClassCache.pString, string_op_shl_bool, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#shl_int", mClassCache.pString, string_op_shl_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#shl_float", mClassCache.pString, string_op_shl_float, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#shl_string", mClassCache.pString, string_op_shl_string, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#shl_range", mClassCache.pString, string_op_shl_range, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#shl_object", mClassCache.pString, string_op_shl_object, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#idx", mClassCache.pString, string_op_idx, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_bool", mClassCache.pString, string_op_cmp_bool, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_int", mClassCache.pString, string_op_cmp_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_float", mClassCache.pString, string_op_cmp_float, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_string", mClassCache.pString, string_op_cmp_string, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#rel_regex", mClassCache.pString, string_op_rel_regex, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("inspect", mClassCache.pString, string_inspect, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("length", mClassCache.pString, string_length, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("trim", mClassCache.pString, string_trim, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("trim_left", mClassCache.pString, string_trim_left, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("trim_right", mClassCache.pString, string_trim_right, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("trim!", mClassCache.pString, string_trim_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("trim_left!", mClassCache.pString, string_trim_left_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("trim_right!", mClassCache.pString, string_trim_right_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("ord", mClassCache.pString, string_ord, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("reverse", mClassCache.pString, string_reverse, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("reverse!", mClassCache.pString, string_reverse_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("substr", mClassCache.pString, string_sub, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 2, false, "from", "to");
  method_add("substr_first", mClassCache.pString, string_sub_first, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 2, false, "str", "offset");
  method_add("substr_last", mClassCache.pString, string_sub_last, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 2, false, "str", "offset");
  method_add("count", mClassCache.pString, string_sub_count, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "str");
  method_add("append", mClassCache.pString, string_append, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "str");
  method_add("prepend", mClassCache.pString, string_prepend, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "str");
  method_add("replace", mClassCache.pString, string_replace, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(2, 3, false, "from", "to", "max");
  method_add("append!", mClassCache.pString, string_append_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "str");
  method_add("prepend!", mClassCache.pString, string_prepend_do, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 1, false, "str");
  method_add("replace!", mClassCache.pString, string_replace_do, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(2, 3, false, "from", "to", "max");
  method_add("split", mClassCache.pString, string_split, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 2, false, "by", "max");
  method_add("apply", mClassCache.pString, string_apply, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 2, false, "range", "fx");
  method_add("apply!", mClassCache.pString, string_apply_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "fx");
  method_add("insert", mClassCache.pString, string_insert, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(2, 2, false, "str", "offset");
  method_add("insert!", mClassCache.pString, string_insert_do, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(2, 2, false, "str", "offset");
  method_add("has", mClassCache.pString, string_has, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "str");
  method_add("case_up", mClassCache.pString, string_case_up, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("case_down", mClassCache.pString, string_case_down, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("case_swap", mClassCache.pString, string_case_swap, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("case_up!", mClassCache.pString, string_case_up_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("case_down!", mClassCache.pString, string_case_down_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("case_swap!", mClassCache.pString, string_case_swap_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("chars", mClassCache.pString, string_chars, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("lines", mClassCache.pString, string_lines, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("translate", mClassCache.pString, string_translate, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(2, 2, false, "from", "to");
  method_add("translate!", mClassCache.pString, string_translate_do, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(2, 2, false, "from", "to");
  method_add("to_b", mClassCache.pString, string_to_b, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_f", mClassCache.pString, string_to_f, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_i", mClassCache.pString, string_to_i, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_s", mClassCache.pString, string_to_s, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);

  // range
  mClassCache.pRange = class_create("range");
  method_add("#create", mClassCache.pRange, range_op_create, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 2, false, "start", "end");
  method_add("#cmp_bool", mClassCache.pRange, range_op_cmp_bool, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_range", mClassCache.pRange, range_op_cmp_range, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_string", mClassCache.pRange, range_op_cmp_string, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#rel_int", mClassCache.pRange, range_op_rel_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#rel_float", mClassCache.pRange, range_op_rel_float, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("inspect", mClassCache.pRange, range_inspect, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("set!", mClassCache.pRange, range_set_do, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 2, false, "start", "end");
  method_add("has", mClassCache.pRange, range_has, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "num");
  method_add("each", mClassCache.pRange, range_each, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "fx");
  method_add("length", mClassCache.pRange, range_length, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("next", mClassCache.pRange, range_next, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("rewind", mClassCache.pRange, range_next, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_a", mClassCache.pRange, range_to_a, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_b", mClassCache.pRange, range_to_b, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_f", mClassCache.pRange, range_to_f, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_i", mClassCache.pRange, range_to_i, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_s", mClassCache.pRange, range_to_s, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);

  // regex
  mClassCache.pRegex = class_create("regex");
  method_add("#create", mClassCache.pRegex, regex_op_create, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_bool", mClassCache.pRegex, regex_op_cmp_bool, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_regex", mClassCache.pRegex, regex_op_cmp_regex, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_string", mClassCache.pRegex, regex_op_cmp_string, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("inspect", mClassCache.pRegex, regex_inspect, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("match", mClassCache.pRegex, regex_match, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "str");
  method_add("study", mClassCache.pRegex, regex_study, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_b", mClassCache.pRegex, regex_to_b, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_s", mClassCache.pRegex, regex_to_s, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);

  // match
  mClassCache.pMatch = class_create("match");
  method_add("#create", mClassCache.pMatch, match_op_create, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_match", mClassCache.pMatch, match_op_cmp_match, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_bool", mClassCache.pMatch, match_op_cmp_bool, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_string", mClassCache.pMatch, match_op_cmp_string, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("count", mClassCache.pMatch, match_count, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("bounds", mClassCache.pMatch, match_bounds, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "id");
  method_add("get", mClassCache.pMatch, match_get, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "id");

  // time
  mClassCache.pTime = class_create("time");
  method_add("#create", mClassCache.pTime, time_op_create, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#add_int", mClassCache.pTime, time_op_add_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#add_time", mClassCache.pTime, time_op_add_time, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#sub_int", mClassCache.pTime, time_op_sub_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#sub_time", mClassCache.pTime, time_op_sub_time, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#mul_int", mClassCache.pTime, time_op_mul_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#div_int", mClassCache.pTime, time_op_div_int, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#inc", mClassCache.pTime, time_op_inc, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#dec", mClassCache.pTime, time_op_dec, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_time", mClassCache.pTime, time_op_cmp_time, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_int", mClassCache.pTime, time_op_cmp_time, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_bool", mClassCache.pTime, time_op_cmp_time, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_string", mClassCache.pTime, time_op_cmp_time, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("inspect", mClassCache.pTime, time_inspect, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("now!", mClassCache.pTime, time_now_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("year", mClassCache.pTime, time_year, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("month", mClassCache.pTime, time_month, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("day_y", mClassCache.pTime, time_day_y, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("day_m", mClassCache.pTime, time_day_m, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("day_w", mClassCache.pTime, time_day_w, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("min", mClassCache.pTime, time_min, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("sec", mClassCache.pTime, time_sec, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("format", mClassCache.pTime, time_format, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "str");
  method_add("to_b", mClassCache.pTime, time_to_b, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_f", mClassCache.pTime, time_to_f, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_i", mClassCache.pTime, time_to_i, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_s", mClassCache.pTime, time_to_s, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);

  // array
  mClassCache.pArray = class_create("array");
  method_add("#create", mClassCache.pArray, array_op_create, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 1, true, "objects");
  method_add("#idx", mClassCache.pArray, array_op_idx, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#add_array", mClassCache.pArray, array_op_add_array, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#shl_object", mClassCache.pArray, array_op_shl_object, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_array", mClassCache.pArray, array_op_cmp_array, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_bool", mClassCache.pArray, array_op_cmp_bool, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_string", mClassCache.pArray, array_op_cmp_string, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#splat", mClassCache.pArray, array_op_splat, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("inspect", mClassCache.pArray, array_inspect, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("taint_all!", mClassCache.pArray, array_taint_all_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("untaint_all!", mClassCache.pArray, array_untaint_all_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("assert_any", mClassCache.pArray, array_assert_any, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 1, true, "objects");
  method_add("assert_all", mClassCache.pArray, array_assert_all, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 1, true, "objects");
  method_add("any", mClassCache.pArray, array_any, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "fx");
  method_add("all", mClassCache.pArray, array_all, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "fx");
  method_add("has", mClassCache.pArray, array_has, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "obj");
  method_add("each", mClassCache.pArray, array_each, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "fx");
  method_add("each_key", mClassCache.pArray, array_each_key, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "fx");
  method_add("each_pair", mClassCache.pArray, array_each_pair, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "fx");
  method_add("next", mClassCache.pArray, array_next, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("next_key", mClassCache.pArray, array_next_key, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("next_pair", mClassCache.pArray, array_next_pair, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("rewind", mClassCache.pArray, array_rewind, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("find", mClassCache.pArray, array_find, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "obj");
  method_add("sort", mClassCache.pArray, array_sort, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "order");
  method_add("sort!", mClassCache.pArray, array_sort_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "order");
  method_add("sort_by", mClassCache.pArray, array_sort_by, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 2, false, "order", "fx");
  method_add("sort_by!", mClassCache.pArray, array_sort_by_do, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 2, false, "order", "fx");
  method_add("shuffle!", mClassCache.pArray, array_shuffle_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("shuffle", mClassCache.pArray, array_shuffle, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("map!", mClassCache.pArray, array_map_do, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 2, false, "array", "mode");
  method_add("sum", mClassCache.pArray, array_sum, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("mult", mClassCache.pArray, array_mul, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("join", mClassCache.pArray, array_join, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0, 1, false, "spacer");
  method_add("push!", mClassCache.pArray, array_push_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, true, "objects");
  method_add("pop!", mClassCache.pArray, array_pop_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0, 1, "count");
  method_add("reindex", mClassCache.pArray, array_reindex, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("sort_shell", mClassCache.pArray, array_sort_shell, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("sort_quick", mClassCache.pArray, array_sort_quick, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("flatten", mClassCache.pArray, array_flatten, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("inflate", mClassCache.pArray, array_inflate, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("unique", mClassCache.pArray, array_unique, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("select", mClassCache.pArray, array_select, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "fx");
  method_add("reject", mClassCache.pArray, array_reject, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "fx");
  method_add("collect", mClassCache.pArray, array_collect, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(2, 2, false, "start", "fx");
  method_add("reindex!", mClassCache.pArray, array_reindex_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("sort_shell!", mClassCache.pArray, array_sort_shell_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("sort_quick!", mClassCache.pArray, array_sort_quick_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("flatten!", mClassCache.pArray, array_flatten_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("inflate!", mClassCache.pArray, array_inflate_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("unique!", mClassCache.pArray, array_unique_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("select!", mClassCache.pArray, array_select_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "fx");
  method_add("reject!", mClassCache.pArray, array_reject_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "fx");
  method_add("clear!", mClassCache.pArray, array_clear_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("zip", mClassCache.pArray, array_zip, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "arr");
  method_add("zip!", mClassCache.pArray, array_zip_do, M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "arr");
  method_add("min", mClassCache.pArray, array_min, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("max", mClassCache.pArray, array_max, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("min_max", mClassCache.pArray, array_min_max, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("length", mClassCache.pArray, array_length, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_b", mClassCache.pArray, array_to_b, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_i", mClassCache.pArray, array_to_i, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_s", mClassCache.pArray, array_to_s, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);

  // exception
  mClassCache.pException = class_create("exception");
  method_add("#create", mClassCache.pException, exception_op_create, M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 2, false, "msg", "type");
  method_add("#cmp_exception", mClassCache.pException, exception_op_cmp_exception, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_bool", mClassCache.pException, exception_op_cmp_exception, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_string", mClassCache.pException, exception_op_cmp_exception, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("msg", mClassCache.pException, exception_msg, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("type", mClassCache.pException, exception_type, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("file", mClassCache.pException, exception_file, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("line", mClassCache.pException, exception_line, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_b", mClassCache.pException, exception_to_b, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_s", mClassCache.pException, exception_to_s, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);

  // method
  mClassCache.pMethod = class_create("method");
  method_add("#cmp_method", mClassCache.pMethod, method_op_cmp_method, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_bool", mClassCache.pMethod, method_op_cmp_bool, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_string", mClassCache.pMethod, method_op_cmp_string, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("inspect", mClassCache.pMethod, method_inspect, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("class", mClassCache.pMethod, method_class, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("call", mClassCache.pMethod, method_call, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0, 1, true, "params");
  method_add("args", mClassCache.pMethod, method_args, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("args_min", mClassCache.pMethod, method_args_min, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("args_max", mClassCache.pMethod, method_args_max, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_s", mClassCache.pMethod, method_to_s, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);

  // class
  mClassCache.pClass = class_create("class");
  method_add("#create", mClassCache.pClass, class_op_create, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_class", mClassCache.pClass, class_op_cmp_class, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_bool", mClassCache.pClass, class_op_cmp_bool, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("#cmp_string", mClassCache.pClass, class_op_cmp_string, M_PROP_PUBLIC | M_PROP_FINAL)->op();
  method_add("inspect", mClassCache.pClass, class_inspect, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("name", mClassCache.pClass, class_name, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("methods", mClassCache.pClass, class_methods, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("operators", mClassCache.pClass, class_operators, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("members", mClassCache.pClass, class_members, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("members_static", mClassCache.pClass, class_members_static, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("parent", mClassCache.pClass, class_parent, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("count", mClassCache.pClass, class_count, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("to_s", mClassCache.pClass, class_to_s, M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);

  // namespaces

  // malco
  rc_class *malco = class_create("malco", NULL, M_PROP_STATIC);
  method_add("build", malco, malco_build, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("release", malco, malco_release, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("version", malco, malco_version, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("unicode", malco, malco_unicode, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("copyright", malco, malco_copyright, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("error_mode", malco, malco_error_mode, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 1, false, "value");
  method_add("how_to_create_explosives", malco, malco_how_to_create_explosives,
             M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(0);

  // math
  rc_class *math = class_create("math", NULL, M_PROP_STATIC);
  method_add("abs", math, math_abs, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("sin", math, math_sin, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("sinh", math, math_sinh, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("cos", math, math_cos, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("cosh", math, math_cosh, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("tan", math, math_tan, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("tanh", math, math_tanh, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("arcsin", math, math_arcsin, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("arcsinh", math, math_arcsinh, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("arccos", math, math_arccos, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("arccosh", math, math_arccosh, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("arctan", math, math_arctan, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("arctanh", math, math_arctanh, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("sqrt", math, math_sqrt, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("cbrt", math, math_cbrt, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("exp", math, math_exp, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("log", math, math_log, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("log2", math, math_log2, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("log10", math, math_log10, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
  method_add("pi", math, math_pi, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("e", math, math_e, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(0);
  method_add("random", math, math_random, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)
      ->setup(1, 2, false, "min", "max");
  method_add("max", math, math_max, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, true, "objects");
  method_add("min", math, math_min, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, true, "objects");
  method_add("even", math, math_even, M_PROP_STATIC | M_PROP_PUBLIC | M_PROP_FINAL)->setup(1, 1, false, "value");
}

/**
 * Detects task from commandline.
 * @param task Task string.
 * @return Task ID.
 */
int rc_core::cmdline_task(const char *task)
{
  if(*task == '-')
  {
    switch(*(task + 1))
    {
      case 'f': return M_TASK_RUN;
      case 'b': return M_TASK_RUN_BC;
      case 'e': return M_TASK_EVAL;
      case 'c': return M_TASK_COMPILE;
      case 'v': return M_TASK_VERSION;
      case 'i': return M_TASK_CREDITS;
    }
  }

  return M_TASK_WTF;
}

/**
 * Processes current script.
 */
int rc_core::task_run(const char *file)
{
  try
  {
    equip();
    printf("Processing file %s...", mFile);
  }
  catch(const sc_exception &ex)
  {
    error_any(ex.mErrorMsg, ex.mErrorType);
  }

  return 0;
}

/**
 * Compiles current script into bytecode.
 * @todo implement.
 * @returns 0 on success, 1 on error.
 */
int rc_core::task_compile(const char *file)
{
  // if there's no current main file, select one
  if(!mFile->length()) mFile->set(file);

  mSource->file_load(file);
  ic_string filename = file;

  // first check whether we compile .mlc or .rasm file.
  auto ext = std::unique_ptr<ic_string>(filename.substr_get(filename.substr_last(".") + 1));
  if(!ext->compare("rasm"))
  {
    rc_rasm rasm(this, mSource, mFile->get());

    printf("Assembling file '%s'...\n", mFile->get());
    try
    {
      rasm.assemble(mFile);
    }
    catch(const sc_exception &ex)
    {
      switch(ex.mErrorType)
      {
        case M_EMODE_COMPILE:
        case M_EMODE_PARSE:
          printf("Exception in file '%s', line %d:\n%s", rasm.get_error_filename()->get(), rasm.get_error_line_number(),
                 ex.mErrorMsg->get());
          return 1;
        default: // critical error
          throw;
      }
    }
  }
  else
  {
    // initialize parser & compiler
    mCompiler = new rc_compiler();

    // debug output, to be removed!
    printf("Compiling file %s...\n", mFile->get());

    delete mParser;
    mCompiler = NULL;
  }

  return 0;
}

/**
 * Run bytecode directly.
 */
int rc_core::task_run_bc(const char *file)
{
  equip();

  mTape->file_load("test.rbc");
  mStrTable->file_load("test.rst");

  rc_deftable *dt = new rc_deftable(this);
  dt->file_load("test.rdt");
  dt->declare(this);

  return mHead->playback();
}

/**
 * Evaluate code from commandline.
 */
int rc_core::task_eval(const char *str)
{
  return 0;
}

/**
 * Display version info.
 */
int rc_core::task_version()
{
  printf("Malco programming language [ %s : %s ] build %i", MALCO_VERSION, MALCO_RELEASE, MALCO_BUILD);
  if(MALCO_UNICODE) printf("\nUnicode version");
  if(MALCO_DEBUG) printf("\nDEBUG BUILD");

  return 0;
}

/**
 * Display credits.
 */
int rc_core::task_credits()
{
  printf("Malco is %s", MALCO_COPYRIGHT);
  return 0;
}

/**
 * Creates a new class.
 * @param name Name of the new class.
 * @param properties Binary set of properties.
 * @param parent Class to derive new class from.
 * @param root Class to create new class in.
 * @return Pointer to created class.
 * @todo inherit properties from parent object
 */
rc_class *rc_core::class_create(const char *name, rc_class *parent, short properties, rc_class *root)
{
  rc_class *cls    = new rc_class();
  cls->mProperties = properties;
  cls->pRoot       = root;

  int namelen = strlen(name);
  cls->mName  = new char[namelen];
  strcpy(cls->mName, name);

  // affix class to it's root
  if(pClassRoot)
  {
    // class lies on the first level
    if(!root)
    {
      // full name is just name
      root           = pClassRoot;
      cls->mFullName = cls->mName;
    }
    else
    {
      // fullname is parentname::name
      int rootlen    = strlen(root->mFullName);
      cls->mFullName = new char[namelen + 2 + rootlen];
      strcpy(cls->mFullName, root->mFullName);
      strcpy(cls->mFullName + rootlen, "::");
      strcpy(cls->mFullName + rootlen + 2, name);
    }

    if(!root->mClasses.get(name))
      root->mClasses.set(name, (void *)cls);
    else
      ERROR(ic_string::format(M_ERR_OVERRIDE_CLASS, name), M_EXC_SCRIPT);

    // inherit properties
    if(!parent) parent = mClassCache.pObject;

    cls->pParent        = parent;
    cls->mMembers       = parent->mMembers;
    cls->mStaticMembers = parent->mStaticMembers;
  }
  else
  {
    // genuine moment of the genesis
    cls->pParent   = NULL;
    cls->pRoot     = NULL;
    cls->mFullName = cls->mName;
    pClassRoot     = cls;
  }

  return cls;
}

/**
 * Finds a class by it's name.
 * @param name Class name with subnamespaces.
 * @param root Root class to start search from.
 * @return Pointer to resolved class.
 */
inline rc_class *rc_core::class_resolve(const char *name, rc_class *root)
{
  if(!root) root = pClassRoot;

  // split name into chunks
  ic_string str(name);
  sc_voidarray *pieces = str.split("::");

  for(int idx = 0; idx < pieces->length(); idx++)
  {
    ic_string *piece = (ic_string *)pieces->mPtr[idx];
    if(idx > 0 || piece->compare("object")) root = (rc_class *)(root->mClasses.get(piece));

    if(!root) break;
  }

  for(int idx = 0; idx < pieces->length(); idx++)
  {
    ic_string *piece = (ic_string *)pieces->mPtr[idx];
    delete piece;
  }

  delete pieces;
  return root;
}

/**
 * Checks if the current class is a heir of another class.
 * @param base Base class.
 * @param final Final class.
 * @return Heritage flag.
 */
bool rc_core::class_is_heir(rc_class *final, rc_class *base)
{
  while(final != NULL)
  {
    if(final == base) return true;
    final = final->pParent;
  }

  return false;
}

/**
 * Returns an array of class ancestors, up to Object.
 * @param cls Class to start from.
 * @return Array of rc_class pointers.
 */
sc_voidarray *rc_core::class_parents(rc_class *cls)
{
  sc_voidlist list;

  list.add((void *)cls);

  while(cls->pParent && cls->pParent != mClassCache.pObject)
  {
    list.add((void *)cls->pParent);
    cls = cls->pParent;
  }

  return list.pack();
}

/**
 * Return a class' type.
 * This means which default class this class inherits from.
 */
int rc_core::class_type(rc_class *cls)
{
  while(cls)
  {
    if(cls == mClassCache.pUndef)
      return M_CLASS_UNDEF;
    else if(cls == mClassCache.pBool)
      return M_CLASS_BOOL;
    else if(cls == mClassCache.pInt)
      return M_CLASS_INT;
    else if(cls == mClassCache.pFloat)
      return M_CLASS_FLOAT;
    else if(cls == mClassCache.pString)
      return M_CLASS_STRING;
    else if(cls == mClassCache.pRange)
      return M_CLASS_RANGE;
    else if(cls == mClassCache.pRegex)
      return M_CLASS_REGEX;
    else if(cls == mClassCache.pMatch)
      return M_CLASS_MATCH;
    else if(cls == mClassCache.pMethod)
      return M_CLASS_METHOD;
    else if(cls == mClassCache.pTime)
      return M_CLASS_TIME;
    else if(cls == mClassCache.pFile)
      return M_CLASS_FILE;
    else if(cls == mClassCache.pDir)
      return M_CLASS_DIR;
    else if(cls == mClassCache.pSocket)
      return M_CLASS_SOCKET;
    else if(cls == mClassCache.pClass)
      return M_CLASS_CLASS;
    else if(cls == mClassCache.pArray)
      return M_CLASS_ARRAY;
    else if(cls == mClassCache.pException)
      return M_CLASS_EXCEPTION;

    cls = cls->pParent;
  }

  return M_CLASS_OTHER;
}

/**
 * Adds a method to a class.
 * @param name Method name.
 * @param func Pointer to native function.
 * @param properties Binary set of properties.
 * @param root Class to add method to.
 * @return Pointer to created method.
 */
inline rc_method *rc_core::method_add(const char *name, rc_class *root, long point, short properties)
{
  rc_method *existing = (rc_method *)root->mMethods.get(name);
  if(existing && existing->mProperties & M_PROP_FINAL)
    ERROR(ic_string::format(M_ERR_OVERRIDE_FINAL, name), M_EXC_SCRIPT);

  rc_method *method = new rc_method();

  method->mName          = name;
  method->mNative        = false;
  method->pNativeFunc    = NULL;
  method->mExecPoint     = point;
  method->pClass         = root;
  method->mProperties    = properties;
  method->pExternalScope = NULL;
  root->mMethods.set(name, (void *)method);

  return method;
}

/**
 * Adds a method to a class.
 * @param name Method name.
 * @param func Pointer to native function.
 * @param properties Binary set of properties.
 * @param root Class to add method to.
 * @return Pointer to created method.
 */
inline rc_method *rc_core::method_add(const char *name, rc_class *root, native_func func, short properties)
{
  rc_method *existing = (rc_method *)root->mMethods.get(name);
  if(existing && existing->mProperties & M_PROP_FINAL)
    ERROR(ic_string::format(M_ERR_OVERRIDE_FINAL, name), M_EXC_SCRIPT);

  rc_method *method = new rc_method();

  method->mName          = name;
  method->mNative        = true;
  method->pNativeFunc    = func;
  method->mExecPoint     = 0;
  method->pClass         = root;
  method->mProperties    = properties;
  method->pExternalScope = NULL;
  root->mMethods.set(name, (void *)method);

  return method;
}

/**
 * Finds an operator by it's name and class signature.
 * @param name Operator name.
 * @param ax Left operand type.
 * @param bx Right operand type.
 */
rc_method *rc_core::op_resolve(const char *name, rc_class *ax, rc_class *bx)
{
  ic_string *opname = new ic_string(name);
  opname->append('_');

  while(ax != NULL)
  {
    while(bx != NULL)
    {
      ic_string *full = new ic_string(*opname);
      full->append(bx->mName);

      rc_method *method = (rc_method *)ax->mMethods.get(full);
      if(method)
      {
        delete opname;
        delete full;
        return method;
      }

      delete full;
      bx = bx->pParent;
    }

    ax = ax->pParent;
  }

  delete opname;
  return NULL;
}

/**
 * Adds a member to a class.
 * @param name Member name.
 * @param properties Binary set of properties.
 * @param root Class to add member to.
 */
void rc_core::member_add(const char *name, rc_class *root, short properties)
{
  if(root->mMembers.get(name) != NULL || root->mStaticMembers.get(name) != NULL)
    ERROR(ic_string::format(M_ERR_OVERRIDE_FINAL, name), M_EXC_SCRIPT);

  rc_var *member      = new rc_var();
  member->mProperties = properties;
  if(properties & M_PROP_STATIC)
    root->mStaticMembers.set(name, (void *)member);
  else
    root->mMembers.set(name, (void *)member);
}

/**
 * Processes an internal error.
 * @param msg Error message.
 */
inline void rc_core::error(const char *msg)
{
  ic_string *str = new ic_string(msg);
  error_any(str, M_EMODE_ERROR);
  delete str;
}

/**
 * Processes an internal error.
 * @param msg Error message.
 */
inline void rc_core::error(ic_string *msg)
{
  error_any(msg, M_EMODE_ERROR);
}

/**
 * Processes an internal error.
 * @param msg Error message.
 */
inline void rc_core::error(const sc_exception &ex)
{
  ic_string *str;

#if MALCO_DEBUG == 1
  str = ic_string::format("Internal error: %s [file: %s, line %i]\n", ex.mErrorMsg->get(), ex.mFile, ex.mLine);
#else
  str = ic_string::format("Internal error: %s\n", ex.mErrorMsg->get());
#endif

  error_any(str, M_EMODE_ERROR);
  delete str;
}

/**
 * Processes an internal error.
 * @param msg Error message.
 * @param mode Error mode.
 * @param head Execution head that caused the error.
 */
inline void rc_core::error(const char *msg, int mode, rc_head *head)
{
  if(mErrorMode & mode)
  {
    ic_string *str =
        ic_string::format("%s (file '%s', line %i, char %i)", msg, head->mFile, head->mLine, head->mOffset);
    error_any(str, mode);
    delete str;
  }
}

/**
 * Processes an internal error.
 * @param msg Error message.
 * @param mode Error mode.
 * @todo Allow output of errors to file (set by configuration).
 */
inline void rc_core::error_any(ic_string *msg, int mode)
{
  if(mErrorMode & mode) printf(msg->get());

  if(mode != M_EMODE_NOTICE && mode != M_EMODE_WARNING) mState = M_STATE_DEAD;
}

/**
 * Loads core plug-ins.
 * Is to be called once!
 */
void rc_core::plugins_load()
{
}

#endif
