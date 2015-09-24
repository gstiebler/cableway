//---------------------------------------------------------------------------
#pragma hdrstop
#include "UOpenGL.h"

COpenGL::COpenGL(int ClientWidth, int ClientHeight, QWidget *parent, shared_ptr<CGrafoDesenho> grafoDesenho, shared_ptr<CInfoCircuitos> infoCircuitos) : 
        QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
		_glCoords( ClientWidth, ClientHeight ),
		_mostraDesenho( grafoDesenho, infoCircuitos, &_glCoords )
{
  static int CONTADOR=0;
  DEBUG=CONTADOR;
  CONTADOR++;
  initialized = false;
	primeiro=true;
}
//---------------------------------------------------------------------------

COpenGL::~COpenGL()
{
}
//---------------------------------------------------------------------------



void COpenGL::Resize(int ClientWidth, int ClientHeight)
{
	_glCoords.resize( ClientWidth, ClientHeight );
	_painter->setViewport(0, 0, _glCoords.canvasWidth, _glCoords.canvasHeight);
	AjustaExibicao();
}
//---------------------------------------------------------------------------


void COpenGL::paintEvent(QPaintEvent *event)
{
	_painter = new QPainter();
    _painter->begin(this);
    _painter->setRenderHint(QPainter::Antialiasing);	
	if (primeiro && !initialized)
	{
		_mostraDesenho.initializeLimits();
		initialized = true;
	}
	AjustaExibicao();//DESLOCA IMAGEM E D� ZOOM
    _mostraDesenho.DrawObjects();
    _painter->end();
	delete _painter;
}


void COpenGL::AjustaExibicao()
{
	_painter->setViewport( _glCoords.getLeft(), _glCoords.getTop(), _glCoords.getWorldWidth(), _glCoords.getWorldHeight() );
}
//---------------------------------------------------------------------------




void COpenGL::mousePressEvent( QMouseEvent *event )
{
	_glCoords.mousePress( event->x(), event->y() );
}



void COpenGL::mouseMoveEvent( QMouseEvent *event )
{
	_glCoords.mouseMove( event->x(), event->y() );
	repaint();
}



void COpenGL::wheelEvent(QWheelEvent * event)
{
	double increase = 1.0 + (event->angleDelta().y() / 1200.0);
	_glCoords.incZoom( increase );
	repaint();
}