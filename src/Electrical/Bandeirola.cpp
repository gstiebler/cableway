#include "Bandeirola.h"

#include <limits>
#include <algorithm> 
#include "UDadosGenerico.h"
#include "UErros.h"
#include "TDesenho.h"
#include "UGeometria.h"
#include "Graph.h"

using namespace std;

Bandeirola::Bandeirola( shared_ptr<Graph> graph, shared_ptr<TDesenho> drawing, TListaItensCelula &groupItems ) :
	ElectricalElement( graph, drawing ), 
	_arcs( groupItems._arcs ),
	_multipoints( groupItems._multipoints ),
	_texts( groupItems._texts ),
	_isValid( true )
{
    initializeEdges();
}


void Bandeirola::initializeEdges()
{
	if( _texts.size() > 1  )
	{
		CErrosMsg *erros = CErrosMsg::getInstance();
		erros->novoErro("No desenho " + _drawing->NomeArquivo
                        + " existe um grupamento em nível de bandeirola com mais de um texto associado. Textos: " );
        for ( auto text : _texts )
            erros->novoErro( text->texto );
        erros->novoErro( "" );
		_isValid = false;
		return;
	}
	else if ( _texts.size() == 0 )
	{
        CErrosMsg *erros = CErrosMsg::getInstance();
        erros->novoErro( "No desenho " + _drawing->NomeArquivo + " existe um grupamento em nível de bandeirola sem textos associados.\n" );
		_isValid = false;
		return;
	}

	vector<TPonto> internalEdges;
	getFreeInternalEdges( internalEdges );

	if( internalEdges.size() == 0 )
	{
        CErrosMsg *erros = CErrosMsg::getInstance();
		erros->novoErro( "No desenho " + _drawing->NomeArquivo + " a bandeirola " + _texts[0]->texto + " possui formato inválido\n");
		_isValid = false;
		return;
	}

	_edges.resize( 1 );
	_edges[0] = make_shared<TVerticeGeral>();
    _edges[0]->TipoVertice = VERTICE_BANDEIROLA;
	_edges[0]->drawing = _drawing;
	_edges[0]->texto = _texts[0]->texto;

	if( internalEdges.size() == 1 )
		_edges[0]->pos = internalEdges[0]; // if there´s only one free edge, use it
	else
	{   // if there are more than 1 free edge, use the farthest edge from the text
		TPonto farthestPoint;
		PontoMaisDistante( _texts[0]->origem, internalEdges, farthestPoint );
		_edges[0]->pos = farthestPoint;
	}
	
	_graph->_verticesGerais->Adiciona( _edges[0] );
}


void Bandeirola::getFreeInternalEdges( std::vector<TPonto> &internalEdges )
{
	for( auto multiPoint : _multipoints )
	{
		internalEdges.push_back( multiPoint->pontos.front() );
		internalEdges.push_back( multiPoint->pontos.back() );
	}

	for ( auto arc : _arcs )
	{
        TPonto pontas[2];
        arc->PontasArco( pontas );
        // Get arc edges and insert on edges list
        internalEdges.push_back( pontas[0] );
        internalEdges.push_back( pontas[1] );
	}
}


bool Bandeirola::isValid()
{
	return _isValid;
}


double Bandeirola::getWidth()
{
	double minX = std::numeric_limits<double>::max();
	double maxX = std::numeric_limits<double>::min();
	for( auto multiPoint : _multipoints )
	{
		for( auto point : multiPoint->pontos )
		{
			minX = min( minX, point.x );
			maxX = min( maxX, point.x );
		}
	}

	for( auto arc : _arcs )
	{
		minX = min( minX, arc->Centro.x - arc->EixoPrimario );
		maxX = max( maxX, arc->Centro.x + arc->EixoPrimario );
	}

	return maxX - minX;
}