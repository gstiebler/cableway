//---------------------------------------------------------------------------

#ifndef UInfoCelulaH
#define UInfoCelulaH
//---------------------------------------------------------------------------

#include <vector>
#include <string>
using namespace std;

class TListaCelulas;
class TListaItensCelula;

struct TInfoCelula
{
	TInfoCelula();
	~TInfoCelula();
	TInfoCelula &operator=( TInfoCelula &f );

	/// <summary> Cria uma nova c�lula. Ou seja, faz DentroCelula ser verdade, 
	/// cria uma nova ListaItensCelula, seta shared, e d� um ID a essa celula.</summary>
	/// <param name="GrupoID">� um identificador �nico para esse grupamento.</param>
	/// <param name="IsShared">Indica o valor da vari�vel shared</param>
	void EntraCelula(int GrupoID, bool IsShared);

	/// <summary>Adiciona um texto a c�lula aberta no momento</summary>
	/// <param name="iTexto">O �ndice do texto no vetor de textos</param>
	/// <param name="Texto">String contendo o texto propriamente dito (DEBUG)</param>
	/// <param name="NivelTexto">nível do texto sendo adicionado: NADA, TAG, EQUIPAMENTO,
	/// BANDEIROLA</param>
	void AdicionaTexto(int iTexto, string Texto, int NivelTexto);

	/// <summary>Fecha a c�lula aberta no momento. Ou seja, v� qual � o tipo do grupo
	/// e toma a medida de acordo</summary>
	void FechaCelula();

	/// Lista de grupamentos em nível de Instrumento.
	TListaCelulas *ListaCelulasInstrumentos, 
	/// Lista de grupamentos em nível de Bandeirolas.
				  *ListaCelulasBandeirolas;

	/// Se DentroCelula � verdadeiro:
	///	Indica qual � o tipo da c�lula atual: NADA, INSTRUMENTO, BANDEIROLA
	int TipoElementoCelulaAtual;

	/// Indica se está dentro de uma c�lula
	bool DentroCelula;

	/// Indica se a c�lula � shared, usado em DGNV7
	bool shared;

	/// Indica quando � o fim da c�lula, usado em DGNV7
	int FimCelula;

	/// Quantia a ser somada a cada item da c�lula
	double somaX, somaY, somaZ;

	/// Lista _TEMPOR�RIA_ de Itens da c�lula ATUAL
	TListaItensCelula *ListaItensCelula;
};
//---------------------------------------------------------------------------
#endif
