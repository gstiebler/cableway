//---------------------------------------------------------------------------
#ifndef UContainerDesenhosH
#define UContainerDesenhosH
//---------------------------------------------------------------------------

#include "UGrafoDesenho.h"
#include "UInfoCircuitos.h"
//#include "UfrmDesenho.h"
#include "TDesenho.h"
#include "UNiveisProjeto.h"
#include "UConfig.h"
#include "UTCallbackStatusCarregamento.h"
#include "UCallbacks.h"
#include <stack>
#include <vector>

struct intVertices
{
  int verticeAtual;
  int verticeOriginal;
  intVertices(int vAtual, int vOriginal) {verticeAtual = vAtual; verticeOriginal = vOriginal;}
};

class CContainerDesenhos
{
private:
  std::vector<void*> ListaDesenhos;
  TParamsInfoCircuitos ParamsInfoCircuitos;
  TNiveisProjetoTransfer *Niveis;
  static bool verificaTextoWrap(void* PonteiroThis, const char *str);
  bool verificaTexto(string str);
  void GeraListaAdjacencias();
  void buscaEmProfundidadeOsVertices(bool *VerticesVisitados, int indice, bool arestazerada, TArestaReduzida arestaRed);
public:
  CContainerDesenhos(TNiveisProjetoTransfer *niveis);
//  CContainerDesenhos(TInterfaceMainPar *imp);
  ~CContainerDesenhos();
  CInfoCircuitos *InfoCircuitos;
  //TfrmDesenho **frmDesenhos;
  void AdicionaDesenho(string NomeArquivo, int id, double altura, bool carregagrafo);

  void addDrawing( CDadosGenerico dados );

  TDesenho * getDesenho(int Indice);
  void CriaFormDesenho(int Indice);
  void MostraCircuito(string circuito);
  void MostraArvore(string Nome);
  void MostraDoubleArvore(string Nome, string Nome2);
  void MudaNiveisDeProjeto(TNiveisProjeto* NiveisProjeto);
  int NumDesenhos();
  void Conclui(callbackStatusCarregamento &call);
  void ReduzGrafo();
  //TfrmDesenhoAbas *getfrmDesenhoAbas();
//  CConfig *Config;
};
//---------------------------------------------------------------------------
#endif
