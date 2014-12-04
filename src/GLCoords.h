
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

	void mouseMove(int X, int Y);

	void mouseUp();

	void DeslocaDesenho(int X, int Y);

	void SetZoom(int Zoom);

	double getLeft();
	double getRight();
	double getBottom();
	double getTop();

	void initializeLimits();
	void updateLimits( double x, double y );

	TPonto ConvertePonto(int X, int Y);

	void updateMean();
	void updateProportion();
	
	double getWidthLimits();
	double getHeightLimits();

	float x, y, xstep, ystep, w, h;
	int size;
	float x1, y1, x2, y2, distX, distY;
	float zoom;
	double fator;//usadas em MostraDesenho
	
	double mediax, mediay;
	double menorx, menory, maiorx, maiory, intervaloX, intervaloY;
	int xMeioTela, yMeioTela;
	int oldZoom;
	
	double FatorZoomX, FatorZoomY;
};