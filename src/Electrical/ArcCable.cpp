#include "ArcCable.h"

#include "UDadosGenerico.h"
#include "UDefines.h"
#include "Graph.h"
#include "UGeometria.h"

ArcCable::ArcCable( shared_ptr<Graph> graph, shared_ptr<TDesenho> drawing, shared_ptr<TArco> arc ) :
	ElectricalElement( graph, drawing ), 
	_arc( arc )
{
    initializeEdges();
	addInternalEdge();
} 


void ArcCable::addInternalEdge()
{
	TPonto p[2];
	shared_ptr<TAresta> Aresta = make_shared<TAresta>( _arc->layerName );
	Aresta->AdicionaVertices( _edges[0], _edges[1], DistPontosManhattan( p[0], p[1] ) );
    Aresta->_drawing = _drawing;
	_graph->_arestas.push_back( Aresta );
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