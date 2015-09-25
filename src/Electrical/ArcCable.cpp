#include "ArcCable.h"

#include "UDadosGenerico.h"
#include "UDefines.h"

ArcCable::ArcCable( std::shared_ptr<TDesenho> drawing, std::shared_ptr<TArco> arc ) :
	ElectricalElement( drawing ), 
	_arc( arc )
{
	TPonto p[2];
    _arc->PontasArco( p );
	for( int i(0); i < 2; ++i )
	{
		_edges[i] = make_shared<TVerticeGeral>();
		_edges[i]->TipoVertice = VERTICE_ARCO;
		_edges[i]->TipoElemento = INSTRUMENTO;
		_edges[i]->drawing = drawing;
		_edges[i]->pos = p[i];
	}
} 