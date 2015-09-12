//---------------------------------------------------------------------------


#pragma hdrstop

#include "UDadosGenerico.h"

#include <string.h>


//Função pointInPolygon:
//  Globals which should be set before calling this function:
//
//  int    polySides  =  how many corners the polygon has
//  float  polyX[]    =  horizontal coordinates of corners
//  float  polyY[]    =  vertical coordinates of corners
//  float  x, y       =  point to be tested
//
//  (Globals are used in this example for purposes of speed.  Change as
//  desired.)
//
//  The function will return YES if the point x,y is inside the polygon, or
//  NO if it is not.  If the point is exactly on the edge of the polygon,
//  then the function may return YES or NO.
//
//  Note that division by zero is avoided because the division is protected
//  by the "if" clause which surrounds it.

bool pointInPolygon(double *polyX, double *polyY, int polySides, double x, double y)
{
  int  i, j=polySides-1 ;
  bool oddNodes=false      ;

  for (i=0; i<polySides; i++) {
    if (polyY[i]<y && polyY[j]>=y
    ||  polyY[j]<y && polyY[i]>=y) {
      if (polyX[i]+(y-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i])<x) {
        oddNodes=!oddNodes; }}
    j=i; }

  return oddNodes;
}
//---------------------------------------------------------------------------

void TArco::PontasArco(TPonto pontas[2])
{
	pontas[0].x = Centro.x + EixoPrimario * cos(AngIni);
	pontas[0].y = Centro.y + EixoPrimario * sin(AngIni);

	pontas[1].x = Centro.x + EixoPrimario * cos(AngEnd);
	pontas[1].y = Centro.y + EixoPrimario * sin(AngEnd);
}       
//---------------------------------------------------------------------------

CDadosGenerico::CDadosGenerico()
{
	Multipoint.clear();
	Arcos.clear();
	Textos.clear();
	//      percorridos = NULL;
}


//---------------------------------------------------------------------------
CDadosGenerico::~CDadosGenerico ()
{

}
//---------------------------------------------------------------------------


double TArco::getAng() const
{
	double dif = AngEnd - AngIni;
	return fabs( dif );
}

