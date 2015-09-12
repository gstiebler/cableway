//---------------------------------------------------------------------------

#include "UInfoCelula.h"
#include "UDefines.h"
#include "UListaItensCelula.h"
#include "UDadosGenerico.h"

void TInfoCelula::FechaCelula( TListaItensCelula *ListaItensCelula )
{
	if( ListaItensCelula->_texts.size() > 0 )
	{
		if( ListaItensCelula->_texts.back()->Nivel == INSTRUMENTO )
			ListaCelulasInstrumentos.push_back(*ListaItensCelula);
		else if ( ListaItensCelula->_texts.back()->Nivel == BANDEIROLA )
			ListaCelulasBandeirolas.push_back(*ListaItensCelula);
	}
}

