//---------------------------------------------------------------------------

#ifndef UVerticesArestasH
#define UVerticesArestasH
//---------------------------------------------------------------------------

#include "UListaV.h"
#include "UDefines.h"
#include <algorithm>
//---------------------------------------------------------------------------

//só é usado no CCaboReta
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
  TVectorInt *ArestasDesenho;//vetor de vectors, o tamanho do vetor é o número de desenhos
};
//---------------------------------------------------------------------------

struct TListaArestasCircuito: public TListaV<TArestasCircuito>
{
  void Adicionar(int NumDesenhos);
};
//---------------------------------------------------------------------------

struct TVerticeEAresta
{
  int Vertice;
  int Aresta;//é a aresta que liga o vértice ao pai da lista de adjacência
};
//---------------------------------------------------------------------------  

struct TListaVerticesEArestas: public TListaV<TVerticeEAresta>
{                                                   
  TVerticeEAresta* getVerticeEAresta(int Indice);
  void AdicionaVerticeEAresta(int vertice, int aresta);
  TListaVerticesEArestas(){}
  ~TListaVerticesEArestas() {}
  TListaVerticesEArestas(const TListaVerticesEArestas &cpy)
  {
    for ( int i = 0 ; i < cpy.Tamanho() ; i++) //cpy->lista->Count ; i++ )
    {
      TVerticeEAresta *temp = new TVerticeEAresta(*cpy.getItem(i));
      Adiciona(temp);
      delete temp;
    }
  }
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
  int IndiceOriginal;//usado somente na geração dos colares
  bool EhPrensaCabo;
  bool EhColar;
};
//---------------------------------------------------------------------------

struct TVerticesGerais: public TListaV<TVerticeGeral>
{
	TVerticesGerais();

  void Adiciona(TVerticeGeral &Item);
  int AchaVerticePeloTexto(string Texto);
  bool VerticeEhEquipamento(int n);
  bool VerticeEhBandeirola(int n);
  void ListaOrd(vector<TVerticeGeral*> *ListaOrdenada);
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
  vector<int> *ArestasRetiradas;
  TArestaReduzida () { ArestasRetiradas = 0; limpa(); }
  TArestaReduzida (TArestaReduzida &cpy)
  {
  Vertice1 = cpy.Vertice1;
  Vertice2 = cpy.Vertice2;
  Tam = cpy.Tam;
  IndiceDesenho = cpy.IndiceDesenho;
  IDArquivo = cpy.IDArquivo;
  ArestasRetiradas = new vector<int>(*cpy.ArestasRetiradas);
  }
  ~TArestaReduzida () { if ( ArestasRetiradas ) delete ArestasRetiradas; ArestasRetiradas = 0; }
  void limpa()
  {
    Vertice1 = Vertice2 = -1;
    Tam = 0;
    IndiceDesenho = IDArquivo = I_DESENHO_NULO;
    if ( ArestasRetiradas )
      delete ArestasRetiradas;
    ArestasRetiradas = new vector<int>;
  }
};
//---------------------------------------------------------------------------

struct TListaArestasReduzidas: public TListaV<TArestaReduzida> {};

struct TListaArestas: public TListaV<TAresta> {};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

bool ComparaTVerticeGeral(TVerticeGeral * Item1, TVerticeGeral * Item2);

#endif


