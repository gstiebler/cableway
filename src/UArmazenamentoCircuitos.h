//---------------------------------------------------------------------------

#ifndef UArmazenamentoCircuitosH
#define UArmazenamentoCircuitosH
//---------------------------------------------------------------------------
#include "UAuxString.h"
//#include "UdmCentral.h"
//#include "UdmORetorno.h"
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
  string rota;
};
//---------------------------------------------------------------------------

class CArmazenamentoCircuitos
{
private:
public:
  CArmazenamentoCircuitos();
  ~CArmazenamentoCircuitos();
  void static GeraExcelRelatorio(string NomeArq);
  void static VetorCircuitos(TCircuito *Circuitos);
  int static NumReg();
  bool static PegaCircuito(string nome, TCircuito &Circuito);
  void static AtualizaRotaMetragem(TCircuito &Circuito);
  void static DeuErro(TCircuito &Circuito, string erro);
};
//--------------------------------------------------------------------------- 
#endif
