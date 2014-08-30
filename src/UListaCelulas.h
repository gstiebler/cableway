//---------------------------------------------------------------------------

#ifndef UListaCelulasH
#define UListaCelulasH
//---------------------------------------------------------------------------
#include "UListaV.h"
#include "UListaItensCelula.h"

struct TListaCelulas: public TListaV<TListaItensCelula>
{
  TListaCelulas():TListaV<TListaItensCelula>() {}
  TListaCelulas(const TListaCelulas *cpy):TListaV<TListaItensCelula>()
  {
    idCelula = cpy->idCelula;
    for ( int i = 0 ; i < cpy->Tamanho() ; i++) //cpy->lista->Count ; i++ )
    {
      TListaItensCelula *temp = new TListaItensCelula(*cpy->getItem(i));
//      TListaItensCelula *old = cpy->getItem(i);
//      TListaItensCelula *old = (TListaItensCelula *)(cpy->lista->Items[i]);
//      temp->esq = old->esq;
//      temp->dir = old->dir;
//      temp->enc = old->enc;
//      temp->emb = old->emb;
//      temp->id = old->id;
//      temp->iTextos->assign(old->iTextos->begin(), old->iTextos->end());
//      temp->cabosRetaRelacionados->assign(old->cabosRetaRelacionados->begin(), old->cabosRetaRelacionados->end());
//      temp->cabosArcoRelacionados->assign(old->cabosArcoRelacionados->begin(), old->cabosArcoRelacionados->end());
      Adiciona(temp);
      delete temp;
    }
  }
  int idCelula;
};
//---------------------------------------------------------------------------
#endif
