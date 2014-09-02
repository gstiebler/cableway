//---------------------------------------------------------------------------

#ifndef UTempoExecH
#define UTempoExecH
//---------------------------------------------------------------------------

#include "UDefines.h"
#include <vector>
#include <string>
#ifdef DEBUG_BUILDER
  #include <vcl.h>
#endif

struct TTempo
{
  #ifdef DEBUG_BUILDER
    AnsiString Nome;
  #endif
  long int Momento;
};

class CTempoExec
{
private:
  static CTempoExec *instance;

  vector<TTempo> Tempos;
  long int freq, overhead;
  double PeriodoContador;
  CTempoExec();
public:
  static CTempoExec *getInstance();
  void MarcaTempo(char *texto);
  void MostraTempo(string nome);
  void MarcaTempo(string texto);
  static void destroi();
  static bool isNull(){return instance == 0;}
  ~CTempoExec(){};
};

#endif

