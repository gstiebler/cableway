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
	std::shared_ptr<Graph> _graph;
	std::shared_ptr<ElectricalElements> _electricalElements;
  std::string NomeArquivo;
  double Altura;
};
//---------------------------------------------------------------------------
#endif
