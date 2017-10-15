/**
 * @file malco.h
 * @author impworks
 * General Malco defines.
 * This file defines general constants used all across Malco and includes all
 * other files.
 */

//****************************************************************
//*                                                              *
//*                     all-purpose includes                     *
//*                                                              *
//****************************************************************

// prevent Visual Studio from being annoying
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_WARNINGS

#include <cctype>
#include <clocale>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>

//****************************************************************
//*                                                              *
//*                     general information                      *
//*                                                              *
//****************************************************************

#ifndef MALCO_H
#define MALCO_H

// malco platforms
// clang-format off
#define M_PLATF_WTF   0
#define M_PLATF_WIN32 1
#define M_PLATF_NIX   2
#define M_PLATF_MAC   4
#define M_PLATF_OTHER 1024
// clang-format on

// run modes
constexpr int M_RUNMODE_WTF    = 0;
constexpr int M_RUNMODE_SLICK  = 1;
constexpr int M_RUNMODE_STRICT = 2;
constexpr int M_RUNMODE_SILENT = 4;

// error modes
constexpr int M_EMODE_INIT    = 1;
constexpr int M_EMODE_PARSE   = 2;
constexpr int M_EMODE_COMPILE = 4;
constexpr int M_EMODE_ERROR   = 8;
constexpr int M_EMODE_EXCEPT  = 16;
constexpr int M_EMODE_WARNING = 32;
constexpr int M_EMODE_NOTICE  = 64;

constexpr int M_EMODE_NONE    = 0;
constexpr int M_EMODE_ALL     = 127;
constexpr int M_EMODE_DEFAULT = 63;

// exception types
constexpr int M_EXC_INTERNAL  = 1;
constexpr int M_EXC_LOOP      = 2;
constexpr int M_EXC_MATH      = 4;
constexpr int M_EXC_IO        = 8;
constexpr int M_EXC_SIGNAL    = 16;
constexpr int M_EXC_REGEX     = 32;
constexpr int M_EXC_SCRIPT    = 64;
constexpr int M_EXC_USER      = 128;
constexpr int M_EXC_SECURITY  = 256;
constexpr int M_EXC_ARGS      = 512;
constexpr int M_EXC_NOT_FOUND = 1024;

constexpr int M_EXC_SLIGHT = M_EXC_MATH | M_EXC_IO | M_EXC_SECURITY | M_EXC_ARGS;

// workstates
constexpr int M_STATE_BOOT    = 0;
constexpr int M_STATE_INIT    = 1;
constexpr int M_STATE_PARSE   = 2;
constexpr int M_STATE_COMPILE = 3;
constexpr int M_STATE_RUN     = 4;
constexpr int M_STATE_DONE    = 5;
constexpr int M_STATE_DEAD    = 6;

// tasks
constexpr int M_TASK_WTF     = 0;
constexpr int M_TASK_VERSION = 1;
constexpr int M_TASK_CREDITS = 2;
constexpr int M_TASK_COMPILE = 3;
constexpr int M_TASK_RUN     = 4;
constexpr int M_TASK_RUN_BC  = 5;
constexpr int M_TASK_EVAL    = 6;

// properties
constexpr int M_PROP_PUBLIC   = 1;
constexpr int M_PROP_PRIVATE  = 2;
constexpr int M_PROP_INTERNAL = 4;
constexpr int M_PROP_STATIC   = 8;
constexpr int M_PROP_STUB     = 16;
constexpr int M_PROP_FINAL    = 32;
constexpr int M_PROP_LINK     = 64;
constexpr int M_PROP_CONST    = 128;

