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
  double Altura;
  ~TDesenho();
};
//---------------------------------------------------------------------------
#endif
