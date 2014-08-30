//---------------------------------------------------------------------------

#ifndef UParamsStructsH
#define UParamsStructsH
//---------------------------------------------------------------------------

class TNiveisProjetoTransfer;
class TVerticesGerais;
class TListaArestas;
class TListaArestasReduzidas;

struct TParamsDadosGenerico
{
  char *NomeArq;
  int TamNomeArq;
  bool FechaArquivo;
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
  TListaArestasReduzidas *ArestasReduzidas;
  int NumDesenhos;
};
//---------------------------------------------------------------------------

#endif


