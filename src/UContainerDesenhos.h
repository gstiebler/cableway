//---------------------------------------------------------------------------
#ifndef UContainerDesenhosH
#define UContainerDesenhosH
//---------------------------------------------------------------------------

#include "UGrafoDesenho.h"
#include "UInfoCircuitos.h"
//#include "UfrmDesenho.h"
#include "TDesenho.h"
#include "UConfig.h"
#include "UTCallbackStatusCarregamento.h"
#include "UCallbacks.h"
#include <stack>
#include <vector>

struct TDesenho;

struct intVertices
{
  int verticeAtual;
  int verticeOriginal;
  intVertices(int vAtual, int vOriginal) {verticeAtual = vAtual; verticeOriginal = vOriginal;}
};

class CContainerDesenhos
{
private:
  std::vector<TDesenho*> ListaDesenhos;
  TParamsInfoCircuitos ParamsInfoCircuitos;
  static bool verificaTextoWrap(void* PonteiroThis, const char *str);
  bool verificaTexto(string str);
  void GeraListaAdjacencias();
  void buscaEmProfundidadeOsVertices(bool *VerticesVisitados, int indice, bool arestazerada, TArestaReduzida arestaRed);
public:
  CContainerDesenhos();
  ~CContainerDesenhos();
  CInfoCircuitos *InfoCircuitos;

  void addDrawing( std::shared_ptr<CDadosGenerico> dados, double altura );

  TDesenho * getDesenho(int Indice);
  void MostraCircuito(string circuito);
  void MostraArvore(string Nome);
  void MostraDoubleArvore(string Nome, string Nome2);
  int NumDesenhos();
  void Conclui(callbackStatusCarregamento &call);
  void ReduzGrafo();
};
//---------------------------------------------------------------------------
#endif
