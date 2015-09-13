#ifndef __GRAPH_H
#define __GRAPH_H

#include <vector>
#include <memory>
#include <string>

struct TVerticesGerais;
struct TAresta;
struct TVerticeGeral;


class VerticesDjikstra
{
  public:
    bool operator<(const VerticesDjikstra&) const; 
    bool operator>(const VerticesDjikstra&) const;
    VerticesDjikstra(std::shared_ptr<TVerticeGeral> vertex, double Distancia);
    std::shared_ptr<TVerticeGeral> _vertex;
    double distancia;
};


class Graph
{
public:
	Graph();
	void merge( std::shared_ptr<Graph> other );
	void GeraListaAdjacencias();
	Graph* getCopy() const;
	void Graph::reduce();
    bool generateDistanceTree( std::shared_ptr<TVerticeGeral> vertice[2], std::vector< std::shared_ptr<TVerticeGeral> > &anterior, 
		std::vector< std::shared_ptr<TAresta> > &vArestas, std::string layer );

	void getEdgesFromPath( std::vector< std::shared_ptr<TVerticeGeral> > &anterior, std::vector< std::shared_ptr<TAresta> > &vArestas, 
				std::vector< std::shared_ptr<TAresta> > &ListaArestas, std::vector< std::string > &vertexLabels, std::shared_ptr<TVerticeGeral> vertex ) const;

	std::shared_ptr<TVerticesGerais> _verticesGerais;
	std::vector< std::shared_ptr<TAresta> > _arestas;
};

#endif
