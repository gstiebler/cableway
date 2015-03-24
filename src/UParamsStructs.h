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
  int IndiceDesenho;
  int IDArquivo; 
//  TInterfaceMainPar *IMP;
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


