//---------------------------------------------------------------------------

#include "UInfoCelula.h"
#include "UDefines.h"
#include "UListaItensCelula.h"
#include "UDadosGenerico.h"

TInfoCelula::TInfoCelula()
{
  // � tempor�rio!
	ListaItensCelula = NULL;
}
//---------------------------------------------------------------------------

TInfoCelula::~TInfoCelula()
{
}
//---------------------------------------------------------------------------


void TInfoCelula::EntraCelula()
{
	// Cria um novo ItemCelula tempor�rio
	ListaItensCelula=new TListaItensCelula;
}

void TInfoCelula::FechaCelula()
{
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

