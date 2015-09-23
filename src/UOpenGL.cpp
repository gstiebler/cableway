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
    DrawObjects();
    _painter->end();
	delete _painter;
}


void COpenGL::DesenhaArco(float x_center, float y_center, float w,
          float h, float startAngle, float endAngle )
{
	QRectF rectf( x_center - w / 2, y_center - h / 2, w, h );
	_painter->drawArc( QRectF( x_center - w / 2, y_center - h / 2, w, h ), startAngle, endAngle - startAngle );
}
//---------------------------------------------------------------------------


void COpenGL::DesenhaBolaFechada(float x_center, float y_center, float w,
          float h, float startAngle, float arcAngle )
{
	_painter->drawEllipse( QRectF( x_center - w / 2, y_center - h / 2, w, h ) );
}
//---------------------------------------------------------------------------

void COpenGL::EscreveTexto(string texto, TPonto origem, double rotacao, double FatorAltura)
{
	QFont textFont;
	double canvasWidth = _glCoords.getRight() - _glCoords.getLeft();
    textFont.setPixelSize( 150000 / canvasWidth );
	_painter->setFont( textFont );
	_painter->setPen( QPen(Qt::white) );
	QPointF coords( origem.x, origem.y );
	_painter->rotate( rotacao );
	_painter->drawText( coords, QString( texto.c_str() ) );
}
//---------------------------------------------------------------------------

void COpenGL::AjustaExibicao()
{
	_painter->setViewport( _glCoords.getLeft(), _glCoords.getTop(), _glCoords.getWorldWidth(), _glCoords.getWorldHeight() );
}
//---------------------------------------------------------------------------
