//---------------------------------------------------------------------------
#ifndef UMostraDesenhoH
#define UMostraDesenhoH

#include "UOpenGL.h"
#include "UInfoCircuitos.h"
#include "UGeometria.h" 
//#include "uglyfont.h"
#include <math.h>
#include <time.h>

#include <QtOpenGL/QGLWidget>

class CGrafoDesenho;

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

class CMostraDesenho: public COpenGL, public QGLWidget
{
private:

    void paintGL();

  bool ExibirCircuito;
  int CircuitoAExibir;
  double x, y, fator;//usadas em MostraDesenho
  CGrafoDesenho *GrafoDesenho;
  CInfoCircuitos *InfoCircuitos;
  bool bMostraNumVerticesDEBUG;
  bool bMostraArvore;
  bool bMostraArvore2;
  bool bMostraBola;
  int VerticeArvore;
  int VerticeArvore2;
  bool destacaCoresDeEquipamentos;
  bool facilitarVerBandeirola;
  bool mostraLigacaoEquipamento;
  bool MostrarPontasDeCaboDescon;
  double xBola, yBola, tamBola;
protected:
  void DrawObjects();
  bool semCores;
  bool primeiro;      
public:
  CMostraDesenho(CGrafoDesenho *grafoDesenho, CInfoCircuitos *infoCircuitos, QWidget *parent = 0);
  ~CMostraDesenho();
  void MostraCircuito(int Circuito);
  void ApagaCircuito();
  void MostraNumVerticesDEBUG(bool mostra);
  void MostraArvore(int vertice);
  void MostraDoubleArvore(int vertice, int vertice2);
  void MostraBola(double x, double y, double tam);
  void SetDestacaCores(bool DestacaCores);
  void SetDestacaBandeirolas(bool facilita);
  void SetMostraChegaEquip(bool MostraEquip);
  void SetMostrarPontasDeCaboDescon(bool mostraPontas);
  void initializeGL();
  void resizeGL(int width, int height);
  void initializeLimits();
  void drawMultipoints();
  void drawArcs();
  void showCircuit();
  void showTree();
  void showDisconnectedCircuitEndings();
  void showBandeirolaEndings();
  void drawTexts();
};
//---------------------------------------------------------------------------

GLfloat inline GeraCor()
{
#define LUM_MIN 0.2
#define COMPLEMENTO (1.0-LUM_MIN)/50.0
	return LUM_MIN + 25 * COMPLEMENTO;
}

#endif
