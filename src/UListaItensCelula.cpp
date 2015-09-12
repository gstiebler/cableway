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

