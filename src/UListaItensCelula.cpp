//---------------------------------------------------------------------------


#pragma hdrstop

#include "UListaItensCelula.h"

using namespace std;

//---------------------------------------------------------------------------
TListaItensCelula::~TListaItensCelula()
{
}
//---------------------------------------------------------------------------

TListaItensCelula::TListaItensCelula()
{
  emb=esq=1.5e280;
  enc=dir=-1.5e280;
}
//---------------------------------------------------------------------------

TListaItensCelula::TListaItensCelula(const TListaItensCelula &cpy)
{
//      temp->esq = old->esq;
//      temp->dir = old->dir;
//      temp->enc = old->enc;
//      temp->emb = old->emb;
//      temp->id = old->id;
//      temp->iTextos->assign(old->iTextos->begin(), old->iTextos->end());
//      temp->cabosRetaRelacionados->assign(old->cabosRetaRelacionados->begin(), old->cabosRetaRelacionados->end());
//      temp->cabosArcoRelacionados->assign(old->cabosArcoRelacionados->begin(), old->cabosArcoRelacionados->end());
  emb = cpy.emb;
  esq = cpy.esq;
  enc = cpy.enc;
  dir = cpy.dir;
  id = cpy.id;

  iTextosBandeirola.assign(cpy.iTextosBandeirola.begin(), cpy.iTextosBandeirola.end());
  iTextos.assign(cpy.iTextos.begin(), cpy.iTextos.end());
  cabosRetaRelacionados.assign(cpy.cabosRetaRelacionados.begin(), cpy.cabosRetaRelacionados.end());
  cabosArcoRelacionados.assign(cpy.cabosArcoRelacionados.begin(), cpy.cabosArcoRelacionados.end());

  _arcs.assign( cpy._arcs.begin(), cpy._arcs.end() );
  _multipoints.assign( cpy._multipoints.begin(), cpy._multipoints.end() );
}

bool TListaItensCelula::VerificaSeCaboRetaJaFoiLigadoAoEquipamento(int IndiceCabo)
{
  bool isAdded = false;
  // Cada cabo só é ligado a um equipamento em um lugar, por isso ele é guardado no cabosRetaRelacionados
  for ( unsigned int k = 0 ; k <(int) cabosRetaRelacionados.size() ; k++ )
  {
    if ( cabosRetaRelacionados[k] == IndiceCabo )
    {
      isAdded = true;
      break;
    }
  }
  return isAdded;
}

bool TListaItensCelula::VerificaSeCaboArcoJaFoiLigadoAoEquipamento(int IndiceCabo)
{
  bool isAdded;
  isAdded = false;
  for ( unsigned int k = 0 ; k <(int) cabosArcoRelacionados.size() ; k++ )
  {
    if ( cabosArcoRelacionados[k] == IndiceCabo )
    {
      isAdded = true;
      break;
    }
  }

  return isAdded;
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
