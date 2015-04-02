//---------------------------------------------------------------------------

#ifndef UParamsStructsH
#define UParamsStructsH
//---------------------------------------------------------------------------

class TVerticesGerais;
class TListaArestas;

#include "UVerticesArestas.h"

#include <vector>
#include <memory>

struct TParamsDadosGenerico
{
  shared_ptr<TDesenho> drawing;
};

struct TParamsGrafoDesenho: public TParamsDadosGenerico
{
  TVerticesGerais *VerticesGerais;
};
//---------------------------------------------------------------------------

struct TParamsInfoCircuitos
{
  TParamsInfoCircuitos();
  ~TParamsInfoCircuitos();
  TVerticesGerais *VerticesGerais, *VerticesReduzidos;
  vector< shared_ptr<TAresta> > Arestas;
  int NumDesenhos;
};
//---------------------------------------------------------------------------

#endif


