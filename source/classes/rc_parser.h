/**
 * @file rc_parser.h
 * @author impworks.
 * rc_parser header.
 * Defines properties and methods of rc_parser class.
 */

#ifndef RC_PARSER_H
#define RC_PARSER_H

//--------------------------------
// rc_lexem
//--------------------------------

/**
 * rc_lexem constructor.
 */
rc_lexem::rc_lexem(char type, long line, long offset, long param)
{
  mType = type;
  mLine = line;
  mOffset = offset;
  mParam = param;
}


//--------------------------------
// rc_parser
//--------------------------------

/**
 * rc_parser constructor.
 */
rc_parser::rc_parser()
{
  mLength = mLastBuf = mLastLength = 0;
}

/**
 * rc_parser destructor.
 */
rc_parser::~rc_parser()
{
}

/**
 * Build table of lexem constants.
 */

#endif