//---------------------------------------------------------------------------

#ifndef UParamsStructsH
#define UParamsStructsH
//---------------------------------------------------------------------------

class TNiveisProjetoTransfer;
class TVerticesGerais;
class TListaArestas;

#include "UVerticesArestas.h"

#include <vector>

struct TParamsDadosGenerico
{
  int IndiceDesenho;
  TNiveisProjetoTransfer *NiveisProjeto;
  int IDArquivo; 
//  TInterfaceMainPar *IMP;
};

struct TParamsGrafoDesenho: public TParamsDadosGenerico
{
  bool CarregaGrafo;
  double AlturaTeto;
  double AlturaInterrup;
  double Altura;
  TVerticesGerais *VerticesGerais;
  TListaArestas *Arestas;
};
//---------------------------------------------------------------------------

struct TParamsInfoCircuitos
{
  TParamsInfoCircuitos();
  ~TParamsInfoCircuitos();
  TVerticesGerais *VerticesGerais, *VerticesReduzidos;
  TListaArestas *Arestas;
  std::vector<TArestaReduzida> ArestasReduzidas;
  int NumDesenhos;
};
//---------------------------------------------------------------------------

#endif


