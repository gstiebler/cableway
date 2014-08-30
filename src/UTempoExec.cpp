//---------------------------------------------------------------------------


#pragma hdrstop

#include "UTempoExec.h"

// Necessário para definir o caráter singleton da classe. Ou seja, zerar inicialmente a instância
CTempoExec * CTempoExec::instance = NULL;

//---------------------------------------------------------------------------
CTempoExec::CTempoExec()
{
  PeriodoContador = freq = 0;
#ifdef DEBUG_BUILDER
  QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
  __int64 Ctr1, Ctr2;
  QueryPerformanceCounter((LARGE_INTEGER *)&Ctr1);
  QueryPerformanceCounter((LARGE_INTEGER *)&Ctr2);
  overhead = Ctr2 - Ctr1; // determine API overhead
 
  PeriodoContador=1000.0/freq;
#endif
}

void CTempoExec::destroi()
{
  if ( instance )
    delete instance;
  instance = 0;
}
CTempoExec* CTempoExec::getInstance()
{
  if ( !instance )
  {
    instance = new CTempoExec();
  }

  return instance;
}

void CTempoExec::MarcaTempo(char *texto)
{
#ifdef DEBUG_BUILDER
  TTempo tempo;
  tempo.Nome=texto;
  QueryPerformanceCounter((LARGE_INTEGER *)&tempo.Momento);
  Tempos.push_back(tempo);
#endif
}

void CTempoExec::MarcaTempo(string texto)
{
#ifdef DEBUG_BUILDER
  TTempo tempo;
  tempo.Nome=texto.c_str();
  QueryPerformanceCounter((LARGE_INTEGER *)&tempo.Momento);
  Tempos.push_back(tempo);
#endif
}
void CTempoExec::MostraTempo(string nome)
{

#ifdef DEBUG_BUILDER
  TStringList *proArquivo = new TStringList();
  for ( int i = 0 ; i <(int) Tempos.size()-1 ; i++ )
  {
    AnsiString TempoString;
    double diferenca;
    diferenca = ( (double)Tempos.at(i+1).Momento - Tempos.at(i).Momento - overhead ) / freq;
    TempoString = Tempos.at(i+1).Nome;
    proArquivo->Add(TempoString + ": " + FormatFloat("###,###,###",diferenca * 1000) + "ms");
  }

  proArquivo->SaveToFile(ExtractFilePath(Application->ExeName) + nome.c_str());
  delete proArquivo;
/* Debug.Print "("; Ctr1; "-"; Ctr2; "-"; Overhead; ") /"; Freq
     Debug.Print "100 additions took";
     Debug.Print (Ctr2 - Ctr1 - Overhead) / Freq; "seconds"*/


  Tempos.clear();

#endif
}


#pragma package(smart_init)
