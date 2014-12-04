
#include "GLCoords.h"

GLCoords::GLCoords( int clientWidth, int clientHeight )
{
	x = -1;
	y = -1;
	fator = -1;
	w = clientWidth;
	h = clientHeight;
	x2=0;
	y2=0;
	
	zoom=50 / 10000.0;
	FatorZoomX=FatorZoomY=0.0;
	oldZoom = 0;
	
	menorx = maiorx = 0.0;
	menory = maiory = 0.0;
	distX = distY = 0.0;
	intervaloX = intervaloY = 0.0;
}



void GLCoords::resize( int clientWidth, int clientHeight )
{
	w = clientWidth;
	h = clientHeight;
	if(h == 0)
		h = 1;
	xMeioTela = w / 2;
	yMeioTela = h / 2;
}



void GLCoords::mouseMove(int X, int Y)
{
    x2=X-x1;
    y2=y1-Y;
    distX = - intervaloX / w * x2;
    distY = - intervaloY / h * y2;
}



void GLCoords::mouseUp()
{
	mediax-=x2*FATOR_TELA/zoom;
	mediay-=y2*FATOR_TELA/zoom;
	maiorx += distX;
	menorx += distX;
	menory += distY;
	maiory += distY;
	x2=0;
	y2=0;
	distX = distY = 0;
}



void GLCoords::DeslocaDesenho(int X, int Y)
{
	x1=X;
	y1=Y;
}



void GLCoords::SetZoom(int Zoom)
{
	int difZoom;
	difZoom = Zoom - oldZoom;
	menorx += (FatorZoomX * difZoom) /2;
	maiorx -= (FatorZoomX * difZoom) /2;
	menory += (FatorZoomY * difZoom) /2;
	maiory -= (FatorZoomY * difZoom) /2;
	intervaloX -= (FatorZoomX * difZoom);
	intervaloY -= (FatorZoomY * difZoom);
	oldZoom = Zoom;
}



double GLCoords::getLeft()
{
	return menorx + distX;
}



double GLCoords::getRight()
{
	return menorx + distX + intervaloX;
}



double GLCoords::getBottom()
{
	return menory + distY;
}



double GLCoords::getTop()
{
	return menory + distY + intervaloY;
}



TPonto GLCoords::ConvertePonto(int X, int Y)
{
  TPonto retorno;

  retorno.x = intervaloX/w * X + menorx + distX;
  retorno.y = intervaloY/h * (h-Y) + menory + distY;

  return retorno;
}



void GLCoords::initializeLimits()
{
	mediax=0;
	mediay=0;
	menorx=Infinity;
	menory=Infinity;
	maiorx=-Infinity;
	maiory=-Infinity;
}



void GLCoords::updateLimits( double x, double y )
{
	if ( x > maiorx )
		maiorx = x;
	if ( y > maiory )
		maiory = y;
	if ( x < menorx )
		menorx = x;
	if ( y < menory)
		menory = y;
}



void GLCoords::updateMean()
{	
	mediax = (menorx + maiorx) / 2;
	mediay = (menory + maiory) / 2;
	if ((maiory-menory)>0.0001)
		fator=1.0/(maiory-menory);
	else if ((maiorx-menorx)>0.0001)
		fator=1.0/(maiorx-menorx);
	else
		fator=1;
}



double GLCoords::getWidthLimits()
{
	return maiorx - menorx;
}



double GLCoords::getHeightLimits()
{
	return maiory - menory;
}



void GLCoords::updateProportion()
{
	intervaloX = maiorx - menorx;
	intervaloY = maiory - menory;
	if ( intervaloX > intervaloY )
	{
		intervaloX *= 1.05;
		menorx -= intervaloX * 0.025;
		maiorx -= intervaloX * 0.025;
		intervaloY = intervaloX * (h / w);
		menory -= (intervaloY - (maiory - menory))/2;
		maiory -= (intervaloY - (maiory - menory))/2;
	}
	else
	{
		intervaloY *= 1.05;
		menory -= intervaloY * 0.025;
		maiory -= intervaloY * 0.025;
		intervaloX = intervaloY * (w / h);
		menorx -= (intervaloX - (maiorx - menorx))/2;
		maiorx -= (intervaloX - (maiorx - menorx))/2;
	}
	FatorZoomX = intervaloX / 500; // 500 � o tamanho do slider
	FatorZoomY = intervaloY / 500; // 500 � o tamanho do slider
	oldZoom = 0;
}
