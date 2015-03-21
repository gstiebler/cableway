//---------------------------------------------------------------------------


#pragma hdrstop

#include "UErros.h"

using namespace std;

// Necessário para definir o caráter singleton da classe. Ou seja, zerar inicialmente a instância
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

