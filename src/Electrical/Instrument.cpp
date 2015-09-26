#include "Instrument.h"

Instrument::Instrument( std::shared_ptr<TDesenho> drawing, TListaItensCelula &groupItems ) :
	ElectricalElement( drawing ), 
	_arcs( groupItems._arcs ),
	_multipoints( groupItems._multipoints ),
	_texts( groupItems._texts )
{
}


void Instrument::connectEdges( std::vector< std::shared_ptr<TVerticeGeral> >& edges )
{
}