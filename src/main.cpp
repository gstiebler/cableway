// A simple program that computes the square root of a number
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "UParamsStructs.h"
#include "UGrafoDesenho.h"
#include "UNiveisProjeto.h"

int main (int argc, char *argv[])
{
	TNiveisProjetoTransfer niveisProjetoTransfer;
	// Cria um novo params
	TParamsGrafoDesenho paramsGrafoDesenho;
	
	TParamsInfoCircuitos paramsInfoCircuitos;
	// O Id
	paramsGrafoDesenho.IDArquivo = 3;
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

	TDadosTransfer dadosDLL;

	CGrafoDesenho grafoDesenho(paramsGrafoDesenho, &dadosDLL);

	return 0;
}