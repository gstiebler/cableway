//---------------------------------------------------------------------------
#ifndef UInfoCircuitosH
#define UInfoCircuitosH
//---------------------------------------------------------------------------
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <set>

#include "UDefines.h"
#include "UArmazenamentoCircuitos.h"
#include "UVerticesArestas.h"

class TListaArestas;
class TParamsInfoCircuitos;
class TArestasCircuito;
class TVerticesGerais;
class Graph;


class CInfoCircuitos
{
private:
  static void SeparaRota(std::string ListaPontos, vector<string> &ListaRota);
  static void MergeRota(vector<std::string> &rota, vector<std::string> NovaParte);
  set<string> getLevelsFromVertex( shared_ptr<TVerticeGeral> vertexIndex ) const;

public:      
  CInfoCircuitos( shared_ptr<Graph> graph );
  ~CInfoCircuitos();
  void AdicionaCircuito(TCircuito &Circuito);
  vector< shared_ptr<TAresta> >& ArestasCircuito( std::string circuitName, shared_ptr<TDesenho> drawing) const;
  void PontosAresta(TPonto Pontos[2], shared_ptr<TAresta> Aresta);
  bool GeraRota(string V1, string V2, string ListaPontos, double &tam, vector<string> &rota, shared_ptr<TArestasCircuito> ArestasCircuito) const;
  bool GeraRota(string Destino, string Origem, double &tam, vector<string> &rota, vector< shared_ptr<TAresta> > &ListaArestas) const;
  void Arvore( shared_ptr<TVerticeGeral> Vertice, std::vector< shared_ptr<TAresta> > &ListaArestas, shared_ptr<TDesenho> drawing); 

  std::map<std::string, shared_ptr<TArestasCircuito> > _circuitEdges;//arestas de um determinado circuito, indexado pelo nome
  shared_ptr<Graph> _graph;
};                                     
//---------------------------------------------------------------------------

#endif
