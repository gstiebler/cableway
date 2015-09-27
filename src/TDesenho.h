//---------------------------------------------------------------------------
#ifndef TDesenhoH
#define TDesenhoH
//---------------------------------------------------------------------------
#include <string>

class CDadosGenerico;
class Graph;

struct TDesenho
{
	std::shared_ptr<CDadosGenerico> _dados;
	shared_ptr<Graph> _graph;
  string NomeArquivo;
  double Altura;
};
//---------------------------------------------------------------------------
#endif
