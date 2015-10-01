//---------------------------------------------------------------------------

#ifndef UArmazenamentoCircuitosH
#define UArmazenamentoCircuitosH
//---------------------------------------------------------------------------

#include <math.h>

#define NUM_GRANDE 1000

struct TCircuito
{
  //imputado
  std::string NomeCircuito;
  int Quantidade;
  std::string Tipo;
  std::string Origem;
  std::string Destino;
  std::string RotaUsuario;
  int ID;

  //obtido do desenho
  double metragem;
  std::vector<std::string> rota;
};
//---------------------------------------------------------------------------
 
#endif
