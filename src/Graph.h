#ifndef __GRAPH_H
#define __GRAPH_H

#include <vector>
#include <memory>

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
	std::shared_ptr<TVerticesGerais> _verticesGerais;
	std::vector< std::shared_ptr<TAresta> > _arestas;
	void merge( std::shared_ptr<Graph> other );

	
    bool generateDistanceTree( std::shared_ptr<TVerticeGeral> vertice[2], std::vector< std::shared_ptr<TVerticeGeral> > &anterior, 
		std::vector< std::shared_ptr<TAresta> > &vArestas, std::string layer );
};

#endif