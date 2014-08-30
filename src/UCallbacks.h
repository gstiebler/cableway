//---------------------------------------------------------------------------

#ifndef UCallbacksH
#define UCallbacksH
//---------------------------------------------------------------------------
struct callbackVerificaTexto
{
  void *PonteiroProThis;
  bool (*ponteiroFuncao)(void *PonteiroThis, const char *str);
  callbackVerificaTexto()
  {
    PonteiroProThis = 0;
    ponteiroFuncao = 0;
  }
};
#endif
