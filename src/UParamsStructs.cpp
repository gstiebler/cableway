//---------------------------------------------------------------------------


#pragma hdrstop

#include "UParamsStructs.h"

#include "UVerticesArestas.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TParamsInfoCircuitos::TParamsInfoCircuitos()
{
  VerticesGerais=new TVerticesGerais;
  VerticesReduzidos = new TVerticesGerais;
}
//---------------------------------------------------------------------------

TParamsInfoCircuitos::~TParamsInfoCircuitos()
{
  delete VerticesGerais;
  delete VerticesReduzidos;
}
//---------------------------------------------------------------------------

