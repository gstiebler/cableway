//---------------------------------------------------------------------------
#ifndef UMostraDesenhoH
#define UMostraDesenhoH

#include "UGeometria.h" 
#include <QPainter>
#include <QBrush>
#include <QPen>
#include "GLCoords.h"

class CGrafoDesenho;
class CInfoCircuitos;

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
  shared_ptr<CGrafoDesenho> GrafoDesenho;
  shared_ptr<CInfoCircuitos> InfoCircuitos;
  bool bMostraNumVerticesDEBUG;
  bool bMostraArvore;
  bool bMostraArvore2;
  bool bMostraBola;
  shared_ptr<TVerticeGeral> VerticeArvore;
  shared_ptr<TVerticeGeral> VerticeArvore2;
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
  void EscreveTexto(string texto, TPonto origem, double rotacao, double FatorAltura);  
  void drawMultipoints();
  void drawArcs();
  void showCircuit();
  void showTree();
  void showDisconnectedCircuitEndings();
  void showBandeirolaEndings();
  void drawTexts();
  QPainter* _painter;
  GLCoords *_glCoords;
  QBrush _brush;
  QPen _pen;

protected:
  bool semCores;   
public:
  CMostraDesenho(shared_ptr<CGrafoDesenho> grafoDesenho, shared_ptr<CInfoCircuitos> infoCircuitos, GLCoords *glCoords);
  ~CMostraDesenho();
  void MostraCircuito(std::shared_ptr<TArestasCircuito> arestasCircuito);
  void ApagaCircuito();
  void MostraNumVerticesDEBUG(bool mostra);
  void MostraArvore(shared_ptr<TVerticeGeral> vertice);
  void MostraDoubleArvore(shared_ptr<TVerticeGeral> vertice, shared_ptr<TVerticeGeral> vertice2);
  void SetDestacaCores(bool DestacaCores);
  void SetDestacaBandeirolas(bool facilita);
  void SetMostraChegaEquip(bool MostraEquip);
  void SetMostrarPontasDeCaboDescon(bool mostraPontas);
  void initializeLimits();

  void DrawObjects();
};
//---------------------------------------------------------------------------

float GeraCor()
{
#define LUM_MIN 0.2
#define COMPLEMENTO (1.0-LUM_MIN)/50.0
	return LUM_MIN + 25 * COMPLEMENTO;
}

#endif
