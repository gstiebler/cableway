//---------------------------------------------------------------------------
#ifndef UOpenGLH
#define UOpenGLH

//#include <GL/glu.h>
//#include <GL/gl.h>
#include <QtOpenGL>
#include <math.h>
#include "UAuxString.h"
#include "UDefines.h"
#include "GLCoords.h"

#include <QtOpenGL/QGLWidget>

//---------------------------------------------------------------------------

class COpenGL : public QGLWidget
{
protected:
  int PixelFormat;
  GLuint startoflist;
  void AjustaExibicao();
  int DEBUG;
  bool initialized;
public:
  COpenGL(int ClientWidth, int ClientHeight, QWidget *parent);
  virtual ~COpenGL();
  void RenderGLScene();
  void SetPixelFormatDescriptor();
  virtual void DrawObjects() = 0;
  void Resize(int ClientWidth, int ClientHeight);
//  void Ortho();

  void Paint();
  
  void DesenhaArco(float x_center, float y_center, float w,
          float h, float startAngle, float arcAngle, int n);
  void DesenhaBolaFechada(float x_center, float y_center, float w,
          float h, float startAngle, float arcAngle, int n);
  void EscreveTexto(string texto, TPonto origem, double rotacao, double FatorAltura);

  GLCoords _glCoords;
};
//---------------------------------------------------------------------------

#endif
