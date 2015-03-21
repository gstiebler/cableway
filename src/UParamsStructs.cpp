//---------------------------------------------------------------------------


#pragma hdrstop

#include "UParamsStructs.h"

#include "UVerticesArestas.h"



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

