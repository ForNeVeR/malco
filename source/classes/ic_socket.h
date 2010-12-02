/**
 * @file ic_socket.h
 * @author impworks.
 * ic_socket header.
 * Defines properties and methods of ic_socket class.
 */

#ifndef IC_SOCKET_H
#define IC_SOCKET_H

/**
 * ic_file constructor.
 */
ic_socket::ic_socket()
{
  mName = NULL;
  mMode = 0;
}

#if MALCO_PLATFORM == M_PLATF_WIN32
#include "ic_socket.w32.h"
#endif

#if MALCO_PLATFORM == M_PLATF_NIX
#include "ic_socket.nix.h""
#endif

#endif