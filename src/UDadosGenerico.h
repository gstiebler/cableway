//---------------------------------------------------------------------------

#ifndef UDadosGenericoH
#define UDadosGenericoH

#include <vector>

#include "UInfoCelula.h"
//#include "UNiveisProjeto.h"
#include <cmath>
#include <memory>
#include "UDefines.h"
#include "UVerticesArestas.h"

struct TElemento
{
	int ID;
	unsigned char CorR, CorG, CorB;
	int Nivel;
	int Peso;
	int Estilo;
	std::string layerName;
	TElemento()
	{
		ID = Nivel = Peso = Estilo = 0;
		CorR = CorG = CorB = 0xFF;
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
	std::vector< std::shared_ptr<TVerticeGeral> > _vertices;
  ///<summary>Fun��o que calcula as duas pontas de um arco.</summary>
  ///<param name"Arco">O Arco.</param>
  ///<param name"pontas">O ponteiro para o vetor de pontas</param>
  void PontasArco(TPonto pontas[2]);
	TArco() : TElemento()
	{
		_vertices.resize( 2 );
		Centro.x = Centro.y = 0.0;
		EixoPrimario = EixoSecundario = Rotacao = AngIni = AngEnd = 0.0;
	}

	double getAng() const;
};
//---------------------------------------------------------------------------

struct TTexto: public TElemento
{
	TPonto origem;
	double rotacao;
	std::string texto;
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
	std::shared_ptr<TDesenho> _drawing;
	TInfoCelula InfoCelula;
	std::string NomeArq;
	std::vector< std::shared_ptr<TMultipoint> > Multipoint;
	std::vector< std::shared_ptr<TArco> > Arcos;
	std::vector< std::shared_ptr<TTexto> > Textos;
	CDadosGenerico();
	~CDadosGenerico ();
};

bool pointInPolygon(double *polyX, double *polyY, int polySides, double x, double y);

#endif

