//---------------------------------------------------------------------------
#ifndef TDesenhoH
#define TDesenhoH
//---------------------------------------------------------------------------
#include <string>

class CDadosGenerico;
class Graph;
class ElectricalElements;

struct TDesenho
{
	std::shared_ptr<CDadosGenerico> _dados;
	shared_ptr<Graph> _graph;
	shared_ptr<ElectricalElements> _electricalElements;
  string NomeArquivo;
  double Altura;
};
//---------------------------------------------------------------------------
#endif
