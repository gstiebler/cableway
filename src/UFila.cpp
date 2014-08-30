//---------------------------------------------------------------------------


#pragma hdrstop

#include "UFila.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CFila::CFila(int Tam)
{
  tam=Tam;
  ini=fim=NumItens=0;
  lista=new int [tam];
}
//---------------------------------------------------------------------------

CFila::~CFila()
{
  delete [] lista;
}
//---------------------------------------------------------------------------

void CFila::Adiciona(int Vertice)
{
  lista[fim]=Vertice;
  fim++;
  fim%=tam;
  NumItens++;
}
//---------------------------------------------------------------------------

int CFila::Retira()
{
  int retorno;
  if (NumItens)
  {
    retorno=lista[ini];
    ini++;
    ini=ini % tam;
    NumItens--;
    return retorno;
  }
  return -1;
}
//---------------------------------------------------------------------------

int CFila::Tamanho()
{
  return NumItens;
}
//---------------------------------------------------------------------------
