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
struct CCaboReta;
struct CCaboArco;

struct TListaItensCelula
{
	bool VerificaSeCaboRetaJaFoiLigadoAoEquipamento( CCaboReta *straightCable );
	bool VerificaSeCaboArcoJaFoiLigadoAoEquipamento( CCaboArco *arcCable );
	
	std::vector< CCaboReta* > cabosRetaRelacionados;
	std::vector< CCaboArco* > cabosArcoRelacionados;
	std::vector< std::shared_ptr<TTexto> > _texts;
	std::vector< std::shared_ptr<TArco> > _arcs;
	std::vector< std::shared_ptr<TMultipoint> > _multipoints;
};
//---------------------------------------------------------------------------
#endif
