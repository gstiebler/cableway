//---------------------------------------------------------------------------
#ifndef UOpenGLH
#define UOpenGLH

#include <math.h>
#include "UDefines.h"
#include "GLCoords.h"
#include "UMostraDesenho.h"

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
  void paintEvent(QPaintEvent *event);
  QPainter* _painter;
  
  bool primeiro;   
public:
  COpenGL(int ClientWidth, int ClientHeight, QWidget *parent);
  virtual ~COpenGL();
  void Resize(int ClientWidth, int ClientHeight);
  CMostraDesenho _mostraDesenho;
//  void Ortho();

  GLCoords _glCoords;
};
//---------------------------------------------------------------------------

#endif