// built-in classes
constexpr int M_CLASS_UNDEF     = 0;
constexpr int M_CLASS_BOOL      = 1;
constexpr int M_CLASS_INT       = 2;
constexpr int M_CLASS_FLOAT     = 3;
constexpr int M_CLASS_STRING    = 4;
constexpr int M_CLASS_RANGE     = 5;
constexpr int M_CLASS_REGEX     = 6;
constexpr int M_CLASS_MATCH     = 7;
constexpr int M_CLASS_TIME      = 8;
constexpr int M_CLASS_FILE      = 9;
constexpr int M_CLASS_DIR       = 10;
constexpr int M_CLASS_SOCKET    = 11;
constexpr int M_CLASS_ARRAY     = 12;
constexpr int M_CLASS_METHOD    = 13;
constexpr int M_CLASS_CLASS     = 14;
constexpr int M_CLASS_EXCEPTION = 15;
constexpr int M_CLASS_OTHER     = 42;

// platform
#ifdef _WIN32
#define MALCO_PLATFORM M_PLATF_WIN32
#elif defined(__gnu_linux__)
#define MALCO_PLATFORM M_PLATF_NIX
#elif __APPLE__ && __MACH__
#define MALCO_PLATFORM M_PLATF_MAC
#else
#define MALCO_PLATFORM M_PLATF_WTF
#endif

// misc info
constexpr char *MALCO_VERSION   = "1.0";
constexpr char *MALCO_RELEASE   = "0.0.1";
constexpr int MALCO_BUILD       = 1;
constexpr int MALCO_UNICODE     = 0;
constexpr int MALCO_DEBUG       = 1;
constexpr char *MALCO_COPYRIGHT = "(c) Impworks & ForNeVeR, 2006-#inf";

// all-purpose stuff
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

constexpr long MAXLONG           = 2147483647;
constexpr unsigned long MAXULONG = (unsigned long)-1;

//****************************************************************
//*                                                              *
//*                       global includes                        *
//*                                                              *
//****************************************************************

#if MALCO_PLATFORM == M_PLATF_WIN32
#include <windows.h>
#endif

//****************************************************************
//*                                                              *
//*                       general includes                       *
//*                                                              *
//****************************************************************

#include "errors.h"
#include "rasm.h"

#include "classes.h"
#include "methods.h"


//****************************************************************
//*                                                              *
//*                    classes body includes                     *
//*                                                              *
//****************************************************************

#include "classes/ic_array.h"
#include "classes/ic_bool.h"
#include "classes/ic_dir.h"
#include "classes/ic_file.h"
#include "classes/ic_float.h"
#include "classes/ic_int.h"
#include "classes/ic_match.h"
#include "classes/ic_object.h"
#include "classes/ic_range.h"
#include "classes/ic_regex.h"
#include "classes/ic_socket.h"
#include "classes/ic_string.h"
#include "classes/ic_time.h"

#include "classes/sc_anymap.h"
#include "classes/sc_exception.h"
#include "classes/sc_file.h"
#include "classes/sc_ini.h"
#include "classes/sc_list.h"
#include "classes/sc_map.h"
#include "classes/sc_md5.h"
#include "classes/sc_random.h"
#include "classes/sc_voidarray.h"
#include "classes/sc_voidlist.h"
#include "classes/sc_voidmap.h"

#include "classes/rc_core.h"
#include "classes/rc_deftable.h"
#include "classes/rc_head.h"
#include "classes/rc_method.h"
#include "classes/rc_parser.h"
#include "classes/rc_rasm.h"
#include "classes/rc_strtable.h"
#include "classes/rc_tape.h"
#include "classes/rc_var.h"


//****************************************************************
//*                                                              *
//*                    methods body includes                     *
//*                                                              *
//****************************************************************

#include "methods/m_array.h"
#include "methods/m_bool.h"
#include "methods/m_class.h"
#include "methods/m_exception.h"
#include "methods/m_float.h"
#include "methods/m_int.h"
#include "methods/m_match.h"
#include "methods/m_method.h"
#include "methods/m_object.h"
#include "methods/m_range.h"
#include "methods/m_regex.h"
#include "methods/m_string.h"
#include "methods/m_time.h"
#include "methods/m_undef.h"

#include "methods/m_malco.h"
#include "methods/m_math.h"

#endif
