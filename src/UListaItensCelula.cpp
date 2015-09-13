//---------------------------------------------------------------------------

#include <algorithm>
#include "UListaItensCelula.h"

using namespace std;


bool TListaItensCelula::VerificaSeCaboRetaJaFoiLigadoAoEquipamento( CCaboReta *straightCable )
{
	return find(cabosRetaRelacionados.begin(), cabosRetaRelacionados.end(), straightCable) != cabosRetaRelacionados.end();
}


bool TListaItensCelula::VerificaSeCaboArcoJaFoiLigadoAoEquipamento( CCaboArco *arcCable )
{
	return find(cabosArcoRelacionados.begin(), cabosArcoRelacionados.end(), arcCable) != cabosArcoRelacionados.end();
}

//---------------------------------------------------------------------------

