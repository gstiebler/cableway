//---------------------------------------------------------------------------

#include <algorithm>
#include "UListaItensCelula.h"

using namespace std;


bool TListaItensCelula::VerificaSeCaboRetaJaFoiLigadoAoEquipamento(int IndiceCabo)
{
	return std::find(cabosRetaRelacionados.begin(), cabosRetaRelacionados.end(), IndiceCabo) != cabosRetaRelacionados.end();
}

bool TListaItensCelula::VerificaSeCaboArcoJaFoiLigadoAoEquipamento(int IndiceCabo)
{
	return std::find(cabosArcoRelacionados.begin(), cabosArcoRelacionados.end(), IndiceCabo) != cabosArcoRelacionados.end();
}

//---------------------------------------------------------------------------

