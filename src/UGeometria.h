//---------------------------------------------------------------------------

#ifndef UGeometriaH
#define UGeometriaH

//#include "UDefines.h"
//#include "UDadosGenerico.h"
//#include "UVerticesArestas.h"

#include <cmath>
#include <vector>
#include <memory>
//#include "UErros.h"
//#include "UDefines.h"

//#include "UVerticesArestas.h"

struct TVerticeGeral;

//---------------------------------------------------------------------------
struct TPonto
{
	double x, y;

	TPonto();
	TPonto( double X, double Y );
	TPonto& operator-( TPonto &other );
	double getLength();
};

struct TReta
{
  TPonto pontos[2];
};

void PontoMaisDistante(TPonto ponto, std::vector<TPonto> pontos, TPonto &maisDist);
///<summary>Distância de um ponto para um segmento de reta</summary>
///<param name"Reta">Um vetor com os dois pontos que definem a reta</param>
///<param name"Ponto">O ponto que queremos calcula a Distância</param>
///<param name"PontoNoSegmento">O ponto que se achou na reta que era o mais pr�ximo do ponto</param>
///<returns>A Distância.</returns>
double DistPontoParaSegmentoReta(TPonto Reta[2], TPonto &Ponto, TPonto &PontoNoSegmento);
///<summary>Distância entre dois pontos</summary>
///<param name"p1">O primeiro ponto.</param>
///<param name"p2">O segundo ponto.</param>
///<returns>A Distância.</returns>
double DistPontos(TPonto &p1, TPonto &p2);
///<summary>Distância Manhattan entre dois pontos (x2-x1 + y2-y1)</summary>
///<param name"p1">O primeiro ponto</param>
///<param name"p2">O segundo ponto</param>
///<returns>A Distância</returns>
double DistPontosManhattan(TPonto &p1, TPonto &p2);
///<summary>Fun��o para ordenar uma reta de acordo com x.</summary>
///<param name"Item1">O primeiro vertice</param>
///<param name"Item2">O segundo vertice</param>
///<returns>Valor booleano, verdadeiro caso o primeiro seja menor do que o segundo.</returns>
bool OrdenaRetaHorizontal(const std::shared_ptr<TVerticeGeral> &Item1, const std::shared_ptr<TVerticeGeral> &Item2);
///<summary>Fun��o para ordenar uma reta de acordo com y.</summary>
///<param name"Item1">O primeiro vertice</param>
///<param name"Item2">O segundo vertice</param>
///<returns>Valor booleano, verdadeiro caso o primeiro seja menor do que o segundo.</returns>
bool OrdenaRetaVertical(const std::shared_ptr<TVerticeGeral> &Item1, const std::shared_ptr<TVerticeGeral> &Item2);

#endif
