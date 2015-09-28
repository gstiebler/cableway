//---------------------------------------------------------------------------
#ifndef UContainerDesenhosH
#define UContainerDesenhosH
//---------------------------------------------------------------------------

#include "UInfoCircuitos.h"
#include <stack>
#include <vector>

struct TDesenho;
class Graph;
class CDadosGenerico;

struct intVertices
{
  int verticeAtual;
  int verticeOriginal;
  intVertices(int vAtual, int vOriginal) {verticeAtual = vAtual; verticeOriginal = vOriginal;}
};

class CContainerDesenhos
{
private:
  std::vector< shared_ptr<TDesenho> > ListaDesenhos;
  void ligaColaresEntreDesenhos();
  static void ChecagemVerticeDuplo( shared_ptr<Graph> graph );
public:
  shared_ptr<CInfoCircuitos> InfoCircuitos;

  void addDrawing( std::shared_ptr<CDadosGenerico> dados, double altura, std::string fileName );

  shared_ptr<TDesenho> getDesenho(int Indice);
  void MostraCircuito(string circuito);
  void MostraArvore(string Nome);
  void MostraDoubleArvore(string Nome, string Nome2);
  int NumDesenhos();
  void Conclui();
  shared_ptr<Graph> _graph;
};
//---------------------------------------------------------------------------
#endif
