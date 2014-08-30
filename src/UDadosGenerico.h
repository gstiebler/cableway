//---------------------------------------------------------------------------

#ifndef UDadosGenericoH
#define UDadosGenericoH

#include <vector>

#include "UInfoCelula.h"
//#include "UNiveisProjeto.h"
#include <cmath>
#include "UDefines.h"

class TNiveisProjeto;


struct TElemento
{
	int ID;
	unsigned char CorR, CorG, CorB;
	int Nivel;
	int Peso;
	int Estilo;
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
	int tam;
	int tipo;
	// Pontos que formam o Multipoint.
	TPonto *pontos;

	TMultipoint() : TElemento() {
		tam = 0;
		tipo = 0;
		pontos = NULL;
	}
	~TMultipoint() {
		if ( pontos )
			delete[] pontos;
	}
  // Construtor de cópia
	TMultipoint(const TMultipoint& TMulti) : TElemento(TMulti)
	{
		tam = TMulti.tam;
		tipo = TMulti.tipo;
		pontos = NULL;
		if ( TMulti.pontos )
		{
			pontos = new TPonto[tam];
			for ( int i = 0 ; i < tam; i++ )
			{
				pontos[i].x = TMulti.pontos[i].x;
				pontos[i].y = TMulti.pontos[i].y;
			}
		}
	}
  // Operador de atribuição
	TMultipoint& operator=( const TMultipoint &TMulti )
	{
		ID = TMulti.ID;
		CorR = TMulti.CorR;
		CorG = TMulti.CorG;
		CorB = TMulti.CorB;
		Nivel = TMulti.Nivel;
		Peso = TMulti.Peso;
		Estilo = TMulti.Estilo;
		tam = TMulti.tam;
		tipo = TMulti.tipo;
		pontos = NULL;
		if ( TMulti.pontos )
		{
			pontos = new TPonto[tam];
			for ( int i = 0 ; i < tam; i++ )
			{
				pontos[i].x = TMulti.pontos[i].x;
				pontos[i].y = TMulti.pontos[i].y;
			}
		}
		return *this;
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
	double AngTam;
	// Índice dos vertices 
	int iV[2];
  ///<summary>Função que calcula as duas pontas de um arco.</summary>
  ///<param name"Arco">O Arco.</param>
  ///<param name"pontas">O ponteiro para o vetor de pontas</param>
  void PontasArco(TPonto pontas[2]);
	TArco() : TElemento()
	{
		Centro.x = Centro.y = 0.0;
		EixoPrimario = EixoSecundario = Rotacao = AngIni = AngTam = 0.0;
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
    AngTam = arco.AngTam;
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
    AngTam = arco.AngTam;
    iV[0] = arco.iV[0];
    iV[1] = arco.iV[1];
  	return *this;
  }
};
//---------------------------------------------------------------------------
struct TTextoTransfer;

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
	///<summary>Cria um objeto TTexto usando um objeto TTextoTransfer como base.</summary>
	///<param name"Texto">Objeto do tipo TTextoTransfer que será usado como base.</param>
	TTexto(const TTextoTransfer &Texto);
	TTexto& operator=( const TTextoTransfer &Texto );
};
//---------------------------------------------------------------------------

struct TTextoTransfer: public TElemento
{
	TPonto origem;
	double rotacao;
	char *texto;
	int tamTexto;
	double FatorAltura;
	TTextoTransfer& operator=( const TTexto &Texto );
	~TTextoTransfer () { delete[] texto; }
};
//---------------------------------------------------------------------------

// Estrutura para troca de dados entre o principal e a dll
struct TDadosTransfer
{
  // São preenchidos na dll
	TInfoCelula InfoCelula;
	int NumMultipoint;
	int NumArcos;
	int NumTextos;
	TMultipoint *Multipoint;
	TArco *Arcos;
	TTextoTransfer *Textos;
  // Já vem preenchidos do Builder;
	int IndiceDesenho;
	int IDArquivo;
	char *NomeArq;
	int NomeArqTam;
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
	TNiveisProjeto *NiveisProjeto;
	vector<TMultipoint> Multipoint;
	vector<TArco> Arcos;
	vector<TTexto> Textos;
	CDadosGenerico();
	~CDadosGenerico ();
	///<summary>Função que preenche um objeto do tipo TDadosTransfer com base na instância
	///do objeto CDadosGenerico.</summary>
	///<param name"dados">Objeto do tipo TDadosTransfer que será preenchido.</param>
	void convertePraTransfer(TDadosTransfer *dados);
	///<summary>Função que preenche a instância de CDadosGenerico com base num objeto 
	///TDadosTransfer que será passado a função.</summary>
	///<param name"dados">Objeto do tipo TDadosTransfer que será usado como base.</param>
	void importaTransfer(TDadosTransfer *dados);
};

bool pointInPolygon(double *polyX, double *polyY, int polySides, double x, double y);
void liberaTransfer (TDadosTransfer *dados);

#endif

