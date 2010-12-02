/**
 * @file rasm.h
 * @author impworks
 * General RASM defines.
 * This file defines operation and modifier codes for RASM bytecode.
 */

#ifndef RASM_H
#define RASM_H

// modifiers
#define RASM_MOD_DEFAULT      0
#define RASM_MOD_UNDEF        1
#define RASM_MOD_TRUE         2
#define RASM_MOD_FALSE        3
#define RASM_MOD_INT          4
#define RASM_MOD_FLOAT        5
#define RASM_MOD_STRING       6
#define RASM_MOD_VAR          7
#define RASM_MOD_PROPERTY     8
#define RASM_MOD_NAMES        9
#define RASM_MOD_NULL         10
#define RASM_MOD_LINK         11
#define RASM_MOD_CONST        12
#define RASM_MOD_SELF         13

// operation codes

// data transfer
#define RASM_CMD_WTF          0
#define RASM_CMD_LOADAX       1
#define RASM_CMD_LOADBX       2

#define RASM_CMD_SAVEAX       4
#define RASM_CMD_SAVEBX       5
#define RASM_CMD_XCHG         6
#define RASM_CMD_ASSIGN       7
#define RASM_CMD_UNSPLASSIGN  8

// basic operations
#define RASM_CMD_ADD          9
#define RASM_CMD_SUB          10
#define RASM_CMD_MUL          11
#define RASM_CMD_DIV          12
#define RASM_CMD_MOD          13
#define RASM_CMD_POW          14
#define RASM_CMD_SHL          15
#define RASM_CMD_SHR          16
#define RASM_CMD_BAND         17
#define RASM_CMD_BOR          18
#define RASM_CMD_BXOR         19
#define RASM_CMD_AND          20
#define RASM_CMD_OR           21
#define RASM_CMD_XOR          22

// unary operations
#define RASM_CMD_INC          23
#define RASM_CMD_DEC          24
#define RASM_CMD_NEG          25

// comparison and jumps
#define RASM_CMD_EQ           26
#define RASM_CMD_EQ_STRICT    27
#define RASM_CMD_REL          28
#define RASM_CMD_LESS         29
#define RASM_CMD_LESS_EQ      30
#define RASM_CMD_GREATER      31
#define RASM_CMD_GREATER_EQ   32
#define RASM_CMD_CMP          33
#define RASM_CMD_JTRUE        34
#define RASM_CMD_JFALSE       35
#define RASM_CMD_JMP          36

// stack commands
#define RASM_CMD_PUSHUS       37
#define RASM_CMD_POPUS        38
#define RASM_CMD_PUSHSRC      39
#define RASM_CMD_POPSRC       40
#define RASM_CMD_PUSHDST      41
#define RASM_CMD_POPDST       42
#define RASM_CMD_SPLAT        43
#define RASM_CMD_UNSPLAT      44
#define RASM_CMD_MOVESRC      45
#define RASM_CMD_MOVEDST      46
#define RASM_CMD_CLRSRC       47
#define RASM_CMD_CLRDST       48

// object manipulation
#define RASM_CMD_NEW          49
#define RASM_CMD_CALL         50
#define RASM_CMD_RETURN       51
#define RASM_CMD_NSP          52
#define RASM_CMD_BINDLAMBDA   53

// array-specific actions
#define RASM_CMD_INDEX        54

// interpreter behavior modification
#define RASM_CMD_INCLUDE      55
#define RASM_CMD_REQUIRE      56
#define RASM_CMD_GC           57
#define RASM_CMD_SETPTY       58
#define RASM_CMD_SETFILE      59
#define RASM_CMD_SETLINE      60
#define RASM_CMD_THROW        61
#define RASM_CMD_TRY          62
#define RASM_CMD_TRIED        63
#define RASM_CMD_EXIT         64

// class table generator commands
#define RASM_CMD_REGCLASS     65
#define RASM_CMD_REGPROPERTY  66
#define RASM_CMD_REGMETHOD    67

#define RASM_CMD_INSPECT      255

#endif
