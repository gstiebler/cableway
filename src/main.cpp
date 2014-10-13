#include <string>
#include <vector>

#include "DwgLoader.h"
#include "UContainerDesenhos.h"
#include "UDadosGenerico.h"
#include "UDefines.h"
#include "UInfoCelula.h"
#include "UInfoCircuitos.h"
#include "UItemCelula.h"
#include "UListaCelulas.h"
#include "UListaItensCelula.h"
#include "UListaV.h"
#include "UserParams/UserParams.h"
#include "UTCallbackStatusCarregamento.h"

int main (int argc, char *argv[])
{
    string fileName = "drawing2.dwg";//TestsUtil::getExePath() + "/../data/tests/drawing2.dwg";

	CContainerDesenhos containerDesenhos;
	CDadosGenerico dados;

	UserParams userParams;
    DwgLoader *loader = new DwgLoader( fileName, &dados, &userParams );

	{
		TMultipoint equip1;
		equip1.pontos.push_back( TPonto( 10.0, 10.0 ) );
		equip1.pontos.push_back( TPonto( 20.0, 10.0 ) );
		equip1.pontos.push_back( TPonto( 20.0, 20.0 ) );
		equip1.pontos.push_back( TPonto( 10.0, 20.0 ) );
		equip1.Nivel = INSTRUMENTO;
		dados.Multipoint.push_back( equip1 );

		TTexto equip1Text;
		equip1Text.texto = "Equipamento 1";
		equip1Text.Nivel = TAG;
		dados.Textos.push_back( equip1Text );
		
		TItemCelula itemCelula;
		itemCelula.Indice = dados.Multipoint.size() - 1;
		itemCelula.TipoVetorCW = VMULTIPOINT;
		TListaItensCelula lista;
		lista.Adiciona( itemCelula );
		lista.iTextos.push_back( dados.Textos.size() - 1 );
		dados.InfoCelula.ListaCelulasInstrumentos->Adiciona( lista );
	}

	{
		TMultipoint equip2;
		equip2.pontos.push_back( TPonto( 40.0, 20.0 ) );
		equip2.pontos.push_back( TPonto( 40.0, 10.0 ) );
		equip2.pontos.push_back( TPonto( 50.0, 10.0 ) );
		equip2.pontos.push_back( TPonto( 50.0, 20.0 ) );
		equip2.Nivel = INSTRUMENTO;
		dados.Multipoint.push_back( equip2 );

		TTexto equip2Text;
		equip2Text.texto = "Equipamento 2";
		equip2Text.Nivel = TAG;
		dados.Textos.push_back( equip2Text );

		TItemCelula itemCelula;
		itemCelula.Indice = dados.Multipoint.size() - 1;
		itemCelula.TipoVetorCW = VMULTIPOINT;
		TListaItensCelula lista;
		lista.Adiciona( itemCelula );
		lista.iTextos.push_back( dados.Textos.size() - 1 );
		dados.InfoCelula.ListaCelulasInstrumentos->Adiciona( lista );
	}

	{
		TMultipoint cable;
		cable.Nivel = CABO;
		cable.pontos.push_back( TPonto( 20.0, 15.0 ) );
		cable.pontos.push_back( TPonto( 40.0, 15.0 ) );
		dados.Multipoint.push_back( cable );
	}


	containerDesenhos.addDrawing( dados, 100.0 );
	callbackStatusCarregamento sc;
	containerDesenhos.Conclui( sc );

	double tam;
	string rota;
	TArestasCircuito *ArestasCircuito = NULL;
	vector<int> ListaBandeirolas;
	vector<string> DEBUG_arestas;
	string SubRotas; 
	TCircuitoAreas *CircuitoAreas = NULL;

	containerDesenhos.InfoCircuitos->GeraRota("Equipamento 2", "Equipamento 1", tam, rota, ArestasCircuito, &ListaBandeirolas,
		&DEBUG_arestas, SubRotas, CircuitoAreas);

	return 0;
}
