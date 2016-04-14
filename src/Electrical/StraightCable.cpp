#include "StraightCable.h"

#include "UDadosGenerico.h"
#include "UDefines.h"
#include "Graph.h"
#include "UGeometria.h"

using namespace std;

StraightCable::StraightCable( shared_ptr<Graph> graph, shared_ptr<TDesenho> drawing, shared_ptr<TMultipoint> multipoint ) :
	ElectricalElement( graph, drawing ), 
	_multipoint( multipoint )
{
    initializeEdges();
}


void StraightCable::initializeEdges()
{
	_edges.resize( 2 );
	for( int i(0); i < 2; ++i )
	{
		_edges[i] = make_shared<TVerticeGeral>();
		_edges[i]->TipoVertice = VERTICE_PONTA_CABO;
		_edges[i]->TipoElemento = CABO;
		_edges[i]->drawing = _drawing;
		_edges[i]->pos = _multipoint->pontos[i];
		_edges[i]->_layer = _multipoint->layerName;
		_graph->_verticesGerais->Adiciona( _edges[i] );
	}
	for( auto edge : _edges )
		_internalEdges.push_back( edge );
}


void StraightCable::connectEdge( shared_ptr<TVerticeGeral> edge )
{
	const double MIN_DIST = 10e-1;
	const double MAX_DIST = 10e-10;
	TPonto line[2];
	for( int i(0); i < 2; ++i )
		line[i] = _multipoint->pontos[i];

	TPonto pointInLine;
	double distance = DistPontoParaSegmentoReta( line, edge->pos, pointInLine );
	// if the edge is over a line of the cable, create an vertex inside cable and connect it to the received edge
	if( distance < MIN_DIST )
	{
		if( distance < MAX_DIST )
			_internalEdges.push_back( edge );
		else
		{
			shared_ptr<TVerticeGeral> edgeInsideCable = make_shared<TVerticeGeral>();
			edgeInsideCable->TipoElemento = CABO;
			edgeInsideCable->drawing = _drawing;
			edgeInsideCable->pos = pointInLine;
			edgeInsideCable->texto = edge->texto;
			_internalEdges.push_back( edgeInsideCable );
			_graph->_verticesGerais->Adiciona( edgeInsideCable );

			shared_ptr<TAresta> Aresta = make_shared<TAresta>( _multipoint->layerName );
			Aresta->AdicionaVertices( edge, edgeInsideCable, distance );
			Aresta->_drawing = _drawing;
			_graph->_arestas.push_back( Aresta );
		}
	}
}


void StraightCable::sortEdges()
{            
	double DifX = fabs( _multipoint->pontos[0].x - _multipoint->pontos[1].x );
    double DifY = fabs( _multipoint->pontos[0].y - _multipoint->pontos[1].y );

	auto orderFunc = DifY > DifX ? OrdenaRetaVertical : OrdenaRetaHorizontal;
    sort( _internalEdges.begin(), _internalEdges.end(), orderFunc );
}


void StraightCable::generateEdges()
{
	sortEdges();

	for ( int i(0); i < _internalEdges.size() - 1; ++i ) 
    {
		shared_ptr<TAresta> Aresta = make_shared<TAresta>( _multipoint->layerName );
		double distance = DistPontos( _internalEdges[i]->pos, _internalEdges[i + 1]->pos );
		Aresta->AdicionaVertices( _internalEdges[i], _internalEdges[i + 1], distance );
		Aresta->_drawing = _drawing;
		_graph->_arestas.push_back( Aresta );
    }
}