//---------------------------------------------------------------------------


#pragma hdrstop

#include "UParamsStructs.h"

#include "UNiveisProjeto.h"
#include "UVerticesArestas.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TParamsInfoCircuitos::TParamsInfoCircuitos()
{
  VerticesGerais=new TVerticesGerais;
  VerticesReduzidos = new TVerticesGerais;
  Arestas=new TListaArestas;
}
//---------------------------------------------------------------------------

TParamsInfoCircuitos::~TParamsInfoCircuitos()
{
  delete VerticesGerais;
  delete VerticesReduzidos;
  delete Arestas;
}
//---------------------------------------------------------------------------

