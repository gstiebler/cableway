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
  vector< shared_ptr<TAresta> > Arestas;
  TArestasCircuito();
  TArestasCircuito(const TArestasCircuito &cpy);
  ~TArestasCircuito();
  map< TDesenho*, vector< shared_ptr<TAresta> > > ArestasDesenho;//vetor de vectors, o tamanho do vetor � o n�mero de desenhos
	void calcArestasDesenho();
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
	static int counter;
	int _autogenId;
  TVerticeGeral();
  TPonto pos;
  int TipoElemento;
  shared_ptr<TListaVerticesEArestas> ListaVerticesEArestas;
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
	vector<shared_ptr<TVerticeGeral> > vertices;
  void Adiciona(shared_ptr<TVerticeGeral> Item);
  shared_ptr<TVerticeGeral> AchaVerticePeloTexto(string Texto);
  void ListaOrd( vector<shared_ptr<TVerticeGeral> > &ListaOrdenada);
};
//---------------------------------------------------------------------------

struct TAresta
{
	static int counter;
	int _autogenId;
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


