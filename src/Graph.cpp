#include "Graph.h"
#include "UVerticesArestas.h"
#include <queue>
#include <limits>

using namespace std;

VerticesDjikstra::VerticesDjikstra(shared_ptr<TVerticeGeral> vertex, double Distancia)
{
    _vertex = vertex;
    distancia = Distancia;
}


bool VerticesDjikstra::operator<(const VerticesDjikstra& right) const
{
    return distancia > right.distancia;
}


bool VerticesDjikstra::operator>(const VerticesDjikstra& right) const
{
    return distancia < right.distancia;
}


Graph::Graph() :
	_verticesGerais( make_shared<TVerticesGerais>() )
{
}


void Graph::merge( shared_ptr<Graph> other )
{
	_arestas.insert( _arestas.end(), other->_arestas.begin(), other->_arestas.end() );
	_verticesGerais->vertices.insert( _verticesGerais->vertices.end(), other->_verticesGerais->vertices.begin(), other->_verticesGerais->vertices.end() );
}


bool Graph::generateDistanceTree( shared_ptr<TVerticeGeral> vertice[2], vector< shared_ptr<TVerticeGeral> > &anterior, vector< shared_ptr<TAresta> > &vArestas, string layer )
{
	vector<double> DistanciaDjikstra( _verticesGerais->vertices.size() );
    priority_queue<VerticesDjikstra> heap;
    heap.push(VerticesDjikstra(vertice[0], 0));
	bool achou_final = false;

	/* initialize single source */
    for ( int n = 0; n < _verticesGerais->vertices.size(); n++ )
    {
        DistanciaDjikstra[n] = std::numeric_limits<double>::max();
        anterior[n] = shared_ptr<TVerticeGeral>();
		_verticesGerais->vertices[n]->IndiceOriginal = n;
    }

	DistanciaDjikstra[ vertice[0]->IndiceOriginal ] = 0; // Distância do vértice pra ele mesmo � zero.

    while(heap.size())
    {
		shared_ptr<TVerticeGeral> vfila = heap.top()._vertex;
		//printf( "VFila: %f %f %d %s\n", vfila->pos.x, vfila->pos.y, vfila->IndiceOriginal, vfila->texto.c_str() );
        double dist = heap.top().distancia;
        heap.pop();

        if(vfila == vertice[1])
            achou_final = true;

		if(dist > DistanciaDjikstra[vfila->IndiceOriginal])
            continue;

        for( auto VerticeEArestaTemp : vfila->ListaVerticesEArestas->list )
        {
			shared_ptr<TAresta> edge = VerticeEArestaTemp.Aresta;
			if ( edge->_layer != "" && edge->_layer != layer )
				continue;

            shared_ptr<TVerticeGeral> vatual = VerticeEArestaTemp.Vertice;
			
			//printf( "vatual: %f %f %d %s\n", vatual->pos.x, vatual->pos.y, vatual->IndiceOriginal, vatual->texto.c_str() );
            int alt = DistanciaDjikstra[vfila->IndiceOriginal] + edge->Tam;
            if ( alt < DistanciaDjikstra[vatual->IndiceOriginal] )
            {
				DistanciaDjikstra[vatual->IndiceOriginal] = alt;
                anterior[vatual->IndiceOriginal] = vfila;
                vArestas[vatual->IndiceOriginal] = edge;
                heap.push(VerticesDjikstra(vatual, alt));
            }
        }
    }

	return achou_final;
}


void Graph::getEdgesFromPath( vector< shared_ptr<TVerticeGeral> > &anterior, vector< shared_ptr<TAresta> > &vArestas, 
				 vector< shared_ptr<TAresta> > &ListaArestas, std::vector< std::string > &vertexLabels, shared_ptr<TVerticeGeral> vertex ) const
{
	string lastText;
	shared_ptr<TAresta> ArestaTemp;
	shared_ptr<TVerticeGeral> vatual = vertex;
	while (anterior[vatual->IndiceOriginal].get() != 0)
    {
		if( !vatual->texto.empty() && vatual->texto != lastText )
		{
			vertexLabels.push_back( vatual->texto );
			lastText = vatual->texto;
		}
		ArestaTemp = vArestas[vatual->IndiceOriginal];
		ListaArestas.push_back( ArestaTemp );
		vatual = anterior[vatual->IndiceOriginal];
    }
}


