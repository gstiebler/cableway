//---------------------------------------------------------------------------
#ifndef UOpenGLH
#define UOpenGLH

#include <math.h>
#include "UDefines.h"
#include "GLCoords.h"
#include "UMostraDesenho.h"

#include <QtOpenGL/QGLWidget>
#include <QMouseEvent>

class CDadosGenerico;
class CInfoCircuitos;

//---------------------------------------------------------------------------

class COpenGL : public QWidget
{
protected:
  int PixelFormat;
  GLuint startoflist;
  void AjustaExibicao();
  int DEBUG;
  bool initialized;
  void paintEvent(QPaintEvent *event);
  void mousePressEvent( QMouseEvent *event );
  void mouseMoveEvent( QMouseEvent *event );
  void wheelEvent(QWheelEvent * event);
  QPainter* _painter;
public:
	COpenGL(int ClientWidth, int ClientHeight, QWidget *parent, std::shared_ptr<CDadosGenerico> dados, 
		std::shared_ptr<ElectricalElements> electricalElements, std::shared_ptr<CInfoCircuitos> infoCircuitos );
  void Resize(int ClientWidth, int ClientHeight);
  GLCoords _glCoords;
  CMostraDesenho _mostraDesenho;

};
//---------------------------------------------------------------------------

#endif
