#include "Instrument.h"

#include <limits>
#include <algorithm> 
#include "UVerticesArestas.h"
#include "UDadosGenerico.h"

using namespace std;

Instrument::Instrument( std::shared_ptr<TDesenho> drawing, TListaItensCelula &groupItems ) :
	ElectricalElement( drawing ), 
	_arcs( groupItems._arcs ),
	_multipoints( groupItems._multipoints ),
	_texts( groupItems._texts )
{
	initializeEdges();
}


void Instrument::initializeEdges()
{
	instrumentVertices.resize( _texts.size() );
	TPonto center = getCenter();
	//para cada texto da célula cria um vértice
	for ( int i(0); i < _texts.size(); ++i )
    {
        instrumentVertices[i] = make_shared<TVerticeGeral>();
        instrumentVertices[i]->TipoElemento = INSTRUMENTO;
		instrumentVertices[i]->drawing = _drawing;
        instrumentVertices[i]->texto = _texts[i]->texto;
        instrumentVertices[i]->pos = center;
        instrumentVertices[i]->TipoVertice = VERTICE_CENTRO_INSTRUMENTO;
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