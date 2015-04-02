//---------------------------------------------------------------------------
#ifndef UGrafoDesenhoH
#define UGrafoDesenhoH
//---------------------------------------------------------------------------
#include <math.h>
//#include "UDadosDGN.h"
//#include "UDadosDWG.h"
#include "UParamsStructs.h" 
#include "UDadosGenerico.h"
//#include "UFila.h"
//#include "TInterfaceMainPar.h"
//#include "TDesenho.h"
//#include "UGeometria.h"
//#include "UTempoExec.h"
#include <vector>
#include "UVerticesArestas.h"
//#include "UErros.h"

#include <memory>

struct TDesenho;
struct TArco;
struct TMultipoint;

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

struct TPontoEIndiceCabo
{
  int IndiceCabo;
  TPonto PosVertice;
  shared_ptr<TVerticeGeral> _vertex;
  int IndiceArco;
  double Dist;
};

typedef vector<TPontoEIndiceCabo> TVectorPontoEIndiceCabo;
//---------------------------------------------------------------------------

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
  void CaboMaisProximo(TPonto &ponto, int &IndiceCabo, double &DistMaisProx, TPonto &PosVertice, int Diferente, int Nivel);
  void GeraVerticesBandeirola();
  void GeraVerticesArcos();
  void GeraVerticesPontaCabos();
  //dado um ponto (de ponta de arco) pega o cabo arco cuja ponta � a mais pr�xima ao ponto dado
  void DistPontoParaPontaCaboArco(TPonto ponto,
             int &IndiceCabo, double &DistMaisPerto, TPonto &PosVertice, shared_ptr<TVerticeGeral> &vertex, int IndiceMax,
             int &pontaCabo);
  //dada uma reta (de instrumento) pega o cabo reta cuja ponta � a mais pr�xima da reta dada
  void DistRetaParaPontaCaboReta(TPonto Reta[2],
                        int &IndiceCabo, double &DistMaisPerto, TPonto &PosVertice);
  //dada uma reta (de instrumento) pega todos os cabos reta cuja ponta está t�o pr�xima quanto a Distância m�nima da reta dada
  void DistRetaParaTodasPontasCaboReta(TPonto Reta[2],
                        TVectorPontoEIndiceCabo &ListaMenores, double DistMinElemCabo);
  //dada uma reta (de instrumento) pega o cabo arco cuja ponta � a mais pr�xima da reta dada
  void DistRetaParaPontaCaboArco(TPonto Reta[2],
                        int &IndiceCabo, double &DistMaisPerto, TPonto &PosVertice, shared_ptr<TVerticeGeral> &vertex);
  //dada uma reta (de instrumento) pega todos os cabos arco cuja ponta está t�o pr�xima quanto a Distância m�nima da reta dada
  void DistRetaParaTodasPontasCaboArco(TPonto Reta[2],
                        TVectorPontoEIndiceCabo &ListaMenores, double DistMinElemCabo);
  void DistArcoParaPontaRetaCabo(TArco &Arco, int &IndiceCabo, double &DistMaisPerto,
                                        TPonto &PosVertice);
  void DistArcoParaPontaArcoCabo(TArco &Arco, int &IndiceArcoCabo, double &DistMaisPerto,
		                                                        TPonto &PosVertice);
  void DistArcoParaTodasPontasRetaCabo(TArco &Arco, TVectorPontoEIndiceCabo &ListaMenores, double DistMinElemCabo);
  void GeraVerticesInstrumentos();
  void OrdenaVerticesRetas();
  void GeraArestas();
  void AchaTextoBandeirola(TPonto &pos, std::string &texto);
  TPonto AchaPosVerticeInstrumento(TListaItensCelula *ListaItensCelula);
  void CriaVerticesEArestasInstrumento (TListaItensCelula *ListaItensCelula, std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento, TPonto PosVertice );
  void GeraVerticesInstrumentosAdicionaMultipoint( shared_ptr<TMultipoint> multipoint, TListaItensCelula *ListaItensCelula, bool &ligado, std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento, TPonto PosVertice);
  void GeraVerticesInstrumentosAdicionaArco( shared_ptr<TArco> arc, TListaItensCelula *ListaItensCelula, bool &ligado, std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento, TPonto PosVertice);
  void GeraVerticesInstrumentosAdicionaMultipointCaboReta(TListaItensCelula *ListaItensCelula, TVectorPontoEIndiceCabo &ListaMenores, bool &ligado, std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento, TPonto PosVertice);
  void GeraVerticesInstrumentosAdicionaMultipointCaboArco(TListaItensCelula *ListaItensCelula, TVectorPontoEIndiceCabo &ListaMenores, bool &ligado, std::vector< shared_ptr<TVerticeGeral> > &VerticesInstrumento, TPonto PosVertice);

public:
  //�ndices referentes ao vetor Multipoint que cont�m CabosArco
  std::vector<CCaboArco> _cabosArco;
  //armazena informa��es das retas que compoem um cabo. Armazena
  //diversos vértices do grafo, ou seja, os outros elementos q se ligam ao cabo
  std::vector< shared_ptr<CCaboReta> > _cabosReta;
  void ChecagemVerticeDuplo(const std::vector< shared_ptr<TDesenho> > &ListaDesenhos);
  int _pri;
  int _ult;
//  TVectorPonto PontosBandeirolas[2];
  TVectorPontosBandeirola _pontosPraMostrarBandeirola;
//  int nPontosBandeirolas;
  //m�dia do raio dos cabos arcos, para se ter uma id�ia da dimensão dos elementos
  //para por exemplo ter um limiar do quanto uma ponta de cabo pode ser separada da outra
  double _mediaRaioCaboArco;
  double _distMinElemCaboPraOpenGL;
  CGrafoDesenho(TParamsGrafoDesenho &ParamsGrafoDesenho, std::shared_ptr<CDadosGenerico> Dados);
//  CGrafoDesenho(TParamsGrafoDesenho &ParamsGrafoDesenho, TInterfaceMainPar &imp);
  ~CGrafoDesenho();

  TVerticesGerais *_verticesGerais;
  vector< shared_ptr<TAresta> > _arestas;
  std::shared_ptr<CDadosGenerico> _dados;
};
//---------------------------------------------------------------------------

#endif
