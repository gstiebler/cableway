//---------------------------------------------------------------------------


#pragma hdrstop

#include "UInfoCelula.h"
#include "UListaCelulas.h"

TInfoCelula::TInfoCelula()
{
	ListaCelulasInstrumentos=new TListaCelulas;
	ListaCelulasBandeirolas=new TListaCelulas;
	TipoElementoCelulaAtual = NADA;
	DentroCelula=false;
	shared = false;
	FimCelula = -1;
	somaX = somaY = somaZ = 0;
  // É temporário!
	ListaItensCelula = NULL;
}
//---------------------------------------------------------------------------

TInfoCelula::~TInfoCelula()
{
	delete ListaCelulasInstrumentos;
	delete ListaCelulasBandeirolas;
	if ( ListaItensCelula )
		delete ListaItensCelula;
}
//---------------------------------------------------------------------------
TInfoCelula& TInfoCelula::operator=( TInfoCelula &f )
{
	delete ListaCelulasInstrumentos;
	delete ListaCelulasBandeirolas;
	ListaCelulasInstrumentos = new TListaCelulas(f.ListaCelulasInstrumentos);
	ListaCelulasBandeirolas = new TListaCelulas(f.ListaCelulasBandeirolas);

	TipoElementoCelulaAtual = f.TipoElementoCelulaAtual;
	DentroCelula=f.DentroCelula;
	shared = f.shared;
	FimCelula = f.FimCelula;
	somaX = f.somaX;
	somaY = f.somaY;
	somaZ = f.somaZ;
	ListaItensCelula = NULL;
	return *this;
};

void TInfoCelula::EntraCelula(int GrupoID, bool IsShared)
{
	// Tipo do elemento inicialmente é NADA
	TipoElementoCelulaAtual = NADA;
	// Mostra que tá dentro da Célula
	DentroCelula = true;
	// Seta o shared
	shared = IsShared;
	// Zera as somas
	somaX = somaY = somaZ = 0;
	// Cria um novo ItemCelula temporário
	ListaItensCelula=new TListaItensCelula;
	// E seta o ID dele
	ListaItensCelula->id = GrupoID;
}

void TInfoCelula::AdicionaTexto(int iTexto, string Texto, int NivelTexto)
{
	// Se o texto está em nível de tag, então ele é adicionado ao vetor de textos
	// da célula.
	if ( NivelTexto == TAG )
	{
		ListaItensCelula->iTextos->push_back(iTexto);
	}
	// Caso ele esteja em nível de bandeirola, ele é adicionado ao vetor de bandeirolas
	// da célula. 
	// Isso é necessário pois textos de bandeirola ficam em níveis de bandeirola
	// enquanto os textos de Equipamento ficam em nível de tag.
	else if ( NivelTexto == BANDEIROLA )
	{
		ListaItensCelula->iTextosBandeirola->push_back(iTexto);
	}
}

void TInfoCelula::FechaCelula()
{
	// Desliga o tag de DentroCelula.
	DentroCelula=false;
	if ( shared )
	{
		delete ListaItensCelula;
		ListaItensCelula = NULL;
		shared = false;
		return;
	}

	// Vê o tipo da célula atual
	switch (TipoElementoCelulaAtual)
	{
	// Caso esteja em grupamento de instrumento, ele adiciona na lista de intrumentos
	case INSTRUMENTODESCON:
		ListaCelulasInstrumentos->Adiciona(ListaItensCelula);
		break;
	// Caso esteja em grupamento de bandeirola, ele adiciona na lista de bandeirolas
	// e bota os textos que estavam em nível de bandeirola como os textos do grupo
	case BANDEIROLA:
		ListaItensCelula->iTextos->assign(ListaItensCelula->iTextosBandeirola->begin(), ListaItensCelula->iTextosBandeirola->end());
		ListaCelulasBandeirolas->Adiciona(ListaItensCelula);
		break;
	// Caso não esteja em um desses níveis, ele apaga o grupo e seta ListaItensCelula
	// como NULL.
	default:
		delete ListaItensCelula;
		ListaItensCelula = NULL;
	}
	// Seta a soma como 0.
	somaX = somaY = somaZ = 0;
}

#pragma package(smart_init)
