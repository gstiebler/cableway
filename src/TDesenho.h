//---------------------------------------------------------------------------
#ifndef TDesenhoH
#define TDesenhoH
//---------------------------------------------------------------------------
#include <string>

class CGrafoDesenho;
struct TDesenho
{
  shared_ptr<CGrafoDesenho> GrafoDesenho;
  string NomeArquivo;
  double Altura;
};
//---------------------------------------------------------------------------
#endif
