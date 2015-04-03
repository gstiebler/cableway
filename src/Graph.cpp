#include "Graph.h"
#include "UVerticesArestas.h"
#include <queue>

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
	_verticesGerais( new TVerticesGerais )
{

}


void Graph::merge( shared_ptr<Graph> other )
{
	_arestas.insert( _arestas.end(), other->_arestas.begin(), other->_arestas.end() );
	_verticesGerais->vertices.insert( _verticesGerais->vertices.end(), other->_verticesGerais->vertices.begin(), other->_verticesGerais->vertices.end() );
}


bool Graph::generateDistanceTree( shared_ptr<TVerticeGeral> vertice[2], vector< shared_ptr<TVerticeGeral> > &anterior, vector< shared_ptr<TAresta> > &vArestas, string layer )
{
	//vector<int> PaisVertices( VerticesGerais->vertices.size() );//armazena os pais de cada vértice na �rvore
	vector<double> DistanciaDjikstra( _verticesGerais->vertices.size() );
    shared_ptr<TVerticeGeral> VerticeTemp;
    TVerticeEAresta *VerticeEArestaTemp;
	shared_ptr<TVerticeGeral> vfila, vatual;
    double dist;
    TListaVerticesEArestas *ListaVerticesEArestasT;
    priority_queue<VerticesDjikstra> heap;
    heap.push(VerticesDjikstra(vertice[0], 0));
	bool achou_final = false;

	    /* initialize single source */
    for ( int n = 0; n < _verticesGerais->vertices.size(); n++ )
    {
        DistanciaDjikstra[n] = Infinity;
        anterior[n] = shared_ptr<TVerticeGeral>();
		_verticesGerais->vertices[n]->IndiceOriginal = n;
    }

	DistanciaDjikstra[ vertice[0]->IndiceOriginal ] = 0; // Distância do vértice pra ele mesmo � zero.

    while(heap.size())
    {
		vfila = heap.top()._vertex;
        dist = heap.top().distancia;
        heap.pop();

        if(vfila == vertice[1])
            achou_final = true;

		if(dist > DistanciaDjikstra[vfila->IndiceOriginal])
            continue;

        ListaVerticesEArestasT = vfila->ListaVerticesEArestas;
        for(int n = 0; n < ListaVerticesEArestasT->list.size(); n++)
        {
            VerticeEArestaTemp = ListaVerticesEArestasT->getVerticeEAresta(n);
			shared_ptr<TAresta> edge = VerticeEArestaTemp->Aresta;
			if ( edge->_layer != "" && edge->_layer != layer )
				continue;

            vatual = VerticeEArestaTemp->Vertice;
            int alt;

			alt = DistanciaDjikstra[vfila->IndiceOriginal] + edge->Tam;
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
							 vector< shared_ptr<TAresta> > &ListaArestas, std::vector< std::string > &vertexLabels, shared_ptr<TVerticeGeral> vertex )
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