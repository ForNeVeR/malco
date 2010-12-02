/**
 * @file ic_dir.h
 * @author impworks.
 * ic_dir header.
 * Defines properties and methods of ic_socket class.
 */

#ifndef IC_DIR_H
#define IC_DIR_H

/**
 * ic_dir constructor.
 */
ic_dir::ic_dir()
{
  mName = NULL;
  mLength = 0;  
}

/**
 * ic_dir constructor.
 * @param name Directory name.
 */
ic_dir::ic_dir(const char *name)
{
  mName = new char[strlen(name)];
  strcpy(mName, name);
  mLength = 0;  
}

/**
 * ic_dir constructor.
 * @param name Directory name.
 */
ic_dir::ic_dir(ic_string *name) 
{
  mName = new char[name->length()];
  strcpy(mName, name->get());
  mLength = 0;
}

#if MALCO_PLATFORM == M_PLATF_WIN32
#include "ic_dir.w32.h"
#endif

#if MALCO_PLATFORM == M_PLATF_NIX
#include "ic_dir.nix.h"
#endif

#endif