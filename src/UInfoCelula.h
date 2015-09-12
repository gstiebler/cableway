//---------------------------------------------------------------------------

#ifndef UInfoCelulaH
#define UInfoCelulaH
//---------------------------------------------------------------------------

#include <vector>
#include <string>
#include <memory>

class TListaItensCelula;

struct TInfoCelula
{ 
	TInfoCelula();
	~TInfoCelula();


	void EntraCelula(int GrupoID, bool IsShared);

	/// <summary>Fecha a c�lula aberta no momento. Ou seja, v� qual � o tipo do grupo
	/// e toma a medida de acordo</summary>
	void FechaCelula();

	/// Lista de grupamentos em nível de Instrumento.
	std::vector<TListaItensCelula> ListaCelulasInstrumentos, ListaCelulasBandeirolas;

	/// Indica se está dentro de uma c�lula
	bool DentroCelula;

	/// Lista _TEMPOR�RIA_ de Itens da c�lula ATUAL
	TListaItensCelula *ListaItensCelula;
};
//---------------------------------------------------------------------------
#endif
