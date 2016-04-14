//---------------------------------------------------------------------------


#pragma hdrstop

#include "UGeometria.h"
#include "UVerticesArestas.h"

using namespace std;

TPonto::TPonto()
{
}


TPonto::TPonto( double X, double Y ) :
	x( X ),
	y( Y )
{}


TPonto& TPonto::operator-( TPonto &other )
{
	return TPonto( x - other.x, y - other.y );
}


double TPonto::getLength()
{
	return sqrt( x * x + y * y );
}


void PontoMaisDistante(TPonto ponto, vector<TPonto> pontos,
		TPonto &maisDist)
{
	int m;
	double MaiorDist, Dist;
	TPonto PontoVetor;

  int IndicePonto=-1;
	MaiorDist=0;
	for (m=0; m<(int)pontos.size(); m++)
	{
		PontoVetor=pontos.at(m);
		TPonto volta;
		Dist = DistPontos(PontoVetor, ponto);
		if (Dist > MaiorDist)
		{
			MaiorDist = Dist;
			IndicePonto = m;
		}
	}//for (m=0; m<NumCabosReta; m++)
	//DistMaisProx=MenorDist;
	if (IndicePonto==-1)
	{
			
	}
  else
  {
    maisDist = pontos[IndicePonto];
  }
}
double DistPontosManhattan(TPonto &p1, TPonto &p2)
{
  return fabs(p1.x-p2.x)+fabs(p1.y-p2.y);
}
//---------------------------------------------------------------------------

//caso a reta não seja horizontal, o coenficiente "a" da reta será o inverso.
//ou seja, ao invés de ser x/y, será y/x
double DistPontoParaSegmentoReta(TPonto Reta[2], TPonto &Ponto, TPonto &PontoNoSegmento)
{
  double DifX, DifY, aDifX, aDifY;
  double a, b;//coeficientes da reta
  double m;//equivalente ao "b" da primeira reta, mas � da reta perpedicular que passa por Ponto
  double x, y;//encontro entre as retas
  int iMenorX, iMenorY, iMaiorX, iMaiorY;

  DifX=Reta[0].x-Reta[1].x;
  DifY=Reta[0].y-Reta[1].y;
  aDifX=fabs(DifX);
  aDifY=fabs(DifY);
  if ( fabs(DifX) <= 0.001 && fabs(DifY) <= 0.001 )
  {
    PontoNoSegmento = Reta[0];
    return DistPontos(Reta[0], Ponto);
  }
  if (aDifX>aDifY)
  {
    if (Reta[0].x<Reta[1].x)
    {
      iMenorX=0;
      iMaiorX=1;
    }
    else
    {
      iMenorX=1;
      iMaiorX=0;
    }
    a=DifY/DifX;
    b=Reta[0].y-a*Reta[0].x;
    m=Ponto.x+a*Ponto.y;
    y=(a*m+b)/(a*a+1);
    x=m-a*y;
    PontoNoSegmento.x=x;
    PontoNoSegmento.y=y;
    if (x<Reta[iMenorX].x)
    {
      PontoNoSegmento.x=Reta[iMenorX].x;
      PontoNoSegmento.y=Reta[iMenorX].y;
      return sqrt(pow(Ponto.x-Reta[iMenorX].x, 2)+pow(Ponto.y-Reta[iMenorX].y, 2));
    }
    else if (x>Reta[iMaiorX].x)
    {                               
      PontoNoSegmento.x=Reta[iMaiorX].x;
      PontoNoSegmento.y=Reta[iMaiorX].y;
      return sqrt(pow(Ponto.x-Reta[iMaiorX].x, 2)+pow(Ponto.y-Reta[iMaiorX].y, 2));
    }
    else
      return sqrt(pow(x-Ponto.x, 2)+pow(y-Ponto.y, 2));
  }
  else
  {
    if (Reta[0].y<Reta[1].y)
    {
      iMenorY=0;
      iMaiorY=1;
    }
    else
    {
      iMenorY=1;
      iMaiorY=0;
    }
    a=DifX/DifY;
    b=Reta[0].x-a*Reta[0].y;
    m=Ponto.y+a*Ponto.x;
    x=(a*m+b)/(a*a+1);
    y=m-a*x;             
    PontoNoSegmento.x=x;
    PontoNoSegmento.y=y;
    if (y<Reta[iMenorY].y)
    {
      PontoNoSegmento.x=Reta[iMenorY].x;
      PontoNoSegmento.y=Reta[iMenorY].y;
      return sqrt(pow(Ponto.x-Reta[iMenorY].x, 2)+pow(Ponto.y-Reta[iMenorY].y, 2));
    }
    else if (y>Reta[iMaiorY].y)
    {    
      PontoNoSegmento.x=Reta[iMaiorY].x;
      PontoNoSegmento.y=Reta[iMaiorY].y;
      return sqrt(pow(Ponto.x-Reta[iMaiorY].x, 2)+pow(Ponto.y-Reta[iMaiorY].y, 2));
    }
    else
      return sqrt(pow(x-Ponto.x, 2)+pow(y-Ponto.y, 2));
  }
}
//---------------------------------------------------------------------------

double DistPontos(TPonto &p1, TPonto &p2)
{
  return sqrt(pow(p1.x-p2.x, 2)+pow(p1.y-p2.y, 2));
}
//---------------------------------------------------------------------------

bool OrdenaRetaVertical(const shared_ptr<TVerticeGeral> &Item1, const shared_ptr<TVerticeGeral> &Item2)
{
	return Item1->pos.y < Item2->pos.y;
}  
//---------------------------------------------------------------------------

bool OrdenaRetaHorizontal(const shared_ptr<TVerticeGeral> &Item1, const shared_ptr<TVerticeGeral> &Item2)
{
	//return (Item1.pos.x - Item2.pos.x) < -0.001; 
	return Item1->pos.x < Item2->pos.x;
}

