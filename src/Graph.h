#ifndef __GRAPH_H
#define __GRAPH_H

#include <vector>
#include <memory>

struct TVerticesGerais;
struct TAresta;

class Graph
{
public:
	Graph();
	std::shared_ptr<TVerticesGerais> _verticesGerais;
	std::vector< std::shared_ptr<TAresta> > _arestas;
	void merge( std::shared_ptr<Graph> other );
};

#endif