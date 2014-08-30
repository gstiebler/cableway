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

enum TTipoOrientacao {VERTICAL, HORIZONTAL};

struct CCaboArco
{
  bool ponta[2];
  int Indice;
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
  bool EhOPrimeiroPonto(TPonto ponto, vector<TMultipoint> Multipoint, int IndiceCabo);
  bool EhOUltimoPonto(TPonto ponto, vector<TMultipoint> Multipoint, int IndiceCabo);
  void AdicionaVertice(int ID, TPonto &ponto);
  int Indice;//�ndice do vetor de Multipoint
  int NumVertices;
  bool ponta[2];
  TTipoOrientacao TipoOrientacao;
  TListaVerticeReta VerticesReta;
};
//---------------------------------------------------------------------------

struct TPontoEIndiceCabo
{
  int IndiceCabo;
  TPonto PosVertice;
  int IndiceVertice;
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
  bool CarregaGrafo;
  TTipoElemento TipoElementoCor[NUM_CORES];
  void AlocaElementos();
  void GeraListaCabos();
  //dado um ponto pega o cabo reta mais pr�ximo do ponto
  void CaboMaisProximo(TPonto &ponto, int &IndiceCabo,
                  double &DistMaisProx, TPonto &PosVertice, int Diferente, int Nivel);
  void GeraVerticesBandeirola();
  void GeraVerticesArcos();
  void GeraVerticesPontaCabos();
  //dado um ponto (de ponta de arco) pega o cabo arco cuja ponta � a mais pr�xima ao ponto dado
  void DistPontoParaPontaCaboArco(TPonto ponto,
             int &IndiceCabo, double &DistMaisPerto, TPonto &PosVertice, int &IndiceVertice, int IndiceMax,
             int &pontaCabo);
  //dada uma reta (de instrumento) pega o cabo reta cuja ponta � a mais pr�xima da reta dada
  void DistRetaParaPontaCaboReta(TPonto Reta[2],
                        int &IndiceCabo, double &DistMaisPerto, TPonto &PosVertice);
  //dada uma reta (de instrumento) pega todos os cabos reta cuja ponta est� t�o pr�xima quanto a dist�ncia m�nima da reta dada
  void DistRetaParaTodasPontasCaboReta(TPonto Reta[2],
                        TVectorPontoEIndiceCabo &ListaMenores, double DistMinElemCabo);
  //dada uma reta (de instrumento) pega o cabo arco cuja ponta � a mais pr�xima da reta dada
  void DistRetaParaPontaCaboArco(TPonto Reta[2],
                        int &IndiceCabo, double &DistMaisPerto, TPonto &PosVertice, int &IndiceVertice);
  //dada uma reta (de instrumento) pega todos os cabos arco cuja ponta est� t�o pr�xima quanto a dist�ncia m�nima da reta dada
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
  bool EhCabo(TElemento &Elemento);
  bool EhInstrumento(TElemento &Elemento);
  bool EhBandeirola(TElemento &Elemento);
  //�ndices referentes ao vetor Multipoint que cont�m CabosArco
//  int *iCabosArco;
  int *iLinhasBandeirola;
  int *iCirculosBandeirola;
  int *iRetangulosInstrumento;
  int *iCirculosInstrumento;
  double AlturaTeto, AlturaInterrup;
  void AchaPonta(int &iPonto, int &iM, int &Ponta, TListaItensCelula *ListaItens, double DistMinBandeirola);
  TPonto AchaPosVerticeInstrumento(TListaItensCelula *ListaItensCelula);
  void CriaVerticesEArestasInstrumento (TListaItensCelula *ListaItensCelula, TVectorInt *iVerticesInstrumento, TPonto PosVertice, bool PrensaCabo);
  bool ligaEquipamentoSeDesligado (TListaItensCelula *ListaItensCelula, bool ligado);
  void GeraVerticesInstrumentosAdicionaMultipoint(int Indice, TListaItensCelula *ListaItensCelula, bool &ligado, TVectorInt *iVerticesInstrumento, TPonto PosVertice);
  void GeraVerticesInstrumentosAdicionaArco(int Indice, TListaItensCelula *ListaItensCelula, bool &ligado, TVectorInt *iVerticesInstrumento, TPonto PosVertice);
  void GeraVerticesInstrumentosAdicionaMultipointCaboReta(TListaItensCelula *ListaItensCelula, TVectorPontoEIndiceCabo &ListaMenores, bool &ligado, TVectorInt *iVerticesInstrumento, TPonto PosVertice);
  void GeraVerticesInstrumentosAdicionaMultipointCaboArco(TListaItensCelula *ListaItensCelula, TVectorPontoEIndiceCabo &ListaMenores, bool &ligado, TVectorInt *iVerticesInstrumento, TPonto PosVertice);
protected:

public:
  //�ndices referentes ao vetor Multipoint que cont�m CabosArco
  CCaboArco **CabosArco;
  //armazena informa��es das retas que compoem um cabo. Armazena
  //diversos v�rtices do grafo, ou seja, os outros elementos q se ligam ao cabo
  CCaboReta **CabosReta;
  void GeraColares(std::vector<void*> ListaDesenhos);
  void ChecagemVerticeDuplo(std::vector<void*> ListaDesenhos);
  int pri;
  int ult;
//  TVectorPonto PontosBandeirolas[2];
  TVectorPontosBandeirola PontosPraMostrarBandeirola;
//  int nPontosBandeirolas;
  //m�dia do raio dos cabos arcos, para se ter uma id�ia da dimens�o dos elementos
  //para por exemplo ter um limiar do quanto uma ponta de cabo pode ser separada da outra
  double MediaRaioCaboArco;
  double DistMinElemCaboPraOpenGL;
  CGrafoDesenho(TParamsGrafoDesenho &ParamsGrafoDesenho, TDadosTransfer *dadosDLL);
//  CGrafoDesenho(TParamsGrafoDesenho &ParamsGrafoDesenho, TInterfaceMainPar &imp);
  ~CGrafoDesenho();
  int NumCabosReta;
  int NumCabosArco;
  int NumLinhasBandeirola;
  int NumCirculosBandeirola;
  int NumRetanguloInstrumento;
  int NumCirculoInstrumento;

  TVerticesGerais *VerticesGerais;
  TListaArestas *Arestas;
  CDadosGenerico *Dados;
};
//---------------------------------------------------------------------------

#endif
