#include "Graph.h"
#include "UVerticesArestas.h"

using namespace std;

Graph::Graph() :
	_verticesGerais( new TVerticesGerais )
{

}

void Graph::merge( shared_ptr<Graph> other )
{
	_arestas.insert( _arestas.end(), other->_arestas.begin(), other->_arestas.end() );
	_verticesGerais->vertices.insert( _verticesGerais->vertices.end(), other->_verticesGerais->vertices.begin(), other->_verticesGerais->vertices.end() );
}