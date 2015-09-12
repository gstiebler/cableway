//---------------------------------------------------------------------------


#pragma hdrstop

#include "UInfoCelula.h"
#include "UDefines.h"
#include "UListaItensCelula.h"

TInfoCelula::TInfoCelula()
{
	TipoElementoCelulaAtual = NADA;
	DentroCelula=false;
  // � tempor�rio!
	ListaItensCelula = NULL;
}
//---------------------------------------------------------------------------

TInfoCelula::~TInfoCelula()
{
	//delete ListaCelulasInstrumentos;
	//delete ListaCelulasBandeirolas;
	//if ( ListaItensCelula )
	//	delete ListaItensCelula;
}
//---------------------------------------------------------------------------
TInfoCelula& TInfoCelula::operator=( TInfoCelula &f )
{
	ListaCelulasInstrumentos.clear();
	ListaCelulasBandeirolas.clear();

	ListaCelulasInstrumentos.assign( f.ListaCelulasInstrumentos.begin(), f.ListaCelulasInstrumentos.end() );
	ListaCelulasBandeirolas.assign( f.ListaCelulasBandeirolas.begin(), f.ListaCelulasBandeirolas.end() );

	TipoElementoCelulaAtual = f.TipoElementoCelulaAtual;
	DentroCelula=f.DentroCelula;
	ListaItensCelula = NULL;
	return *this;
};

void TInfoCelula::EntraCelula(int GrupoID, bool IsShared)
{
	// Tipo do elemento inicialmente � NADA
	TipoElementoCelulaAtual = NADA;
	// Mostra que t� dentro da C�lula
	DentroCelula = true;
	// Cria um novo ItemCelula tempor�rio
	ListaItensCelula=new TListaItensCelula;
	// E seta o ID dele
	ListaItensCelula->id = GrupoID;
}

void TInfoCelula::AdicionaTexto(int iTexto, string Texto, int NivelTexto)
{
	// Se o texto está em nível de tag, então ele � adicionado ao vetor de textos
	// da c�lula.
	if ( NivelTexto == INSTRUMENTO )
	{
		ListaItensCelula->iTextos.push_back(iTexto);
		TipoElementoCelulaAtual = INSTRUMENTO;
	}
	// Caso ele esteja em nível de bandeirola, ele � adicionado ao vetor de bandeirolas
	// da c�lula.
	// Isso � necess�rio pois textos de bandeirola ficam em n�veis de bandeirola
	// enquanto os textos de Equipamento ficam em nível de tag.
	else if ( NivelTexto == BANDEIROLA )
	{
		ListaItensCelula->iTextosBandeirola.push_back(iTexto);
		TipoElementoCelulaAtual = BANDEIROLA;
	}
}

void TInfoCelula::FechaCelula()
{
	// Desliga o tag de DentroCelula.
	DentroCelula=false;

	// V� o tipo da c�lula atual
	switch (TipoElementoCelulaAtual)
	{
	// Caso esteja em grupamento de instrumento, ele adiciona na lista de intrumentos
	case INSTRUMENTO:
		ListaCelulasInstrumentos.push_back(*ListaItensCelula);
		break;
	// Caso esteja em grupamento de bandeirola, ele adiciona na lista de bandeirolas
	// e bota os textos que estavam em nível de bandeirola como os textos do grupo
	case BANDEIROLA:
		ListaItensCelula->iTextos.assign(ListaItensCelula->iTextosBandeirola.begin(), ListaItensCelula->iTextosBandeirola.end());
		ListaCelulasBandeirolas.push_back(*ListaItensCelula);
		break;
	// Caso não esteja em um desses n�veis, ele apaga o grupo e seta ListaItensCelula
	// como NULL.
	default:
		delete ListaItensCelula;
		ListaItensCelula = NULL;
	}
}

