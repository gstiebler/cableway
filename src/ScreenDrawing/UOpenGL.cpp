//---------------------------------------------------------------------------
#pragma hdrstop
#include "UOpenGL.h"

using namespace std;

COpenGL::COpenGL(int ClientWidth, int ClientHeight, QWidget *parent, shared_ptr<CDadosGenerico> dados, 
				 shared_ptr<ElectricalElements> electricalElements, shared_ptr<CInfoCircuitos> infoCircuitos) : 
        QWidget( parent),
		_glCoords( ClientWidth, ClientHeight ),
		_mostraDesenho( dados, electricalElements, infoCircuitos, &_glCoords )
{
  static int CONTADOR=0;
  DEBUG=CONTADOR;
  CONTADOR++;
  initialized = false;
	_mostraDesenho.initializeLimits();
}
//---------------------------------------------------------------------------



void COpenGL::Resize(int ClientWidth, int ClientHeight)
{
	_glCoords.resize( ClientWidth, ClientHeight );
}
//---------------------------------------------------------------------------


void COpenGL::paintEvent(QPaintEvent *event)
{
	_painter = new QPainter();
    _painter->begin(this);
    _painter->setRenderHint(QPainter::Antialiasing);
	_painter->fillRect(event->rect(), QBrush( QColor( 0, 0, 0 ) ) );	
	_glCoords.resize( event->rect().width(), event->rect().height() );
	AjustaExibicao();//DESLOCA IMAGEM E D� ZOOM
    _mostraDesenho.DrawObjects( _painter );
    _painter->end();
	delete _painter;
	_painter = NULL;
}


void COpenGL::AjustaExibicao()
{
	QRect rectf( _glCoords.getLeft(), _glCoords.getTop(), _glCoords.getWidth(), _glCoords.getHeight() );
	_painter->setWindow( rectf );  
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