#include "ArcCable.h"

#include "UDadosGenerico.h"
#include "UDefines.h"
#include "Graph.h"

ArcCable::ArcCable( shared_ptr<Graph> graph, shared_ptr<TDesenho> drawing, shared_ptr<TArco> arc ) :
	ElectricalElement( graph, drawing ), 
	_arc( arc )
{
    initializeEdges();
} 


void ArcCable::initializeEdges()
{
	TPonto p[2];
    _arc->PontasArco( p );
	_edges.resize( 2 );
	for( int i(0); i < 2; ++i )
	{
		_edges[i] = make_shared<TVerticeGeral>();
		_edges[i]->TipoVertice = VERTICE_ARCO;
		_edges[i]->TipoElemento = INSTRUMENTO;
		_edges[i]->drawing = _drawing;
		_edges[i]->pos = p[i];
		_graph->_verticesGerais->Adiciona( _edges[i] );
	}
}


void ArcCable::connectEdge( shared_ptr<TVerticeGeral> edge )
{
}