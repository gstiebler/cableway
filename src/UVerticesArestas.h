//---------------------------------------------------------------------------

#ifndef UVerticesArestasH
#define UVerticesArestasH
//---------------------------------------------------------------------------

#include "UDefines.h"
#include "UGeometria.h"
#include <algorithm>
#include <memory>
#include <map>
#include <string>

//---------------------------------------------------------------------------

struct TVerticeGeral;
struct TAresta;
struct TDesenho;

struct TArestasCircuito
{
	void calcArestasDesenho();

  std::vector< std::shared_ptr<TAresta> > Arestas;
  std::map< TDesenho*, std::vector< std::shared_ptr<TAresta> > > ArestasDesenho;//vetor de vectors, o tamanho do vetor � o n�mero de desenhos
};
//----------------------------------------------------------------------------  

struct TVerticeEAresta;
struct TAresta;

struct TListaVerticesEArestas
{
	TVerticeEAresta* getVerticeEAresta(int Indice);
	void AdicionaVerticeEAresta(std::shared_ptr<TVerticeGeral> vertice, std::shared_ptr<TAresta> aresta);
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
	void removeEdges();

	TPonto pos;
	TTipoElemento TipoElemento;
	std::shared_ptr<TListaVerticesEArestas> ListaVerticesEArestas;
	std::string texto;
	std::shared_ptr<TDesenho> drawing;
	TTipoVertice TipoVertice;
	int IndiceOriginal;//usado somente na gera��o dos colares
	bool EhColar;
	static int counter;
	int _autogenId;
	std::string _layer;
};
//---------------------------------------------------------------------------

struct TAresta;

struct TVerticeEAresta
{
	std::shared_ptr<TVerticeGeral> Vertice;
    std::shared_ptr<TAresta> Aresta;//� a aresta que liga o vértice ao pai da lista de adjac�ncia
};
//--------------------------------------------------------------------------

struct TVerticesGerais
{
  void Adiciona(std::shared_ptr<TVerticeGeral> Item);
  std::shared_ptr<TVerticeGeral> AchaVerticePeloTexto(std::string Texto);
  void ListaOrd( std::vector<std::shared_ptr<TVerticeGeral> > &ListaOrdenada);

	std::vector<std::shared_ptr<TVerticeGeral> > vertices;
};
//---------------------------------------------------------------------------

struct TAresta
{	
	TAresta( std::string layer );
	TAresta( TAresta *other );
	void AdicionaVertices( std::shared_ptr<TVerticeGeral> v1, std::shared_ptr<TVerticeGeral> v2, double dist);
	std::shared_ptr<TVerticeGeral> otherVertex( TVerticeGeral *vertex );
	static void createAdjancency( std::shared_ptr<TAresta> edge );

	static int counter;
	int _autogenId;
	std::shared_ptr<TVerticeGeral> _vertices[2];
	double Tam;
	std::shared_ptr<TDesenho> _drawing;
	std::string _layer;
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

bool ComparaTVerticeGeral(std::shared_ptr<TVerticeGeral> Item1, std::shared_ptr<TVerticeGeral> Item2);

#endif


