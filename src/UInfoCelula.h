//---------------------------------------------------------------------------

#ifndef UInfoCelulaH
#define UInfoCelulaH
//---------------------------------------------------------------------------

#include <vector>
#include <string>
#include <memory>
#include "UListaItensCelula.h"

class TListaItensCelula;

struct TInfoCelula
{ 
	void FechaCelula( TListaItensCelula *ListaItensCelula );

	/// Lista de grupamentos em nível de Instrumento.
	std::vector<TListaItensCelula> ListaCelulasInstrumentos, ListaCelulasBandeirolas;
};
//---------------------------------------------------------------------------
#endif
