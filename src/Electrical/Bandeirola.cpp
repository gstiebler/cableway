#include "Bandeirola.h"

#include "UDadosGenerico.h"

Bandeirola::Bandeirola( std::shared_ptr<TDesenho> drawing, TListaItensCelula &groupItems ) :
	ElectricalElement( drawing ), 
	_arcs( groupItems._arcs ),
	_multipoints( groupItems._multipoints ),
	_texts( groupItems._texts )
{
	_edges.resize( _texts.size() );
}