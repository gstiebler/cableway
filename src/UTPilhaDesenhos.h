//---------------------------------------------------------------------------

#ifndef UTPilhaDesenhosH
#define UTPilhaDesenhosH
//---------------------------------------------------------------------------

#include "UDefines.h"

#include <string>

struct TPilhaDesenhos
{
  private:
    TStringList PilhaArquivos;
    std::string StringPilha;
    void MontaString();
    int No;
    TVectorInt Nos;
    TStringList ListaNos;
  public:
    TPilhaDesenhos();
    ~TPilhaDesenhos();
    void Adiciona(std::string nome);
    void Retira();
    string StringPilhaArquivos();
    int GetNo();
};

#endif
