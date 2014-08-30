//---------------------------------------------------------------------------

#ifndef UListaItensCelulaH
#define UListaItensCelulaH
//---------------------------------------------------------------------------
#include "UListaV.h"
#include "UItemCelula.h"
//#include "UVetor.h"
#include <vector>

struct TListaItensCelula: public TListaV<TItemCelula>
{
//  int iTexto;
//  int iTexto2Colar;
  double esq, dir, enc, emb;
  int id;
  std::vector<int> *iTextos, *iTextosBandeirola, *cabosRetaRelacionados, *cabosArcoRelacionados;
  TListaItensCelula();
  TListaItensCelula(const TListaItensCelula &cpy);
  ~TListaItensCelula();
bool VerificaSeCaboRetaJaFoiLigadoAoEquipamento(int indiceCabo);
bool VerificaSeCaboArcoJaFoiLigadoAoEquipamento(int indiceCabo);
};
//---------------------------------------------------------------------------
#endif
