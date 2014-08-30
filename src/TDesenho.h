//---------------------------------------------------------------------------
#ifndef TDesenhoH
#define TDesenhoH
//---------------------------------------------------------------------------
#include <string>

class CGrafoDesenho;
struct TDesenho
{
  CGrafoDesenho *GrafoDesenho;
  string NomeArquivo;
  int ID;
  double Altura;
  ~TDesenho();
};
//---------------------------------------------------------------------------
#endif
