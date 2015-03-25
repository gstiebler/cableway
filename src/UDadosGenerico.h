//---------------------------------------------------------------------------

#ifndef UDadosGenericoH
#define UDadosGenericoH

#include <vector>

#include "UInfoCelula.h"
//#include "UNiveisProjeto.h"
#include <cmath>
#include <memory>
#include "UDefines.h"

struct TElemento
{
	int ID;
	unsigned char CorR, CorG, CorB;
	int Nivel;
	int Peso;
	int Estilo;
	string layerName;
	TElemento()
	{
		ID = Nivel = Peso = Estilo = 0;
		CorR = CorG = CorB = 0xFF;
	}
	TElemento(const TElemento& TElem)
	{
		ID = TElem.ID;
		Nivel = TElem.Nivel;
		Peso = TElem.Peso;
		Estilo = TElem.Estilo;
		CorR = TElem.CorR;
		CorG = TElem.CorG;
		CorB = TElem.CorB;
	}
};
//---------------------------------------------------------------------------

struct TMultipoint: public TElemento
{
	int tipo;
	// Pontos que formam o Multipoint.
	std::vector<TPonto> pontos;

	TMultipoint() : TElemento() {
		tipo = 0;
	}
  // Construtor de c�pia
	TMultipoint(const TMultipoint& TMulti) : TElemento(TMulti)
	{
		*this = TMulti;
	}
  // Operador de atribui��o
	TMultipoint& operator=( const TMultipoint &TMulti );
};
//---------------------------------------------------------------------------

struct TArco: public TElemento
{
	TPonto Centro;
	double EixoPrimario;
	double EixoSecundario;
	// Counterclockwise rotation in degrees
	double Rotacao;
	// Start angle (degrees counterclockwise of primary axis)
	double AngIni;
	// Sweep angle (degrees)
	double AngEnd;
	// �ndice dos vertices 
	int iV[2];
  ///<summary>Fun��o que calcula as duas pontas de um arco.</summary>
  ///<param name"Arco">O Arco.</param>
  ///<param name"pontas">O ponteiro para o vetor de pontas</param>
  void PontasArco(TPonto pontas[2]);
	TArco() : TElemento()
	{
		Centro.x = Centro.y = 0.0;
		EixoPrimario = EixoSecundario = Rotacao = AngIni = AngEnd = 0.0;
		iV[0] = iV[1] = -1;
	}
  TArco(const TArco &arco) : TElemento(arco)
  {
    Centro.x = arco.Centro.x;
    Centro.y = arco.Centro.y;
    EixoPrimario = arco.EixoPrimario;
    EixoSecundario = arco.EixoSecundario;
    Rotacao = arco.Rotacao;
    AngIni = arco.AngIni;
    AngEnd = arco.AngEnd;
    iV[0] = arco.iV[0];
    iV[1] = arco.iV[1];
  }
	TArco& operator=( const TArco &arco )
  {
		ID = arco.ID;
		CorR = arco.CorR;
		CorG = arco.CorG;
		CorB = arco.CorB;
		Nivel = arco.Nivel;
		Peso = arco.Peso;
		Estilo = arco.Estilo;
    Centro.x = arco.Centro.x;
    Centro.y = arco.Centro.y;
    EixoPrimario = arco.EixoPrimario;
    EixoSecundario = arco.EixoSecundario;
    Rotacao = arco.Rotacao;
    AngIni = arco.AngIni;
    AngEnd = arco.AngEnd;
    iV[0] = arco.iV[0];
    iV[1] = arco.iV[1];
  	return *this;
  }
	double getAng() const;
};
//---------------------------------------------------------------------------

struct TTexto: public TElemento
{
	TPonto origem;
	double rotacao;
	string texto;
	double FatorAltura;
	TTexto() : TElemento()
	{
		origem.x = origem.y = rotacao = FatorAltura = 0.0;
	};
};
//---------------------------------------------------------------------------

class CDadosGenerico
{
protected:
public:
	int IndiceDesenho;
	int NumElementos;
	int IDArquivo;
	TInfoCelula InfoCelula;
	string NomeArq;
	vector< std::shared_ptr<TMultipoint> > Multipoint;
	vector< std::shared_ptr<TArco> > Arcos;
	vector< std::shared_ptr<TTexto> > Textos;
	CDadosGenerico();
	~CDadosGenerico ();
};

bool pointInPolygon(double *polyX, double *polyY, int polySides, double x, double y);

#endif

