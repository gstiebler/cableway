//---------------------------------------------------------------------------
#ifndef UGrafoDesenhoH
#define UGrafoDesenhoH
//---------------------------------------------------------------------------
#include <math.h>
#include "UDadosGenerico.h"
#include <vector>
#include "UVerticesArestas.h"

#include <memory>

struct TDesenho;
struct TArco;
struct TMultipoint;
class Graph;

enum TTipoOrientacao {VERTICAL, HORIZONTAL};

struct CCaboArco
{
  bool ponta[2];
  shared_ptr<TArco> _arco;
  CCaboArco ()
  {
    ponta[0] = ponta[1] = false;
  }
};

//N�mero de CCaboReta = NumMultipoint
struct CCaboReta
{
public:
  CCaboReta();
  ~CCaboReta();
  bool EhOPrimeiroPonto( TPonto ponto );
  bool EhOUltimoPonto( TPonto ponto );
  void AdicionaVertice( shared_ptr<TVerticeGeral> Vertice );
  shared_ptr<TMultipoint> _multipoint;
  bool ponta[2];
  TTipoOrientacao TipoOrientacao;
  std::vector< shared_ptr< TVerticeGeral > > VerticesReta;
};
//---------------------------------------------------------------------------

struct TPontoEIndiceCaboReta
{
  shared_ptr<CCaboReta> straightCable;
  TPonto PosVertice;
  shared_ptr<TVerticeGeral> _vertex;
  double Dist;
};

typedef vector<TPontoEIndiceCaboReta> TVectorPontoEIndiceCaboReta;
//---------------------------------------------------------------------------

struct TPontoEIndiceCaboArco
{
	shared_ptr<CCaboArco> arcCable;
	TPonto PosVertice;
	shared_ptr<TVerticeGeral> _vertex;
	int IndiceArco;
};


struct TPontosBandeirola
{
  TPonto NaBandeirola;
  TPonto NoCabo;
};

typedef vector<TPontosBandeirola> TVectorPontosBandeirola;

class CGrafoDesenho
{
private:
  TTipoElemento TipoElementoCor[NUM_CORES];
  void GeraListaCabos();
  //dado um ponto pega o cabo reta mais pr�ximo do ponto
  void CaboMaisProximo(TPonto &ponto, std::shared_ptr<CCaboReta> &straightCable, double &DistMaisProx, TPonto &PosVertice, int Diferente, int Nivel) const;
  void GeraVerticesBandeirola();
  void GeraVerticesArcos();
  void GeraVerticesPontaCabos();
  //dado um ponto (de ponta de arco) pega o cabo arco cuja ponta � a mais pr�xima ao ponto dado
  void DistPontoParaPontaCaboArco(TPonto ponto, shared_ptr<CCaboArco> &arcCable, double &DistMaisPerto, TPonto &PosVertice, 
	  shared_ptr<TVerticeGeral> &vertex, int IndiceMax, int &pontaCabo) const;
  //dada uma reta (de instrumento) pega todos os cabos reta cuja ponta está t�o pr�xima quanto a Distância m�nima da reta dada
  void DistRetaParaTodasPontasCaboReta(TPonto Reta[2], TVectorPontoEIndiceCaboReta &ListaMenores, double DistMinElemCabo) const;
  //dada uma reta (de instrumento) pega todos os cabos arco cuja ponta está t�o pr�xima quanto a Distância m�nima da reta dada
  void DistRetaParaTodasPontasCaboArco(TPonto Reta[2], vector<TPontoEIndiceCaboArco> &ListaMenores, double DistMinElemCabo) const;
  void DistArcoParaTodasPontasRetaCabo(TArco &Arco, TVectorPontoEIndiceCaboReta &ListaMenores, double DistMinElemCabo) const;
  void GeraVerticesInstrumentos();
  void OrdenaVerticesRetas();
  void GeraArestas();
  void createColarEdges( TListaItensCelula *ListaItensCelula, std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento );
  TPonto AchaPosVerticeInstrumento(TListaItensCelula *ListaItensCelula) const;
  void CriaVerticesEArestasInstrumento (TListaItensCelula *ListaItensCelula, std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento, TPonto PosVertice );
  void GeraVerticesInstrumentosAdicionaMultipoint( shared_ptr<TMultipoint> multipoint, TListaItensCelula *ListaItensCelula, bool &ligado, std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento, TPonto PosVertice);
  void GeraVerticesInstrumentosAdicionaArco( shared_ptr<TArco> arc, TListaItensCelula *ListaItensCelula, bool &ligado, std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento, TPonto PosVertice);
  void GeraVerticesInstrumentosAdicionaMultipointCaboReta(TListaItensCelula *ListaItensCelula, TVectorPontoEIndiceCaboReta &ListaMenores, bool &ligado, std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento, TPonto PosVertice);
  void GeraVerticesInstrumentosAdicionaMultipointCaboArco(TListaItensCelula *ListaItensCelula, vector<TPontoEIndiceCaboArco> &ListaMenores, bool &ligado, std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento, TPonto PosVertice);

public:
  //�ndices referentes ao vetor Multipoint que cont�m CabosArco
  std::vector< shared_ptr<CCaboArco> > _cabosArco;
  //armazena informa��es das retas que compoem um cabo. Armazena
  //diversos vértices do grafo, ou seja, os outros elementos q se ligam ao cabo
  std::vector< shared_ptr<CCaboReta> > _cabosReta;
  void ChecagemVerticeDuplo(const std::vector< shared_ptr<TDesenho> > &ListaDesenhos) const;
  int _ult;
//  TVectorPonto PontosBandeirolas[2];
  TVectorPontosBandeirola _pontosPraMostrarBandeirola;
  //m�dia do raio dos cabos arcos, para se ter uma id�ia da dimensão dos elementos
  //para por exemplo ter um limiar do quanto uma ponta de cabo pode ser separada da outra
  double _mediaRaioCaboArco;
  double _distMinElemCaboPraOpenGL;
//  CGrafoDesenho(TParamsGrafoDesenho &ParamsGrafoDesenho, TInterfaceMainPar &imp);
  ~CGrafoDesenho();

  std::shared_ptr<CDadosGenerico> _dados;
  shared_ptr<Graph> _graph;
};
//---------------------------------------------------------------------------

#endif
