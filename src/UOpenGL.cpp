//---------------------------------------------------------------------------
#pragma hdrstop
#include "UOpenGL.h"

COpenGL::COpenGL(int ClientWidth, int ClientHeight, QWidget *parent) : 
        QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
		_glCoords( ClientWidth, ClientHeight )
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
    _mostraDesenho.DrawObjects();
    _painter->end();
	delete _painter;
}


void COpenGL::AjustaExibicao()
{
	_painter->setViewport( _glCoords.getLeft(), _glCoords.getTop(), _glCoords.getWorldWidth(), _glCoords.getWorldHeight() );
}
//---------------------------------------------------------------------------
