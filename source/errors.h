/**
 * @file errors.h
 * @author impworks
 * Error codes and macros.
 * This file declares all error-related codes and macros.
 */

#ifndef ERRORS_H
#define ERRORS_H

/**
 * Macro for deep error throwing.
 */
#ifdef ERROR
#undef ERROR
#endif

#if MALCO_DEBUG == 1
#define ERROR(msg, level) throw new sc_exception(msg, level, __FILE__, __LINE__)
#else
#define ERROR(msg, level) throw new sc_exception(msg, level)
#endif

/*
  The error messages are interpolated during output.
  It is possible to use the any of the 'printf' function special codes:
*/

#define M_ERR_WTF                   "Unknown error has occured. WTF?"
#define M_ERR_NO_MEMORY             "Not enough memory."
#define M_ERR_BAD_INNER_INDEX       "Index was outside of bounds."
#define M_ERR_NO_SOURCE             "Specified source file was not found."
#define M_ERR_NO_STRINGTABLE        "Corresponding string table was not found."
#define M_ERR_NO_DEFTABLE           "Corresponding definition table was not found."
#define M_ERR_NO_SETUP              "Setup file (malco.ini) was not found."
#define M_ERR_BAD_SETUP             "Setup statement '%s' does not exist."
#define M_ERR_NO_EXTENSION          "Extension %s could not be found."
#define M_ERR_BAD_EXTENSION         "'%s' is not a valid Malco extension."
#define M_ERR_IO_NO_FILE            "Required file was not found."
#define M_ERR_BAD_MODE              "Run mode '%s' is incorrect."
#define M_ERR_BAD_COMMANDLINE       "Incorrect command line. Usage:\n malco -f filename.mlc\n"\
                                    " malco -b filename.rbc\n malco -e 'code'\n"\
                                    " malco -c (filename.mlc|filename.rasm)\n malco -v\n malco -i"
#define M_ERR_CACHE_WRITE_FAIL      "Cannot save bytecode cache / tables on disk."

#define M_ERR_PARSE_UNEXPECTED      "Unexpected token '%s'."
#define M_ERR_PARSE_UNKNOWN         "Unknown token '%s'."
#define M_ERR_FINAL_STUB            "Class '%s' cannot be defined as both final and stub."
#define M_ERR_ASSIGN_NON_LVALUE     "Cannot assign to a non-lvalue."
#define M_ERR_ASSIGN_CONST          "Cannot assign to a constant."

#define M_ERR_METHOD_IN_METHOD      "Cannot declare method '%s' in method '%s' context."
#define M_ERR_CLASS_IN_METHOD       "Cannot declare class '%s' in method '%s' context."
#define M_ERR_EXTEND_IN_METHOD      "Cannot extend class '%s' in method '%s' context."
#define M_ERR_METHOD_IN_LAMBDA      "Cannot declare method '%s' in lambda context."
#define M_ERR_CLASS_IN_LAMBDA       "Cannot declare class '%s' in lambda context."
#define M_ERR_EXTEND_IN_LAMBDA      "Cannot extend class '%s' in lambda context."
#define M_ERR_EXTEND_PROPERTY       "Cannot extend object with properties."
#define M_ERR_LINK_TO_CONST         "Cannot create link to a constant."
#define M_ERR_LINK_IN_DESTRUCTOR    "Cannot create links to $@ in a destructor."
#define M_ERR_MODIFIER_ON_CONST     "Cannot call a modifier method on a constant."
#define M_ERR_INC_CONST             "Cannot increment a constant."
#define M_ERR_DEC_CONST             "Cannot decrement a constant."

#define M_ERR_NO_FX                 "Method '%s' does not exist in class '%s'."
#define M_ERR_NO_OP                 "Operator '%s' is not defined for class '%s'."
#define M_ERR_FX_MANY_PARAMS        "Too many parameters in call of method '%s'."
#define M_ERR_FX_FEW_PARAMS         "Too few parameters in call of method '%s'."
#define M_ERR_FX_WRONG_TYPE         "Parameter %i does not belong to class '%s' in method '%s'."
#define M_ERR_OP_WRONG_TYPE         "Cannot join objects of classes '%s' and '%s' with operator '%s'."
#define M_ERR_NO_PROPERTY           "Property '%s' does not exist in class '%s'."
#define M_ERR_NO_CLASS              "Class '%s' does not exist."
#define M_ERR_OVERRIDE_CLASS        "Class '%s' already exists."
#define M_ERR_OVERRIDE_FINAL        "Cannot override final method '%s'."
#define M_ERR_DERIVE_FINAL          "Cannot derive child class '%s' from a final class."
#define M_ERR_EXTEND_FINAL          "Cannot extend final class '%s'."
#define M_ERR_DUPLICATE_PROPERTY    "Cannot create several properties with the same name in class '%s'."
#define M_ERR_CREATE_STUB           "Cannot create object of stub class '%s'."
#define M_ERR_THIS_DISABLED         "$@ pointer does not exist in current context."
#define M_ERR_PROPERTY_PRIVATE      "Cannot access private property '%s' in class '%s'."
#define M_ERR_FX_PRIVATE            "Cannot invoke private method '%s' in class '%s'."
#define M_ERR_BAD_IDENTIFIER        "No class/method/constant named '%s' found in '%s'."
#define M_ERR_BAD_BIND              "Cannot bind a non-method object."
#define M_ERR_FROZEN                "Cannot modify a frozen object."
#define M_ERR_DIRECT_OP_INVOKE      "Cannot invoke an operator directly by it's name."
#define M_ERR_OBJECT_PARENT         "The Object class has no parent."
#define M_ERR_CLASS_ROOT            "Class '%s' is on the top level and has no root."
#define M_ERR_BAD_STATIC_CALL       "Cannot statically call a dynamic method '%s'."

