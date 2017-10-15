/**
 * @file rasm.h
 * @author impworks
 * General RASM defines.
 * This file defines operation and modifier codes for RASM bytecode.
 */

#ifndef RASM_H
#define RASM_H

// modifiers
constexpr int RASM_MOD_DEFAULT  = 0;
constexpr int RASM_MOD_UNDEF    = 1;
constexpr int RASM_MOD_TRUE     = 2;
constexpr int RASM_MOD_FALSE    = 3;
constexpr int RASM_MOD_INT      = 4;
constexpr int RASM_MOD_FLOAT    = 5;
constexpr int RASM_MOD_STRING   = 6;
constexpr int RASM_MOD_VAR      = 7;
constexpr int RASM_MOD_PROPERTY = 8;
constexpr int RASM_MOD_NAMES    = 9;
constexpr int RASM_MOD_NULL     = 10;
constexpr int RASM_MOD_LINK     = 11;
constexpr int RASM_MOD_CONST    = 12;
constexpr int RASM_MOD_SELF     = 13;

// operation codes

// data transfer
constexpr int RASM_CMD_WTF    = 0;
constexpr int RASM_CMD_LOADAX = 1;
constexpr int RASM_CMD_LOADBX = 2;

constexpr int RASM_CMD_SAVEAX      = 4;
constexpr int RASM_CMD_SAVEBX      = 5;
constexpr int RASM_CMD_XCHG        = 6;
constexpr int RASM_CMD_ASSIGN      = 7;
constexpr int RASM_CMD_UNSPLASSIGN = 8;

// basic operations
constexpr int RASM_CMD_ADD  = 9;
constexpr int RASM_CMD_SUB  = 10;
constexpr int RASM_CMD_MUL  = 11;
constexpr int RASM_CMD_DIV  = 12;
constexpr int RASM_CMD_MOD  = 13;
constexpr int RASM_CMD_POW  = 14;
constexpr int RASM_CMD_SHL  = 15;
constexpr int RASM_CMD_SHR  = 16;
constexpr int RASM_CMD_BAND = 17;
constexpr int RASM_CMD_BOR  = 18;
constexpr int RASM_CMD_BXOR = 19;
constexpr int RASM_CMD_AND  = 20;
constexpr int RASM_CMD_OR   = 21;
constexpr int RASM_CMD_XOR  = 22;

// unary operations
constexpr int RASM_CMD_INC = 23;
constexpr int RASM_CMD_DEC = 24;
constexpr int RASM_CMD_NEG = 25;

// comparison and jumps
constexpr int RASM_CMD_EQ         = 26;
constexpr int RASM_CMD_EQ_STRICT  = 27;
constexpr int RASM_CMD_REL        = 28;
constexpr int RASM_CMD_LESS       = 29;
constexpr int RASM_CMD_LESS_EQ    = 30;
constexpr int RASM_CMD_GREATER    = 31;
constexpr int RASM_CMD_GREATER_EQ = 32;
constexpr int RASM_CMD_CMP        = 33;
constexpr int RASM_CMD_JTRUE      = 34;
constexpr int RASM_CMD_JFALSE     = 35;
constexpr int RASM_CMD_JMP        = 36;

// stack commands
constexpr int RASM_CMD_PUSHUS  = 37;
constexpr int RASM_CMD_POPUS   = 38;
constexpr int RASM_CMD_PUSHSRC = 39;
constexpr int RASM_CMD_POPSRC  = 40;
constexpr int RASM_CMD_PUSHDST = 41;
constexpr int RASM_CMD_POPDST  = 42;
constexpr int RASM_CMD_SPLAT   = 43;
constexpr int RASM_CMD_UNSPLAT = 44;
constexpr int RASM_CMD_MOVESRC = 45;
constexpr int RASM_CMD_MOVEDST = 46;
constexpr int RASM_CMD_CLRSRC  = 47;
constexpr int RASM_CMD_CLRDST  = 48;

// object manipulation
constexpr int RASM_CMD_NEW        = 49;
constexpr int RASM_CMD_CALL       = 50;
constexpr int RASM_CMD_RETURN     = 51;
constexpr int RASM_CMD_NSP        = 52;
constexpr int RASM_CMD_BINDLAMBDA = 53;

// array-specific actions
constexpr int RASM_CMD_INDEX = 54;

// interpreter behavior modification
constexpr int RASM_CMD_INCLUDE = 55;
constexpr int RASM_CMD_REQUIRE = 56;
constexpr int RASM_CMD_GC      = 57;
constexpr int RASM_CMD_SETPTY  = 58;
constexpr int RASM_CMD_SETFILE = 59;
constexpr int RASM_CMD_SETLINE = 60;
constexpr int RASM_CMD_THROW   = 61;
constexpr int RASM_CMD_TRY     = 62;
constexpr int RASM_CMD_TRIED   = 63;
constexpr int RASM_CMD_EXIT    = 64;

// class table generator commands
constexpr int RASM_CMD_REGCLASS    = 65;
constexpr int RASM_CMD_REGPROPERTY = 66;
constexpr int RASM_CMD_REGMETHOD   = 67;

constexpr int RASM_CMD_INSPECT = 255;

#endif
