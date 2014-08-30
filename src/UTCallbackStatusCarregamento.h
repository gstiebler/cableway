//---------------------------------------------------------------------------

#ifndef UTCallbackStatusCarregamentoH
#define UTCallbackStatusCarregamentoH
//---------------------------------------------------------------------------
//#include "system.hpp"

#include <string>

struct callbackStatusCarregamento
{
  void *PonteiroProThis;
  void (*ponteiroFuncao)(void *PonteiroThis, std::string text, int quantos, int total);
  callbackStatusCarregamento()
  {
    PonteiroProThis = NULL;
    ponteiroFuncao = NULL;
  }
};


#endif
