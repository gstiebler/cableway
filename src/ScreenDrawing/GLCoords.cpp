
#include "GLCoords.h"
#include <limits>

GLCoords::GLCoords( int clientWidth, int clientHeight ) :
	xMousePress( -1 ),
	yMousePress( -1 ),
	zoom( 0.9 ),
	canvasWidth( clientWidth ),
	canvasHeight( clientHeight )
{
	initializeLimits();
}



void GLCoords::resize( int clientWidth, int clientHeight )
{
	canvasWidth = clientWidth;
	canvasHeight = clientHeight;

	if(canvasHeight == 0)
		canvasHeight = 1;
}



void GLCoords::mousePress(int X, int Y)
{
	xMousePress = X;
	yMousePress = Y;
	xViewCenterOnMousePress = xViewCenter;
	yViewCenterOnMousePress = yViewCenter;
}



void GLCoords::mouseMove(int X, int Y)
{
	int deltaXScreen = xMousePress - X;
	int deltaYScreen = yMousePress - Y;
	double deltaX = deltaXScreen / getScreenToWorldRatio();
	double deltaY = deltaYScreen / getScreenToWorldRatio();
	xViewCenter = xViewCenterOnMousePress + deltaX;
	yViewCenter = yViewCenterOnMousePress + deltaY;
	//printf( "deltaXScreen %d, deltaX %f, ratio %f\n", deltaXScreen, deltaX, getScreenToWorldRatio() );
}




void GLCoords::incZoom( double increase )
{
	zoom *= increase;
}



void GLCoords::initializeLimits()
{
	menorx = std::numeric_limits<double>::max();
	menory = std::numeric_limits<double>::max();
	maiorx = -std::numeric_limits<double>::max();
	maiory = -std::numeric_limits<double>::max();
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



void GLCoords::updateProportion()
{
	xViewCenter = menorx + getWorldWidth() / 2;
	yViewCenter = menory + getWorldHeight() / 2;
}



double GLCoords::getWorldWidth() const
{
	return maiorx - menorx;
}



double GLCoords::getWorldHeight() const
{
	return maiory - menory;
}



double GLCoords::getWidth() const
{
	return (maiorx - menorx) / zoom;
}



double GLCoords::getHeight() const
{
	return (maiory - menory) / zoom;
}



double GLCoords::getLeft() const
{
	return xViewCenter - getWorldWidth() / ( 2 * zoom );
}



double GLCoords::getRight() const
{
	return xViewCenter + getWorldWidth() / ( 2 * zoom );
}



double GLCoords::getBottom() const
{
	return yViewCenter + getWorldHeight() / ( 2 * zoom );
}



double GLCoords::getTop() const
{
	return yViewCenter - getWorldHeight() / ( 2 * zoom );
}



double GLCoords::getScreenToWorldRatio() const
{
	return canvasWidth / ( getRight() - getLeft() ) ;
}
