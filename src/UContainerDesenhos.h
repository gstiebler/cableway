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
  std::vector< std::shared_ptr<TDesenho> > ListaDesenhos;
  void ligaColaresEntreDesenhos();
  static void ChecagemVerticeDuplo( std::shared_ptr<Graph> graph );
public:
  std::shared_ptr<CInfoCircuitos> InfoCircuitos;

  void addDrawing( std::shared_ptr<CDadosGenerico> dados, double altura, std::string fileName );

  std::shared_ptr<TDesenho> getDesenho(int Indice);
  void MostraCircuito(std::string circuito);
  void MostraArvore(std::string Nome);
  void MostraDoubleArvore(std::string Nome, std::string Nome2);
  int NumDesenhos();
  void Conclui();
  std::shared_ptr<Graph> _graph;
};
//---------------------------------------------------------------------------
#endif
