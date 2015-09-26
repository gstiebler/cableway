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
		_edges[i]->TipoElemento = INSTRUMENTO;
		_edges[i]->drawing = _drawing;
		_edges[i]->pos = _multipoint->pontos[i];
		_graph->_verticesGerais->Adiciona( _edges[i] );
	}
}


void StraightCable::connectEdge( shared_ptr<TVerticeGeral> edge )
{
	const double MIN_DIST = 10e-1;
	TPonto line[2];
	for( int i(0); i < 2; ++i )
		line[i] = _multipoint->pontos[i];

	TPonto pointInLine;
	double distance = DistPontoParaSegmentoReta( line, edge->pos, pointInLine  );
	if( distance < MIN_DIST ) // if the edge is over a line of the cable, add this edge
		_edges.push_back( edge );
}