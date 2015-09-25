#ifndef INSTRUMENT_H__
#define INSTRUMENT_H__

#include <vector>
#include <memory>
#include "UListaItensCelula.h"
#include "ElectricalElement.h"

struct TTexto;
struct TArco;
struct TMultipoint;

class Instrument : public ElectricalElement
{
public:
	Instrument( std::shared_ptr<TDesenho> drawing, TListaItensCelula &groupItems );
	std::vector< std::shared_ptr<TTexto> > _texts;
	std::vector< std::shared_ptr<TArco> > _arcs;
	std::vector< std::shared_ptr<TMultipoint> > _multipoints;
};

#endif