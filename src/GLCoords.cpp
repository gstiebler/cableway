
#include "GLCoords.h"

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
	xViewCenter = xViewCenterOnMousePress + ( xMousePress - X ) * getScreenToWorldRatio();
	yViewCenter = yViewCenterOnMousePress + ( yMousePress - Y ) * getScreenToWorldRatio();
}




void GLCoords::incZoom( double increase )
{
	zoom *= increase;
}



void GLCoords::initializeLimits()
{
	menorx = Infinity;
	menory = Infinity;
	maiorx = -Infinity;
	maiory = -Infinity;
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
	return canvasWidth/ ( getWorldWidth() * zoom ) ;
}
