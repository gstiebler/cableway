//---------------------------------------------------------------------------

#ifndef UListaItensCelulaH
#define UListaItensCelulaH
//---------------------------------------------------------------------------

//#include "UVetor.h"
#include <vector>
#include <memory>

struct TArco;
struct TMultipoint;
struct TTexto;

struct TListaItensCelula
{
	TListaItensCelula();
	~TListaItensCelula();
	bool VerificaSeCaboRetaJaFoiLigadoAoEquipamento(int indiceCabo);
	bool VerificaSeCaboArcoJaFoiLigadoAoEquipamento(int indiceCabo);
	
	std::vector<int> cabosRetaRelacionados, cabosArcoRelacionados;
	std::vector< std::shared_ptr<TTexto> > _texts;
	std::vector< std::shared_ptr<TArco> > _arcs;
	std::vector< std::shared_ptr<TMultipoint> > _multipoints;
};
//---------------------------------------------------------------------------
#endif
