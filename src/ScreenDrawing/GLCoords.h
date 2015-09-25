
#ifndef __GL_COORDS__
#define __GL_COORDS__

#include "UDefines.h"

#define FATOR_TELA 0.162
#define RANGE 50.0
#define ZOOM_SETA 1.05
#define ZOOM_MENOR 1.025
#define ZOOM_MAIOR 1.1
#define EXP_ZOOM 1.20

class GLCoords
{
public:

	GLCoords( int clientWidth, int clientHeight );

	void resize( int clientWidth, int clientHeight );
	
	void mousePress(int X, int Y);
	void mouseMove(int X, int Y);

	void incZoom( double increase );

	void updateProportion();
	void initializeLimits();
	void updateLimits( double x, double y );

	double getLeft() const;
	double getRight() const;
	double getBottom() const;
	double getTop() const;

	double getScreenToWorldRatio() const;
	
	double getWorldWidth() const;
	double getWorldHeight() const;
	
	double getWidth() const;
	double getHeight() const;

	int xMousePress, yMousePress;
	double xViewCenterOnMousePress, yViewCenterOnMousePress;
	double xViewCenter, yViewCenter;
	int canvasWidth, canvasHeight;
	float zoom;
	
	double menorx, menory, maiorx, maiory;
};

#endif