#ifndef BANDEIROLA_H__
#define BANDEIROLA_H__

#include <vector>
#include <memory>

struct TTexto;
struct TArco;
struct TMultipoint;

class Bandeirola
{
public:
   	std::vector< std::shared_ptr<TTexto> > _texts;
	std::vector< std::shared_ptr<TArco> > _arcs;
	std::vector< std::shared_ptr<TMultipoint> > _multipoints;
};

#endif