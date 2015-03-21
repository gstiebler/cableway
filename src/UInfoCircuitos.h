//---------------------------------------------------------------------------
#ifndef UInfoCircuitosH
#define UInfoCircuitosH
//---------------------------------------------------------------------------
#include <math.h>
#include <vector>
#include <string>
//#include <values.h>
#include <algorithm>
#include <memory>
//#include "UArmazenamentoBandeirola.h"

//#include "UGrafoDesenho.h"
//#include "UArmazenamentoCircuitos.h"
//#include "UAuxString.h"
//#include "UConfig.h"
//#include "UTCallbackStatusCarregamento.h"
#include "UCallbacks.h"
#include "UDefines.h"
#include "UArmazenamentoCircuitos.h"
#include "UTCallbackStatusCarregamento.h"
#include "UVerticesArestas.h"

class TListaArestas;
class TParamsInfoCircuitos;
class TArestasCircuito;
class TVerticesGerais;

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
    VerticesDjikstra(int numero, double Distancia);
    int n;
    double distancia;
};

//---------------------------------------------------------------------------

class CInfoCircuitos
{
private:
  void GeraInfoCircuitos(callbackStatusCarregamento &call);
  TListaCircuitos *ListaCircuitosArestas;//lista de circuitos de uma determinada aresta
  vector< shared_ptr<TAresta> > Arestas;
  static void SeparaRota(std::string ListaPontos, vector<string> *ListaRota);
  static void MergeRota(vector<std::string> &rota, vector<std::string> NovaParte);
  int NumDesenhos;
  bool dentroEquipamento;
  callbackVerificaTexto CallVT;
  double *DistanciaDjikstra;
  int *anterior;
  //bool *VerticesVisitados, *VerticesExplorados;
  int *PaisVertices, *vArestas;
public:      
  CInfoCircuitos(TParamsInfoCircuitos *ParamsInfoCircuitos, callbackStatusCarregamento &call, callbackVerificaTexto &callVT);

  ~CInfoCircuitos();

  std::vector<TArestasCircuito> ArestasDoCircuito;//arestas de um determinado circuito

  string CircuitosDaAresta(int Aresta);

  int ArestaDoPonto(TPonto ponto, TPonto &PontoNaReta, int IndiceDesenho);
  int ListaArestasDoCircuito(string circuito);
  int ListaArestasDoCircuito(int idCircuito);
  void AdicionaCircuito(TCircuito &Circuito);
  TVectorInt * ArestasCircuito(int circuito, int IndiceDesenho);
  void PontosAresta(TPonto Pontos[2], int iAresta);
  bool GeraRota(string V1, string V2, string ListaPontos, double &tam, vector<string> &rota,
              TArestasCircuito *ArestasCircuito, TVectorInt *ListaBandeirolas,
              TStringList *DEBUG_arestas, TCircuitoAreas *CircuitoAreas);
  bool GeraRota(string Destino, string Origem, double &tam, vector<string> &rota,
             TArestasCircuito *ArestasCircuito, TVectorInt *ListaBandeirolas,
             TStringList *DEBUG_arestas, string &SubRotas, TCircuitoAreas *CircuitoAreas);
  void Arvore(int Vertice, TVectorInt &ListaArestas, int IndiceDesenho);        
  int ApagaArestasDoCircuito(string circuito, int idCircuito);
  TVerticesGerais *VerticesGerais;
};                                     
//---------------------------------------------------------------------------

#endif
