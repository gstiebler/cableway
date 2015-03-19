//---------------------------------------------------------------------------


#pragma hdrstop

#include "UDadosGenerico.h"

#include <string.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

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

TTexto::TTexto(const TTextoTransfer &Texto)
{
	origem = Texto.origem;
	rotacao = Texto.rotacao;
	FatorAltura = Texto.FatorAltura;
	texto = string(Texto.texto, Texto.tamTexto);
	CorR = Texto.CorR;
	CorG = Texto.CorG;
	CorB = Texto.CorB;
	ID = Texto.ID;
	Nivel = Texto.Nivel;
	Peso = Texto.Peso;
	Estilo = Texto.Estilo;
}
//---------------------------------------------------------------------------

TTexto& TTexto::operator=( const TTextoTransfer &Texto ) 
{
	origem = Texto.origem;
	rotacao = Texto.rotacao;
	FatorAltura = Texto.FatorAltura;
	texto = string(Texto.texto, Texto.tamTexto);
	CorR = Texto.CorR;
	CorG = Texto.CorG;
	CorB = Texto.CorB;
	ID = Texto.ID;
	Nivel = Texto.Nivel;
	Peso = Texto.Peso;
	Estilo = Texto.Estilo;
	return *this;
}
//---------------------------------------------------------------------------

TTextoTransfer& TTextoTransfer::operator=( const TTexto &Texto )
{
	origem = Texto.origem;
	rotacao = Texto.rotacao;
	FatorAltura = Texto.FatorAltura;
	texto = new char[Texto.texto.size()+1];
	tamTexto = Texto.texto.size();
	strncpy(texto, Texto.texto.c_str(), tamTexto);
	texto[tamTexto] = '\0';
	CorR = Texto.CorR;
	CorG = Texto.CorG;
	CorB = Texto.CorB;
	ID = Texto.ID;
	Nivel = Texto.Nivel;
	Peso = Texto.Peso;
	Estilo = Texto.Estilo;
	return *this;
}
//---------------------------------------------------------------------------
CDadosGenerico::CDadosGenerico()
{
	NumElementos = IDArquivo = IndiceDesenho = 0;

	InfoCelula.somaX = InfoCelula.somaY = InfoCelula.somaZ = InfoCelula.FimCelula = 0;
	InfoCelula.DentroCelula = false;
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


void CDadosGenerico::convertePraTransfer(TDadosTransfer *dados)
{
	//dados->NumMultipoint = NumMultipoint;
	//dados->NumArcos = NumArcos;
	//dados->NumTextos = NumTextos;
	//dados->Multipoint = new TMultipoint[NumMultipoint];
	//dados->Arcos = new TArco[NumArcos];
	//dados->Textos = new TTextoTransfer[NumTextos];
	//dados->IndiceDesenho = IndiceDesenho;
	//dados->IDArquivo = IDArquivo;
	//for ( int i = 0 ; i < NumMultipoint ; i++ )
	//{
	//	dados->Multipoint[i] = Multipoint[i];
	//}
	////      copy(Multipoint.begin(), Multipoint.end(), dados->Multipoint);
	//if ( NumArcos > 0 )
	//	copy(Arcos.begin(), Arcos.end(), dados->Arcos);
	//if ( NumTextos > 0 )
	//	copy(Textos.begin(), Textos.end(), dados->Textos);
	//dados->InfoCelula = InfoCelula;
	//dados->NomeArqTam = NomeArq.size();
	//dados->NomeArq = new char[dados->NomeArqTam+1];
	//strncpy(dados->NomeArq, NomeArq.c_str(), dados->NomeArqTam);
}
//---------------------------------------------------------------------------

void CDadosGenerico::importaTransfer(TDadosTransfer *dados)
{
	//int i;
	//NumMultipoint = dados->NumMultipoint;
	//NumArcos = dados->NumArcos;
	//NumTextos = dados->NumTextos;
	//NumElementos = NumMultipoint + NumArcos + NumTextos;
	//IndiceDesenho = dados->IndiceDesenho;
	//IDArquivo = dados->IDArquivo;
	//for ( i = 0 ; i < NumMultipoint ; i++ )
	//{
	//	Multipoint.push_back(dados->Multipoint[i]);
	//}
	//for ( i = 0 ; i < NumArcos ; i++ )
	//{
	//	Arcos.push_back(dados->Arcos[i]);
	//}
	//for ( i = 0 ; i < NumTextos ; i++ )
	//{
	//	Textos.push_back(dados->Textos[i]);
	//}
	////      Multipoint.assign(&dados->Multipoint[0], &dados->Multipoint[NumMultipoint-1]);
	////      Arcos.assign(&dados->Arcos[0], &dados->Arcos[NumArcos-1]);
	////      Textos.assign(&dados->Textos[0], &dados->Textos[NumTextos-1]);
	//InfoCelula = dados->InfoCelula;
	//NomeArq = string(dados->NomeArq, dados->NomeArqTam);
}

void liberaTransfer (TDadosTransfer *dados)
{
	delete[] dados->Multipoint;
	delete[] dados->Arcos;
	delete[] dados->Textos;
	delete[] dados->NomeArq;
}
