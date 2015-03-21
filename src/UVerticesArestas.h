//---------------------------------------------------------------------------

#ifndef UVerticesArestasH
#define UVerticesArestasH
//---------------------------------------------------------------------------

#include "UDefines.h"
#include <algorithm>
#include <memory>
//---------------------------------------------------------------------------

//s� � usado no CCaboReta
struct TVerticeReta
{
  int ID;
  TPonto pos;
};
//---------------------------------------------------------------------------

struct TArestasCircuito
{
  string Circuito;
  TVectorInt Arestas;
  int NumDesenhos;
  int idCircuito;
  TArestasCircuito(int numDesenhos);
  TArestasCircuito(const TArestasCircuito &cpy);
  ~TArestasCircuito();
  void Apaga();
  vector< vector<int> > ArestasDesenho;//vetor de vectors, o tamanho do vetor � o n�mero de desenhos
};
//---------------------------------------------------------------------------

struct TVerticeEAresta
{
  int Vertice;
  int Aresta;//� a aresta que liga o vértice ao pai da lista de adjac�ncia
};
//---------------------------------------------------------------------------  

struct TListaVerticesEArestas
{
    std::vector<TVerticeEAresta> list;

  TVerticeEAresta* getVerticeEAresta(int Indice);
  void AdicionaVerticeEAresta(int vertice, int aresta);
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
  int iDesenho;
  int IDArquivo;
  TTipoVertice TipoVertice;
  int IndiceOriginal;//usado somente na gera��o dos colares
  bool EhColar;
};
//---------------------------------------------------------------------------

struct TVerticesGerais
{
	TVerticesGerais();
	vector<shared_ptr<TVerticeGeral> > vertices;
  void Adiciona(TVerticeGeral &Item);
  int AchaVerticePeloTexto(string Texto);
  bool VerticeEhEquipamento(int n);
  bool VerticeEhBandeirola(int n);
  void ListaOrd( vector<shared_ptr<TVerticeGeral> > &ListaOrdenada);
  #ifdef _DEBUG_BUILDER
  void DEBUG_Grava();
  #endif
};
//---------------------------------------------------------------------------

struct TAresta
{
  int Vertice1, Vertice2;
  double Tam;
  int IndiceDesenho;
  int IDArquivo;
  TAresta();
  void AdicionaVertices(int v1, int v2, double dist);
};
struct TArestaReduzida
{
  int Vertice1, Vertice2;
  double Tam;
  int IndiceDesenho;
  int IDArquivo;
  vector<int> ArestasRetiradas;
  TArestaReduzida () 
  { 
	  limpa(); 
  }

  TArestaReduzida(const TArestaReduzida &cpy);

  void limpa()
  {
    Vertice1 = Vertice2 = -1;
    Tam = 0;
    IndiceDesenho = IDArquivo = I_DESENHO_NULO;

	ArestasRetiradas.clear();
  }
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

bool ComparaTVerticeGeral(shared_ptr<TVerticeGeral> Item1, shared_ptr<TVerticeGeral> Item2);

#endif


