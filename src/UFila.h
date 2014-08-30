//---------------------------------------------------------------------------

#ifndef UFilaH
#define UFilaH
//---------------------------------------------------------------------------
#endif

//#include <vcl.h>

class CFila
{
private:  
  int tam, ini, fim, NumItens;
  int *lista;
public:
  CFila(int Tam);
  ~CFila();
  void Adiciona(int Vertice);
  int Retira();
  int Tamanho();
};              
//---------------------------------------------------------------------------
