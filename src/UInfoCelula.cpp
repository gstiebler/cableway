//---------------------------------------------------------------------------

#include "UInfoCelula.h"
#include "UDefines.h"
#include "UListaItensCelula.h"
#include "UDadosGenerico.h"

TInfoCelula::TInfoCelula()
{
	DentroCelula=false;
  // � tempor�rio!
	ListaItensCelula = NULL;
}
//---------------------------------------------------------------------------

TInfoCelula::~TInfoCelula()
{
}
//---------------------------------------------------------------------------


void TInfoCelula::EntraCelula(int GrupoID, bool IsShared)
{
	// Mostra que t� dentro da C�lula
	DentroCelula = true;
	// Cria um novo ItemCelula tempor�rio
	ListaItensCelula=new TListaItensCelula;
	// E seta o ID dele
	ListaItensCelula->id = GrupoID;
}

void TInfoCelula::FechaCelula()
{
	// Desliga o tag de DentroCelula.
	DentroCelula=false;

	if( ListaItensCelula->_texts.size() > 0 )
	{
		if( ListaItensCelula->_texts.back()->Nivel == INSTRUMENTO )
			ListaCelulasInstrumentos.push_back(*ListaItensCelula);
		else if ( ListaItensCelula->_texts.back()->Nivel == BANDEIROLA )
			ListaCelulasBandeirolas.push_back(*ListaItensCelula);
	}
	delete ListaItensCelula;
	ListaItensCelula = NULL;
}

