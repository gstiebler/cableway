#include "Instrument.h"

#include <limits>
#include <algorithm> 
#include "UVerticesArestas.h"
#include "UDadosGenerico.h"
#include "Graph.h"
#include "UGeometria.h"

using namespace std;

Instrument::Instrument( shared_ptr<Graph> graph, shared_ptr<TDesenho> drawing, TListaItensCelula &groupItems ) :
	ElectricalElement( graph, drawing ), 
	_arcs( groupItems._arcs ),
	_multipoints( groupItems._multipoints ),
	_texts( groupItems._texts )
{
	initializeEdges();
	createColarEdge();
}


void Instrument::initializeEdges()
{
	_instrumentVertices.resize( _texts.size() );
	TPonto center = getCenter();
	//para cada texto da célula cria um vértice
	for ( int i(0); i < _texts.size(); ++i )
    {
        _instrumentVertices[i] = make_shared<TVerticeGeral>();
        _instrumentVertices[i]->TipoElemento = INSTRUMENTO;
		_instrumentVertices[i]->drawing = _drawing;
        _instrumentVertices[i]->texto = _texts[i]->texto;
        _instrumentVertices[i]->pos = center;
        _instrumentVertices[i]->TipoVertice = VERTICE_CENTRO_INSTRUMENTO;
		_graph->_verticesGerais->Adiciona( _instrumentVertices[i] );
    }
}


void Instrument::createColarEdge()
{
	if( _instrumentVertices.size() == 2 )
	{        
		shared_ptr<TAresta> Aresta = make_shared<TAresta>( "" );
		// the 0.1 is used for this edge not to be the preferred one sometimes
        Aresta->AdicionaVertices( _instrumentVertices[0], _instrumentVertices[1], 0.1 );
        Aresta->_drawing = _drawing;
		_graph->_arestas.push_back( Aresta );
	}
}


TPonto Instrument::getCenter() const
{
	double minX = std::numeric_limits<double>::max();
	double minY = std::numeric_limits<double>::max();
	double maxX = -std::numeric_limits<double>::max();
	double maxY = -std::numeric_limits<double>::max();

	for( auto multiPoint : _multipoints )
	{
		for( auto point : multiPoint->pontos )
		{
			minX = min( minX, point.x );
			maxX = max( maxX, point.x );
			minY = min( minY, point.y );
			maxY = max( maxY, point.y );
		}
	}

	for( auto arc : _arcs )
	{
		TPonto edges[2];
		arc->PontasArco( edges );
		for( auto edge : edges )
		{
			minX = min( minX, edge.x );
			maxX = max( maxX, edge.x );
			minY = min( minY, edge.y );
			maxY = max( maxY, edge.y );
		}
	}
	return TPonto( (maxX + minX) / 2, (maxY + minY) / 2 );
}


void Instrument::connectEdge( shared_ptr<TVerticeGeral> geometricEdge )
{
	const double MIN_DIST = 10e-1;
	for( auto multiPoint : _multipoints ) // iterate on all multipoints of the instrument
	{
		auto pointsClosed = multiPoint->pontos;
		pointsClosed.push_back( multiPoint->pontos.front() );
		for( int i(0); i < pointsClosed.size() - 1; ++i) // iterate on all points of the multipoint
		{
			TPonto line[2];
			line[0] = pointsClosed[i];
			line[1] = pointsClosed[i + 1];
			TPonto pointInLine;
			double distance = DistPontoParaSegmentoReta( line, geometricEdge->pos, pointInLine  );
			if( distance < MIN_DIST ) // if the edge is over a line of the equipment, add this edge
				addEdgeToCenter( geometricEdge );
		}
	}

	for( auto arc : _arcs )
	{
		TPonto diffPoint = arc->Centro - geometricEdge->pos;
		if ( fabs( diffPoint.getLength() - arc->EixoPrimario ) < MIN_DIST ) // consider that the arc is a circle
			addEdgeToCenter( geometricEdge );
	}
}


void Instrument::addEdgeToCenter( shared_ptr<TVerticeGeral> geometricEdge )
{	
	for( auto instrumentVertex : _instrumentVertices ) // add an edge for all the instruments internal vertices
	{
		shared_ptr<TAresta> edge = make_shared<TAresta>( geometricEdge->_layer );
		edge->AdicionaVertices( instrumentVertex, geometricEdge, 0.0 );
		edge->_drawing = _drawing;
		_graph->_arestas.push_back( edge );
	}
}