#define M_ERR_DIV_BY_ZERO           "Division by zero."
#define M_ERR_NEGATIVE_SQRT         "Calculating square root of a negative number."
#define M_ERR_BAD_MATH              "Mathematically disallowed operation '%s'."
#define M_ERR_CYCLIC_ASSIGN         "Cannot assign an object to it's property or subitem."
#define M_ERR_BREAK_1               "Cannot break level 1."
#define M_ERR_RETURN_1              "Cannot return from level 1."
#define M_ERR_EXCEPTION             "Uncaught exception:\n%s\nFile: %s; line: %i"
#define M_ERR_BAD_REGEXP            "Regular expression could not compile."
#define M_ERR_BAD_ITERATOR          "Object given for foreach is not iterable."
#define M_ERR_BAD_TO_A              "to_a() method should return an array or it's derived type."
#define M_ERR_BAD_TO_B              "to_b() method should return a boolean or it's derived type."
#define M_ERR_BAD_TO_F              "to_f() method should return a float or it's derived type."
#define M_ERR_BAD_TO_I              "to_i() method should return an integer or it's derived type."
#define M_ERR_BAD_TO_S              "to_s() method should return a string or it's derived type."
#define M_ERR_BAD_REL_RETURN        "rel operator should return a boolean."
#define M_ERR_BAD_CMP_RETURN        "cmp operator should return an integer or an undef."
#define M_ERR_BAD_INDEX             "Cannot use this kind of object for index."
#define M_ERR_TYPE_EXPECTED         "The method is expected to return an object of %s type or it's derivative."
#define M_ERR_MATCH_CONSTRUCTOR     "Cannot explicitly create a match object. It can only be returned from a regexp."
#define M_ERR_CLASS_CONSTRUCTOR     "Cannot explicitly create a class. It should be defined in code instead."

#define M_ERR_UNKNOWN_CMD           "Unknown RASM command #%i at offset %i."
#define M_ERR_UNKNOWN_MOD           "Unknown RASM modifier #%i at offset %i."
#define M_ERR_OP_EMPTY_REGISTER     "Cannot invoke operator '%s' if AX or BX are empty."
#define M_ERR_POP_US_EMPTY          "Cannot pop a value from an empty user stack"
#define M_ERR_NO_CONST_STRING       "Constant string was does not exist in the table."
#define M_ERR_GOTO_NOWHERE          "Cannot goto address '%i' because it exceeds tape length."
#define M_ERR_INTERNAL              "Internal error: %s"

#define M_ERR_RASM_UNKNOWN_CMD      "Unknown command name: '%s'."
#define M_ERR_RASM_BAD_MOD          "Bad modifiers for entity '%s'."
#define M_ERR_RASM_BAD_LABEL_NAME   "Bad label name '%s'."
#define M_ERR_RASM_BAD_LINE         "Cannot parse line: '%s'."
#define M_ERR_RASM_WRONG_MOD_SET    "Bad modifiers set."
#define M_ERR_RASM_NESTED_METHOD    "Cannot declare method '%s' inside method '%s'."
#define M_ERR_MALFORMED_STRING      "String constant was expected but got '%s'."
#define M_ERR_BAD_ESCAPE_SEQ        "Cannot parse escape sequences in string '%s'."

#define M_WARN_IMBALANCED_PASG      "Number of arguments on both sides of a parallel assignment isn't equal."
#define M_WARN_OVERRIDE_FX          "Method '%s' overrides equally named method defined in Object."
#define M_WARN_EMPTY_FX             "Method '%s' is empty."
#define M_WARN_EMPTY_CLASS          "Class '%s' is empty."
#define M_WARN_EMPTY_LAMBDA         "Lambda is empty."
#define M_WARN_MEANINGLESS          "Instruction is possibly meaningless.";
#define M_WARN_FX_UNKNOWN_PARAM     "Method '%s' does not have a parameter named '%s'."
#define M_WARN_FX_NO_RETURN         "Method was expected to return something, but it didn't. Using undef as default."
#define M_WARN_FX_BAD_RETURN_COUNT  "Method returned %i values, but was expected to return %i. Other values omitted."
#define M_WARN_REGEX_SUBSTR_ID      "Match data can only store up to 10 substrings, from 0 to 9."
#define M_WARN_EMPTY_SUBSTR         "Source string was not of sufficient length to get a proper substring."
#define M_WARN_ARRAY_INCOMPARABLE   "Array contains incomparable objects, '%s' failed."
#define M_WARN_DUPLICATE_KEY        "Key '%s' was duplicate, thus only the last value has been saved."

#endif
