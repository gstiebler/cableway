#include "StraightCable.h"

#include "UDadosGenerico.h"
#include "UDefines.h"

StraightCable::StraightCable( std::shared_ptr<TDesenho> drawing, std::shared_ptr<TMultipoint> multipoint ) :
	ElectricalElement( drawing ), 
	_multipoint( multipoint )
{
	_edges.resize( 2 );
	for( int i(0); i < 2; ++i )
	{
		_edges[i] = make_shared<TVerticeGeral>();
		_edges[i]->TipoVertice = VERTICE_PONTA_CABO;
		_edges[i]->TipoElemento = INSTRUMENTO;
		_edges[i]->drawing = drawing;
		_edges[i]->pos = _multipoint->pontos[i];
	}
}


void StraightCable::connectEdges( std::vector< std::shared_ptr<TVerticeGeral> >& edges )
{
}