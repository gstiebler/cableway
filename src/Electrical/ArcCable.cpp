#include "ArcCable.h"

#include "UDadosGenerico.h"
#include "UDefines.h"
#include "Graph.h"
#include "UGeometria.h"

using namespace std;

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
	_arc->PontasArco( p );
	shared_ptr<TAresta> Aresta = make_shared<TAresta>( _arc->layerName );
	double dist = DistPontosManhattan( p[0], p[1] );
	Aresta->AdicionaVertices( _edges[0], _edges[1], dist );
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
		_edges[i]->_layer = _arc->layerName;
		_graph->_verticesGerais->Adiciona( _edges[i] );
	}
}


void ArcCable::connectEdge( std::shared_ptr<TVerticeGeral> geometricEdge )
{
	const double MIN_DIST = 10e-1;
	// TODO connect to the middle of the arc
	for( auto internalEdge : _edges )
	{
		TPonto diffPoint = internalEdge->pos - geometricEdge->pos;
		if( diffPoint.getLength() < MIN_DIST )
		{
			shared_ptr<TAresta> edge = make_shared<TAresta>( geometricEdge->_layer );
			double dist = DistPontosManhattan( internalEdge->pos, geometricEdge->pos );
			edge->AdicionaVertices( internalEdge, geometricEdge, dist );
			edge->_drawing = _drawing;
			_graph->_arestas.push_back( edge );
		}
	}
}