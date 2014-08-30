//---------------------------------------------------------------------------


#pragma hdrstop

#include "UListaItensCelula.h"

using namespace std;

//---------------------------------------------------------------------------
TListaItensCelula::~TListaItensCelula()
{
  delete cabosRetaRelacionados;
  delete cabosArcoRelacionados;
  delete iTextos;
  delete iTextosBandeirola;
}
//---------------------------------------------------------------------------

TListaItensCelula::TListaItensCelula(): TListaV<TItemCelula>()
{
  emb=esq=1.5e280;
  enc=dir=-1.5e280;

  cabosRetaRelacionados = new vector<int>();
  cabosArcoRelacionados = new vector<int>();
  iTextosBandeirola = new vector<int>();
  iTextos = new vector<int>();
}
//---------------------------------------------------------------------------

TListaItensCelula::TListaItensCelula(const TListaItensCelula &cpy): TListaV<TItemCelula>()
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

  iTextosBandeirola = new vector<int>();
  iTextosBandeirola->assign(cpy.iTextosBandeirola->begin(), cpy.iTextosBandeirola->end());
  iTextos = new vector<int>();
  iTextos->assign(cpy.iTextos->begin(), cpy.iTextos->end());
  cabosRetaRelacionados = new vector<int>();
  cabosRetaRelacionados->assign(cpy.cabosRetaRelacionados->begin(), cpy.cabosRetaRelacionados->end());
  cabosArcoRelacionados = new vector<int>();
  cabosArcoRelacionados->assign(cpy.cabosArcoRelacionados->begin(), cpy.cabosArcoRelacionados->end());
  for ( int i = 0 ; i < cpy.Tamanho() ; i++ )
  {
    TItemCelula temp, *old;
    old = cpy.getItem(i);
    temp.Indice = old->Indice;
    temp.TipoVetorCW = old->TipoVetorCW;
    Adiciona(temp);
  }
}

bool TListaItensCelula::VerificaSeCaboRetaJaFoiLigadoAoEquipamento(int IndiceCabo)
{
  bool isAdded = false;
  // Cada cabo só é ligado a um equipamento em um lugar, por isso ele é guardado no cabosRetaRelacionados
  for ( unsigned int k = 0 ; k <(int) cabosRetaRelacionados->size() ; k++ )
  {
    if ( cabosRetaRelacionados->at(k) == IndiceCabo )
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
  for ( unsigned int k = 0 ; k <(int) cabosArcoRelacionados->size() ; k++ )
  {
    if ( cabosArcoRelacionados->at(k) == IndiceCabo )
    {
      isAdded = true;
      break;
    }
  }

  return isAdded;
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
