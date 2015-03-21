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
  bool CarregaGrafo;
  double Altura;
  TVerticesGerais *VerticesGerais;
};
//---------------------------------------------------------------------------

struct TParamsInfoCircuitos
{
  TParamsInfoCircuitos();
  ~TParamsInfoCircuitos();
  TVerticesGerais *VerticesGerais, *VerticesReduzidos;
  vector< shared_ptr<TAresta> > Arestas;
  std::vector<TArestaReduzida> ArestasReduzidas;
  int NumDesenhos;
};
//---------------------------------------------------------------------------

#endif


