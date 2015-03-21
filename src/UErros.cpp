//---------------------------------------------------------------------------


#pragma hdrstop

#include "UErros.h"

using namespace std;

// Necess�rio para definir o car�ter singleton da classe. Ou seja, zerar inicialmente a inst�ncia
CErrosMsg * CErrosMsg::instance = 0;

//---------------------------------------------------------------------------
CErrosMsg::CErrosMsg()
{
}

CErrosMsg* CErrosMsg::getInstance()
{
  if ( !instance )
  {
    instance = new CErrosMsg();
  }

  return instance;
}
void CErrosMsg::destroi()
{
  if ( instance )
    delete instance;
  instance = 0;
}

void CErrosMsg::novoErro(string novo)
{
	printf( "%s\n", novo.c_str() );
}