void Graph::GeraListaAdjacencias()
{
	for ( auto edge : _arestas )
		TAresta::createAdjancency( edge );
}


Graph* Graph::getCopy() const
{
	Graph *copy = new Graph();
	map<TVerticeGeral*, shared_ptr<TVerticeGeral>> verticesMap;
	for( int i(0); i < _verticesGerais->vertices.size(); ++i )
	{
		shared_ptr<TVerticeGeral> verticeTemp = make_shared<TVerticeGeral>( *(_verticesGerais->vertices[i].get()) );
		verticesMap[_verticesGerais->vertices[i].get()] = verticeTemp;
		copy->_verticesGerais->vertices.push_back( verticeTemp );
	}

	for( int i(0); i < _arestas.size(); ++i)
	{
		shared_ptr<TAresta> newEdge = make_shared<TAresta>( _arestas[i].get() );

		for( int j(0); j < 2; ++j )
			newEdge->_vertices[j] = verticesMap[ _arestas[i]->_vertices[j].get() ];

		copy->_arestas.push_back( newEdge );
	}

	copy->GeraListaAdjacencias();
	return copy;
}


void Graph::reduce()
{
	vector<shared_ptr<TVerticeGeral> > vertices[2];

	for( int i(0); i < _verticesGerais->vertices.size(); ++i)
	{
		if( !_verticesGerais->vertices[i]->texto.empty() ) 
			continue;

		shared_ptr<TListaVerticesEArestas> listaVerticesEArestas = _verticesGerais->vertices[i]->ListaVerticesEArestas;
		if( listaVerticesEArestas->list.size() == 1 )
			vertices[0].push_back( _verticesGerais->vertices[i] );
	}

	// eliminate vertices with no text and only one edge
	for( int i(0); i < vertices[0].size(); ++i )
	{
		shared_ptr<TVerticeGeral> vertex = vertices[0][i];
		// mark the edge to be removed
		vertex->ListaVerticesEArestas->list[0].Aresta->_autogenId = -1;

		vertex->removeEdges();

		// mark the vertex to be removed
		vertex->_autogenId = -1;
	}

	for( int i(0); i < _verticesGerais->vertices.size(); ++i)
	{
		shared_ptr<TVerticeGeral> vertex = _verticesGerais->vertices[i];
		if( !vertex->texto.empty() ) 
			continue;

		shared_ptr<TListaVerticesEArestas> listaVerticesEArestas = vertex->ListaVerticesEArestas;
		if ( listaVerticesEArestas->list.size() == 2 )
			vertices[1].push_back( vertex );
	}

	// eliminate vertices with no text and two edges
	for( int i(0); i < vertices[1].size(); ++i )
	{
		shared_ptr<TVerticeGeral> vertex = vertices[1][i];
		shared_ptr<TAresta> newEdge = make_shared<TAresta>( vertex->ListaVerticesEArestas->list[0].Aresta->_layer );

		// remove old edges
		for( int j(0); j < 2; ++j )
		{
			TVerticeEAresta &verticeEAresta = vertex->ListaVerticesEArestas->list[j];
			newEdge->Tam += verticeEAresta.Aresta->Tam;
			newEdge->_vertices[j] = verticeEAresta.Vertice;

			// mark the edge to be removed
			verticeEAresta.Aresta->_autogenId = -1;
			vertex->removeEdges();
			vertex->_autogenId = -1;
		}
		_arestas.push_back( newEdge );
		TAresta::createAdjancency( newEdge );
	}

	// remove marked vertices
	for( int i( _verticesGerais->vertices.size() - 1); i >= 0; --i )
	{
		if( _verticesGerais->vertices[i]->_autogenId < 0 )
			_verticesGerais->vertices.erase( _verticesGerais->vertices.begin() + i );
	}

	// remove marked edges
	for( int i( _arestas.size() - 1); i >= 0; --i ) 
	{
		if( _arestas[i]->_autogenId < 0 )
			_arestas.erase( _arestas.begin() + i );
	}
}