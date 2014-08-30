//---------------------------------------------------------------------------

#ifndef UItemCelulaH
#define UItemCelulaH
//---------------------------------------------------------------------------
#include "UDefines.h"
struct TItemCelula
{
  int Indice;
  TTipoVetorCW TipoVetorCW;
  TItemCelula(){Indice = -1;
				TipoVetorCW = VNADA;}
  TItemCelula(const TItemCelula &cpy){Indice=cpy.Indice; TipoVetorCW=cpy.TipoVetorCW;}
};
//---------------------------------------------------------------------------
#endif
 