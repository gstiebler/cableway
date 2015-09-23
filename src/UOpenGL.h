//---------------------------------------------------------------------------
#ifndef UOpenGLH
#define UOpenGLH

#include <math.h>
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
  void paintEvent(QPaintEvent *event);
  QPainter* _painter;
  QBrush _brush;
  QPen _pen;
public:
  COpenGL(int ClientWidth, int ClientHeight, QWidget *parent);
  virtual ~COpenGL();
  virtual void DrawObjects() = 0;
  void Resize(int ClientWidth, int ClientHeight);
//  void Ortho();
  
  void DesenhaArco(float x_center, float y_center, float w,
          float h, float startAngle, float endAngle );
  void DesenhaBolaFechada(float x_center, float y_center, float w,
          float h, float startAngle, float arcAngle );
  void EscreveTexto(string texto, TPonto origem, double rotacao, double FatorAltura);

  GLCoords _glCoords;
};
//---------------------------------------------------------------------------

#endif
