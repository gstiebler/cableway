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

	/// <summary> Cria uma nova célula. Ou seja, faz DentroCelula ser verdade, 
	/// cria uma nova ListaItensCelula, seta shared, e dá um ID a essa celula.</summary>
	/// <param name="GrupoID">É um identificador único para esse grupamento.</param>
	/// <param name="IsShared">Indica o valor da variável shared</param>
	void EntraCelula(int GrupoID, bool IsShared);

	/// <summary>Adiciona um texto a célula aberta no momento</summary>
	/// <param name="iTexto">O índice do texto no vetor de textos</param>
	/// <param name="Texto">String contendo o texto propriamente dito (DEBUG)</param>
	/// <param name="NivelTexto">Nível do texto sendo adicionado: NADA, TAG, EQUIPAMENTO,
	/// BANDEIROLA</param>
	void AdicionaTexto(int iTexto, string Texto, int NivelTexto);

	/// <summary>Fecha a célula aberta no momento. Ou seja, vê qual é o tipo do grupo
	/// e toma a medida de acordo</summary>
	void FechaCelula();

	/// Lista de grupamentos em nível de Instrumento.
	TListaCelulas *ListaCelulasInstrumentos, 
	/// Lista de grupamentos em nível de Bandeirolas.
				  *ListaCelulasBandeirolas;

	/// Se DentroCelula é verdadeiro:
	///	Indica qual é o tipo da célula atual: NADA, INSTRUMENTO, BANDEIROLA
	int TipoElementoCelulaAtual;

	/// Indica se está dentro de uma célula
	bool DentroCelula;

	/// Indica se a célula é shared, usado em DGNV7
	bool shared;

	/// Indica quando é o fim da célula, usado em DGNV7
	int FimCelula;

	/// Quantia a ser somada a cada item da célula
	double somaX, somaY, somaZ;

	/// Lista _TEMPORÁRIA_ de Itens da célula ATUAL
	TListaItensCelula *ListaItensCelula;
};
//---------------------------------------------------------------------------
#endif
