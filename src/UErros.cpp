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

//void CErrosMsg::defineMemo(TMemo* memo)
//{
//  erros = memo;
//}

void CErrosMsg::novoErro(string novo)
{
  //int pos;

  //string temp, original;
  //original = novo.c_str();
  //while (1)
  //{
  //  pos=original.AnsiPos("\n");
  //  if (!pos)
  //    break;
  //  temp=original.SubString(1, pos-1);
  //  original.Delete(1, pos);
  //  //erros->Lines->Add(temp);
  //}
  //erros->Lines->Add(original);

//  tmp.
}


#pragma package(smart_init)
