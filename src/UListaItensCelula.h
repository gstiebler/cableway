//---------------------------------------------------------------------------

#ifndef UListaItensCelulaH
#define UListaItensCelulaH
//---------------------------------------------------------------------------

//#include "UVetor.h"
#include <vector>
#include <memory>

struct TArco;
struct TMultipoint;

struct TListaItensCelula
{

	std::vector< std::shared_ptr<TArco> > _arcs;
	std::vector< std::shared_ptr<TMultipoint> > _multipoints;
  double esq, dir, enc, emb;
  int id;
  std::vector<int> iTextos, iTextosBandeirola, cabosRetaRelacionados, cabosArcoRelacionados;
  TListaItensCelula();
  TListaItensCelula(const TListaItensCelula &cpy);
  ~TListaItensCelula();
bool VerificaSeCaboRetaJaFoiLigadoAoEquipamento(int indiceCabo);
bool VerificaSeCaboArcoJaFoiLigadoAoEquipamento(int indiceCabo);
};
//---------------------------------------------------------------------------
#endif
