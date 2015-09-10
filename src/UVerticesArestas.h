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
  TArestasCircuito();
  TArestasCircuito(const TArestasCircuito &cpy);
  ~TArestasCircuito();
	void calcArestasDesenho();

  vector< shared_ptr<TAresta> > Arestas;
  map< TDesenho*, vector< shared_ptr<TAresta> > > ArestasDesenho;//vetor de vectors, o tamanho do vetor � o n�mero de desenhos
};
//----------------------------------------------------------------------------  

struct TVerticeEAresta;
struct TAresta;

struct TListaVerticesEArestas
{
	TVerticeEAresta* getVerticeEAresta(int Indice);
	void AdicionaVerticeEAresta(shared_ptr<TVerticeGeral> vertice, shared_ptr<TAresta> aresta);
	TListaVerticesEArestas(){}
	~TListaVerticesEArestas() {}
	TListaVerticesEArestas(const TListaVerticesEArestas &cpy);
	void removeEdge( TVerticeGeral *vertex );

    std::vector<TVerticeEAresta> list;
};
//---------------------------------------------------------------------------

struct TVerticeGeral
{
	TVerticeGeral();
	TVerticeGeral( TVerticeGeral &other );
	void removeEdges();

	TPonto pos;
	int TipoElemento;
	shared_ptr<TListaVerticesEArestas> ListaVerticesEArestas;
	string texto;
	shared_ptr<TDesenho> drawing;
	TTipoVertice TipoVertice;
	int IndiceOriginal;//usado somente na gera��o dos colares
	bool EhColar;
	static int counter;
	int _autogenId;
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
  void Adiciona(shared_ptr<TVerticeGeral> Item);
  shared_ptr<TVerticeGeral> AchaVerticePeloTexto(string Texto);
  void ListaOrd( vector<shared_ptr<TVerticeGeral> > &ListaOrdenada);

	vector<shared_ptr<TVerticeGeral> > vertices;
};
//---------------------------------------------------------------------------

struct TAresta
{	
	TAresta( string layer );
	TAresta( TAresta *other );
	void AdicionaVertices( shared_ptr<TVerticeGeral> v1, shared_ptr<TVerticeGeral> v2, double dist);
	std::shared_ptr<TVerticeGeral> otherVertex( TVerticeGeral *vertex );
	static void createAdjancency( shared_ptr<TAresta> edge );

	static int counter;
	int _autogenId;
	shared_ptr<TVerticeGeral> _vertices[2];
	double Tam;
	shared_ptr<TDesenho> _drawing;
	string _layer;
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

bool ComparaTVerticeGeral(shared_ptr<TVerticeGeral> Item1, shared_ptr<TVerticeGeral> Item2);

#endif


