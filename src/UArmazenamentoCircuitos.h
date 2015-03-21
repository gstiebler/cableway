//---------------------------------------------------------------------------

#ifndef UArmazenamentoCircuitosH
#define UArmazenamentoCircuitosH
//---------------------------------------------------------------------------

#include <math.h>

#define NUM_GRANDE 1000

struct TCircuito
{
  //imputado
  string NomeCircuito;
  int Quantidade;
  string Tipo;
  string Origem;
  string Destino;
  string RotaUsuario;
  int ID;

  //obtido do desenho
  double metragem;
  vector<string> rota;
};
//---------------------------------------------------------------------------
 
#endif
