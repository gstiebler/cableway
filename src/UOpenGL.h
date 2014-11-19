//---------------------------------------------------------------------------
#ifndef UOpenGLH
#define UOpenGLH

//#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include "UAuxString.h"
#include "UDefines.h"


#define FATOR_TELA 0.162
#define TAMANHO 50.0f
#define RANGE 50.0
#define ZOOM_SETA 1.05
#define ZOOM_MENOR 1.025
#define ZOOM_MAIOR 1.1
#define EXP_ZOOM 1.20

//---------------------------------------------------------------------------

class COpenGL
{
protected:
  int PixelFormat;
  GLuint startoflist;
  GLfloat x, y, xstep, ystep, w, h;
  GLsizei size;
  bool apertado;
  GLfloat x1, y1, x2, y2, distX, distY;
  GLfloat zoom;
  double mediax, mediay;
  double menorx, menory, maiorx, maiory, intervaloX, intervaloY;
  int xMeioTela, yMeioTela;
  int oldZoom;
  void AjustaExibicao();
  int DEBUG;
  double FatorZoomX, FatorZoomY;
  bool XEhMaior, initialized;
public:
  COpenGL(int ClientWidth, int ClientHeight);
  virtual ~COpenGL();
  void RenderGLScene();
  void SetPixelFormatDescriptor();
  virtual void DrawObjects() = 0;
  void Resize(int ClientWidth, int ClientHeight);
//  void Ortho();

  void Paint();
  void MouseMove(int X, int Y);
  void MouseUp();
  void DeslocaDesenho(int X, int Y);
  void SetZoom(int Zoom);
  
  void DesenhaArco(float x_center, float y_center, float w,
          float h, float startAngle, float arcAngle, int n);
  void DesenhaBolaFechada(float x_center, float y_center, float w,
          float h, float startAngle, float arcAngle, int n);
  void EscreveTexto(string texto, TPonto origem, double rotacao, double FatorAltura);

  TPonto ConvertePonto(int X, int Y);
};
//---------------------------------------------------------------------------

#endif
