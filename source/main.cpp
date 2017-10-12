//****************************************************************
//*                                                              *
//*                  Malco Programming Language                  *
//*                                                              *
//****************************************************************
//*                                                              *
//*        copyright by Impworks & ForNeVeR, 2006 - #inf         *
//*                                                              *
//****************************************************************
//*                                                              *
//*  Malco is an all-purpose  interpreted  programming language  *
//*  designed to  be  orthogonal and  logical, yet  pleasing to  *
//*  write code  in. It is heavily  inspired  by  PHP and Ruby.  *
//*  It  is  supposed  to  be used  as  console and server-side  *
//*  programming  language, however not limited  to those uses.  *
//*                                                              *
//****************************************************************

#include "malco.h"

int main(int argc, char *argv[])
{
  rc_core malco;

  try
  {
    malco.init();
    int res = malco.process(argc, argv);
    return res;
  }
  catch(const sc_exception &ex)
  {
    malco.error(ex);
    return -1;
  }
}
