//---------------------------------------------------------------------------

#ifndef UVerticesArestasH
#define UVerticesArestasH
//---------------------------------------------------------------------------

#include "UDefines.h"
#include <algorithm>
#include <memory>
#include <map>

//---------------------------------------------------------------------------

struct TVerticeGeral;
struct TAresta;
struct TDesenho;

struct TArestasCircuito
{
  string Circuito;
  vector< shared_ptr<TAresta> > Arestas;
  int idCircuito;
  TArestasCircuito();
  TArestasCircuito(const TArestasCircuito &cpy);
  ~TArestasCircuito();
  void Apaga();
  map< TDesenho*, vector< shared_ptr<TAresta> > > ArestasDesenho;//vetor de vectors, o tamanho do vetor � o n�mero de desenhos
};
//----------------------------------------------------------------------------  

struct TVerticeEAresta;
struct TAresta;

struct TListaVerticesEArestas
{
    std::vector<TVerticeEAresta> list;

  TVerticeEAresta* getVerticeEAresta(int Indice);
  void AdicionaVerticeEAresta(shared_ptr<TVerticeGeral> vertice, shared_ptr<TAresta> aresta);
  TListaVerticesEArestas(){}
  ~TListaVerticesEArestas() {}
  TListaVerticesEArestas(const TListaVerticesEArestas &cpy);
};
//---------------------------------------------------------------------------

struct TVerticeGeral
{
  TVerticeGeral();
  ~TVerticeGeral();
  TVerticeGeral(const TVerticeGeral &cpy);
  TPonto pos;
  int TipoElemento;
  TListaVerticesEArestas *ListaVerticesEArestas;
  double dist;//altura 3d, pouco usado
  string texto;
  shared_ptr<TDesenho> drawing;
  TTipoVertice TipoVertice;
  int IndiceOriginal;//usado somente na gera��o dos colares
  bool EhColar;
};
//---------------------------------------------------------------------------

struct TAresta;

struct TVerticeEAresta
{
	shared_ptr<TVerticeGeral> Vertice;
    shared_ptr<TAresta> Aresta;//� a aresta que liga o vértice ao pai da lista de adjac�ncia
};
//--------------------------------------------------------------------------

struct TVerticesGerais
{
	TVerticesGerais();
	vector<shared_ptr<TVerticeGeral> > vertices;
  void Adiciona(shared_ptr<TVerticeGeral> Item);
  shared_ptr<TVerticeGeral> AchaVerticePeloTexto(string Texto);
  bool VerticeEhEquipamento(int n);
  bool VerticeEhBandeirola(int n);
  void ListaOrd( vector<shared_ptr<TVerticeGeral> > &ListaOrdenada);
};
//---------------------------------------------------------------------------

struct TAresta
{
	shared_ptr<TVerticeGeral> _vertices[2];
  double Tam;
	shared_ptr<TDesenho> _drawing;
  string _layer;
  TAresta( string layer );
  void AdicionaVertices( shared_ptr<TVerticeGeral> v1, shared_ptr<TVerticeGeral> v2, double dist);
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

bool ComparaTVerticeGeral(shared_ptr<TVerticeGeral> Item1, shared_ptr<TVerticeGeral> Item2);

#endif


