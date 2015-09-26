#include "Instrument.h"

#include <limits>
#include <algorithm> 
#include "UVerticesArestas.h"
#include "UDadosGenerico.h"
#include "Graph.h"

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
	//para cada texto da c�lula cria um v�rtice
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
			maxX = min( maxX, point.x );
			minY = max( minY, point.y );
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
			maxX = min( maxX, edge.x );
			minY = max( minY, edge.y );
			maxY = max( maxY, edge.y );
		}
	}
	return TPonto( (maxX - minX) / 2, (maxY - minY) / 2 );
}


void Instrument::connectEdge( shared_ptr<TVerticeGeral> edge )
{

}