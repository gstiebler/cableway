//---------------------------------------------------------------------------
#ifndef UMostraDesenhoH
#define UMostraDesenhoH

#include "UGeometria.h" 
#include <QPainter>
#include <QBrush>
#include <QPen>
#include "GLCoords.h"

class CDadosGenerico;
class CInfoCircuitos;
class ElectricalElements;
struct TArestasCircuito;

#define FATOR_FATORES 500
#define FATOR_TEXTO_NUM_VERTICES 0.0005
//---------------------------------------------------------------------------


#define CORCABO 0xFF0000 // Vermelho
#define CORINSTRUMENTO 0x0000FF // Azul
#define CORBANDEIROLA 0x00FF00 // Verde
#define CORINSTRUMENTODESCON 0xFF00FF // Rosa
#define CORTAG 0xFFFF00 // Amarelo
#define CORCAMINHO 0x00FFFF
#define CORNADA 0x999999 // Cinza
#define CORARVORE 0xFF7F00 // Ab�bora
#define CORARVORE2 0xB200B2 // Roxo

class CMostraDesenho 
{
private:
  bool ExibirCircuito;
  std::shared_ptr<TArestasCircuito> _arestasCircuito;
  std::shared_ptr<CInfoCircuitos> InfoCircuitos;
  bool bMostraNumVerticesDEBUG;
  bool bMostraArvore;
  bool bMostraArvore2;
  bool bMostraBola;
  std::shared_ptr<TVerticeGeral> VerticeArvore;
  std::shared_ptr<TVerticeGeral> VerticeArvore2;
  bool destacaCoresDeEquipamentos;
  bool facilitarVerBandeirola;
  bool mostraLigacaoEquipamento;
  bool MostrarPontasDeCaboDescon;
  double xBola, yBola, tamBola;
  void setColorFromLevel( int level );
  void setColor( unsigned char r, unsigned char g, unsigned char b );
  
  
  void DesenhaArco(float x_center, float y_center, float w,
          float h, float startAngle, float endAngle );
  void DesenhaBolaFechada(float x_center, float y_center, float w,
          float h, float startAngle, float arcAngle );
  void EscreveTexto(std::string texto, TPonto origem, double rotacao, double FatorAltura);  
  void drawMultipoints();
  void drawArcs();
  void showCircuit();
  void showTree();
  template<class T>
  void showDisconnectedCircuitEndings( std::vector< std::shared_ptr<T> > &geometricEdges );
  void showDisconnectedCircuitEndings();
  void showBandeirolaEndings();
  void drawTexts();
  QPainter* _painter;
  GLCoords *_glCoords;
  QBrush _brush;
  QPen _pen;
  std::shared_ptr<CDadosGenerico> _dados;
  std::shared_ptr<ElectricalElements> _electricalElements;

protected:
  bool semCores;   
public:
  CMostraDesenho( std::shared_ptr<CDadosGenerico> dados, std::shared_ptr<ElectricalElements> electricalElements, 
	  std::shared_ptr<CInfoCircuitos> infoCircuitos, GLCoords *glCoords);
  ~CMostraDesenho();
  void MostraCircuito(std::shared_ptr<TArestasCircuito> arestasCircuito);
  void ApagaCircuito();
  void MostraNumVerticesDEBUG(bool mostra);
  void MostraArvore(std::shared_ptr<TVerticeGeral> vertice);
  void MostraDoubleArvore(std::shared_ptr<TVerticeGeral> vertice, std::shared_ptr<TVerticeGeral> vertice2);
  void SetDestacaCores(bool DestacaCores);
  void SetDestacaBandeirolas(bool facilita);
  void SetMostraChegaEquip(bool MostraEquip);
  void SetMostrarPontasDeCaboDescon(bool mostraPontas);
  void initializeLimits();

  void DrawObjects( QPainter *painter );
};
//---------------------------------------------------------------------------

#endif
