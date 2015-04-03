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

struct TListaCircuitos
{
  int NumCircuitos;
//  TStringList *Circuitos;
  std::vector<std::string> *Circuitos;
  void AdicionaCircuito(std::string Circuito);
  TListaCircuitos();                           
  ~TListaCircuitos();
};
//---------------------------------------------------------------------------

struct TCircuitoAreas
{
  int IDArquivo;
  bool ativo;
  vector<std::string> rota;
  TCircuitoAreas()
  {
    ativo = false;
    IDArquivo = I_DESENHO_NULO;
  }
};
//---------------------------------------------------------------------------

struct TListaCircuitosBandeirola: public TListaCircuitos
{
  string Bandeirola;
};
//---------------------------------------------------------------------------

class VerticesDjikstra
{
  public:
    bool operator<(const VerticesDjikstra&) const; 
    bool operator>(const VerticesDjikstra&) const;
    VerticesDjikstra(shared_ptr<TVerticeGeral> vertex, double Distancia);
    shared_ptr<TVerticeGeral> _vertex;
    double distancia;
};

//---------------------------------------------------------------------------

class CInfoCircuitos
{
private:
  static void SeparaRota(std::string ListaPontos, vector<string> *ListaRota);
  static void MergeRota(vector<std::string> &rota, vector<std::string> NovaParte);
  set<string> getLevelsFromVertex( shared_ptr<TVerticeGeral> vertexIndex );
public:      
  CInfoCircuitos( shared_ptr<Graph> graph );

  ~CInfoCircuitos();

  std::map<std::string, shared_ptr<TArestasCircuito> > ArestasDoCircuito;//arestas de um determinado circuito, indexado pelo nome

  int ArestaDoPonto(TPonto ponto, TPonto &PontoNaReta, shared_ptr<TDesenho> drawing);
  void AdicionaCircuito(TCircuito &Circuito, int numDrawings);
  vector< shared_ptr<TAresta> >& ArestasCircuito( std::string circuitName, shared_ptr<TDesenho> drawing);
  void PontosAresta(TPonto Pontos[2], shared_ptr<TAresta> Aresta);
  bool GeraRota(string V1, string V2, string ListaPontos, double &tam, vector<string> &rota,
              shared_ptr<TArestasCircuito> ArestasCircuito, vector< shared_ptr<TVerticeGeral> > &ListaBandeirolas,
              TStringList *DEBUG_arestas, TCircuitoAreas *CircuitoAreas);
  bool GeraRota(string Destino, string Origem, double &tam, vector<string> &rota,
             shared_ptr<TArestasCircuito> ArestasCircuito, vector< shared_ptr<TVerticeGeral> > &ListaBandeirolas,
             string &SubRotas);
  bool generateDistanceTree( shared_ptr<TVerticeGeral> vertice[2], vector< shared_ptr<TVerticeGeral> > &anterior, vector< shared_ptr<TAresta> > &vArestas, string layer );
  void Arvore( shared_ptr<TVerticeGeral> Vertice, std::vector< shared_ptr<TAresta> > &ListaArestas, shared_ptr<TDesenho> drawing); 
  shared_ptr<Graph> _graph;
};                                     
//---------------------------------------------------------------------------

#endif
