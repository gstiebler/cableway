#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "UParamsStructs.h"
#include "UGrafoDesenho.h"
#include "UNiveisProjeto.h"
#include "TDesenho.h"
#include "UContainerDesenhos.h"

int main (int argc, char *argv[])
{
	int cableLevel = 5;
	int equipLevel = 7;
	int tagEquipLevel = 10;

	TNiveisProjetoTransfer niveisProjetoTransfer;
	niveisProjetoTransfer.ListaCabo.push_back( "5" );
	niveisProjetoTransfer.ListaInstrumento.push_back( "7" );
	niveisProjetoTransfer.ListaTag.push_back( "10" );
	// Cria um novo params
	TParamsGrafoDesenho paramsGrafoDesenho;
	
	TParamsInfoCircuitos paramsInfoCircuitos;
	// O Id
	paramsGrafoDesenho.IDArquivo = 0;
	// Para carregar o grafo
	paramsGrafoDesenho.CarregaGrafo = true;
	// Define as alturas -- HARDCODED?? :(
	paramsGrafoDesenho.AlturaTeto=2.0;
	paramsGrafoDesenho.AlturaInterrup=0.7;
	paramsGrafoDesenho.Altura = 30.0;
	// Preenche o índice do desenho
	paramsGrafoDesenho.IndiceDesenho = 0;
	// Passa um ponteiro para o VerticesGerais (TVerticesGerais)
	paramsGrafoDesenho.VerticesGerais = paramsInfoCircuitos.VerticesGerais;
	// E um ponteiro pro Arestas (TListaArestas)
	paramsGrafoDesenho.Arestas = paramsInfoCircuitos.Arestas;
	// E um ponteiro pro NiveisProjeto (TNiveisProjetoTransfer)
	paramsGrafoDesenho.NiveisProjeto = &niveisProjetoTransfer;

	CDadosGenerico dados;

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

		//dados.InfoCelula.AdicionaTexto( dados.Textos.size(), "equip 1", tagEquipLevel );
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

		//dados.InfoCelula.ListaCelulasInstrumentos->
	}

	{
		TMultipoint cable;
		cable.Nivel = CABO;
		cable.pontos.push_back( TPonto( 20.0, 15.0 ) );
		cable.pontos.push_back( TPonto( 40.0, 15.0 ) );
		dados.Multipoint.push_back( cable );
	}

	CGrafoDesenho grafoDesenho(paramsGrafoDesenho, &dados);

	// Cria um novo desenho
	TDesenho *desenho = new TDesenho;
	// E o ID
	desenho->ID = 0;
	desenho->Altura = 50.0;
	desenho->GrafoDesenho = &grafoDesenho;

	CContainerDesenhos containerDesenhos( &niveisProjetoTransfer );
	containerDesenhos.addDrawing( desenho );
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