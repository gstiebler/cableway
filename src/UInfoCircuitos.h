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
  static void SeparaRota(std::string ListaPontos, std::vector<std::string> &ListaRota);
  static void MergeRota(std::vector<std::string> &rota, std::vector<std::string> NovaParte);
  std::set<std::string> getLevelsFromVertex( std::shared_ptr<TVerticeGeral> vertexIndex ) const;

public:      
  CInfoCircuitos( std::shared_ptr<Graph> graph );
  ~CInfoCircuitos();
  void AdicionaCircuito(TCircuito &Circuito);
  static void PontosAresta(TPonto Pontos[2], std::shared_ptr<TAresta> Aresta);
  std::vector< std::shared_ptr<TAresta> >& ArestasCircuito( std::string circuitName, std::shared_ptr<TDesenho> drawing) const;
  bool GeraRota(std::string V1, std::string V2, std::string ListaPontos, double &tam, std::vector<std::string> &rota, std::shared_ptr<TArestasCircuito> ArestasCircuito) const;
  bool GeraRota(std::string Destino, std::string Origem, double &tam, std::vector<std::string> &rota, std::vector< std::shared_ptr<TAresta> > &ListaArestas) const;
  void Arvore( std::shared_ptr<TVerticeGeral> Vertice, std::vector< std::shared_ptr<TAresta> > &ListaArestas, std::shared_ptr<TDesenho> drawing) const; 

  std::map<std::string, std::shared_ptr<TArestasCircuito> > _circuitEdges;//arestas de um determinado circuito, indexado pelo nome
  std::shared_ptr<Graph> _graph;
};                                     
//---------------------------------------------------------------------------

#endif
